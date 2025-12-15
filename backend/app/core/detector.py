import os
import time
import traceback

import cv2
import numpy as np
import torch
from PIL import Image
from ultralytics import YOLO as UltralyticsYOLO, YOLO
from ultralytics.utils.plotting import Annotator, colors
import threading
from collections import defaultdict
from .unknown_insect_detector import UnknownInsectDetector

STATS_FILE = "detection_stats.txt"
STATS_DIR = r"backend\app\web\stats"

class YOLOv8Detector:
    def __init__(self, db_service=None):
        # 设置默认模型路径（使用绝对路径更可靠）
        self.default_model_path = os.path.normpath(os.path.join(
            os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
            "models",
            "bug_best.pt"
        ))

        self.default_classes_path = os.path.normpath(os.path.join(
            os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
            "models",
            "bug_best.txt"
        ))

        # 初始化模型相关变量
        self.model = None
        self.class_names = []
        self.current_model_path = None
        self.current_classes_path = None
        self.device = None
        self.confidence = 0.5
        self.nms_iou = 0.3
        self.input_shape = [640, 640]
        self.letterbox_image = True
        self.cuda = False
        self.model_lock = threading.Lock()
        self.class_stats = defaultdict(int)
        self.total_detections = 0
        self.stats_lock = threading.Lock()

        # 初始化未知昆虫检测器
        self.unknown_insect_detector = UnknownInsectDetector(
            output_dir=os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'web', 'unknown_insects'),
            db_service=db_service
        )
        self.current_session_id = None

        # 自动加载默认模型
        self.load_default_model()

    def set_session_id(self, session_id):
        """设置当前会话ID"""
        self.current_session_id = session_id
        self.unknown_insect_detector.clear_session_data(session_id)

    def update_stats(self, detections):
        """更新类别统计信息"""
        with self.stats_lock:
            for detection in detections:
                class_name = detection["class_name"]
                self.class_stats[class_name] += 1
                self.total_detections += 1
    def load_default_model(self):
        """加载默认模型"""
        return self.load_model(self.default_model_path, self.default_classes_path)
    def reset_stats(self):
        """重置统计信息"""
        with self.stats_lock:
            self.stats = {
                "total_detections": 0,
                "class_counts": defaultdict(int),
                "start_time": time.time(),
                "last_update": time.time()
            }
        self.save_stats()

    def get_stats(self):
        """获取当前统计信息"""
        with self.stats_lock:
            return {
                "total_detections": self.stats["total_detections"],
                "class_counts": dict(self.stats["class_counts"]),
                "start_time": self.stats["start_time"],
                "last_update": self.stats["last_update"],
                "stats_file": STATS_FILE
            }
    def load_model(self, model_path=None, classes_path=None):
        """加载或切换模型"""
        # 如果没有提供路径，则使用默认路径
        model_path = model_path or self.default_model_path
        classes_path = classes_path or self.default_classes_path

        with self.model_lock:
            if model_path != self.current_model_path or classes_path != self.current_classes_path:
                try:
                    # 释放旧模型资源
                    if self.model is not None:
                        del self.model
                        torch.cuda.empty_cache()

                    # 加载新模型
                    self.model = YOLO(model_path)
                    self.current_model_path = model_path

                    # 加载类别名称
                    with open(classes_path, 'r', encoding='utf-8') as f:
                        self.class_names = [c.strip() for c in f.readlines()]
                    self.current_classes_path = classes_path

                    # 设置设备
                    self.device = torch.device('cuda' if torch.cuda.is_available() and self.cuda else 'cpu')
                    self.model.to(self.device)
                    self.model.eval()

                    # 重置统计信息
                    self.reset_stats()

                    print(f'{model_path} 模型和类别已加载.')
                    return True
                except Exception as e:
                    print(f'模型加载失败: {str(e)}')
                    return False
            return True

    def save_stats(self, reset_file=False):
        """保存统计信息到文件
        Args:
            reset_file (bool): 是否重置文件（启动服务器时设为True）
        """
        with self.stats_lock:  # 使用正确的锁变量名
            try:
                file_path = os.path.join(STATS_DIR, STATS_FILE)

                # 确保统计目录存在
                os.makedirs(STATS_DIR, exist_ok=True)

                # 启动时清空或新建文件
                if reset_file:
                    mode = 'w'  # 覆盖模式
                    print(f"[初始化] 新建/清空统计文件: {file_path}")
                else:
                    mode = 'a'  # 追加模式

                with open(file_path, mode, encoding='utf-8') as f:
                    # 写入时间戳标识（区分不同运行会话）
                    if reset_file:
                        f.write(f"=== 会话开始于 {time.strftime('%Y-%m-%d %H:%M:%S')} ===\n\n")

                    f.write("目标检测统计结果\n")
                    f.write("================\n")
                    f.write(
                        f"开始时间: {time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(self.stats['start_time']))}\n")
                    f.write(
                        f"最后更新时间: {time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(self.stats['last_update']))}\n")
                    f.write(f"总检测数量: {self.stats['total_detections']}\n\n")
                    f.write("各类别检测数量:\n")

                    # 按检测数量降序排序
                    sorted_stats = sorted(self.stats['class_counts'].items(), key=lambda x: x[1], reverse=True)
                    for class_name, count in sorted_stats:
                        f.write(f"{class_name}: {count}\n")

                    f.write("\n")  # 添加空行分隔不同记录
                    print(f"统计结果已保存到 {file_path}")
            except Exception as e:
                print(f"保存统计信息失败: {str(e)}")
                traceback.print_exc()


    def detect(self, frame):
        """执行目标检测"""
        with self.model_lock:  # 确保线程安全
            if self.model is None:
                print("模型未加载!")
                return frame, []

            try:
                # 执行推理
                results = self.model(frame, verbose=False)

                # 解析结果
                detections = []
                detected_bboxes = []  # 用于未知昆虫检测
                
                for result in results:
                    for box in result.boxes:
                        x1, y1, x2, y2 = map(int, box.xyxy[0].tolist())
                        conf = float(box.conf[0])
                        cls_id = int(box.cls[0])

                        if conf >= self.confidence:
                            label = f"{self.class_names[cls_id]}: {conf:.2f}"
                            detections.append({
                                "bbox": [x1, y1, x2, y2],
                                "confidence": conf,
                                "class_id": cls_id,
                                "class_name": self.class_names[cls_id]
                            })
                            detected_bboxes.append([x1, y1, x2, y2])

                            # 绘制检测结果
                            cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                            cv2.putText(frame, label, (x1, y1 - 10),
                                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

                # 检测未知昆虫（只在有会话时进行）
                if self.current_session_id is not None:
                    unknown_objects = self.unknown_insect_detector.extract_unknown_objects(
                        frame, detected_bboxes
                    )
                    
                    # 保存未知昆虫
                    for unknown_obj in unknown_objects:
                        self.unknown_insect_detector.save_unknown_insect(
                            frame, unknown_obj, self.current_session_id
                        )
                        
                        # 在帧上标记未知昆虫（红色框）
                        x1, y1, x2, y2 = unknown_obj['bbox']
                        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 0, 255), 2)
                        cv2.putText(frame, f"Unknown: {unknown_obj['confidence_score']:.2f}", 
                                    (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)

                # 更新统计信息
                if detections:
                    self.update_stats(detections)

                return frame, detections

            except Exception as e:
                print(f"检测异常: {str(e)}")
                traceback.print_exc()
                return frame, []

    def get_unknown_insects(self, session_id):
        """获取指定会话的未知昆虫列表"""
        return self.unknown_insect_detector.get_session_unknown_insects(session_id)

    def get_unknown_insects_statistics(self):
        """获取未知昆虫统计信息"""
        return self.unknown_insect_detector.get_statistics()