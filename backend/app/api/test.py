import os
import time
import cv2
import numpy as np
import torch
from PIL import Image
from ultralytics import YOLO as UltralyticsYOLO, YOLO
from ultralytics.utils.plotting import Annotator, colors
import threading


class YOLOv8Detector:
    def __init__(self):
        # 设置默认模型路径（使用绝对路径更可靠）
        self.default_model_path = os.path.normpath(os.path.join(
            os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
            "models",
            "bug_best.pt"
        ))

        self.default_classes_path = os.path.normpath(os.path.join(
            os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
            "models",
            "bug_classic.txt"
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

        # 自动加载默认模型
        self.load_default_model()

    def load_default_model(self):
        """加载默认模型"""
        return self.load_model(self.default_model_path, self.default_classes_path)

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

                    print(f'{model_path} 模型和类别已加载.')
                    return True
                except Exception as e:
                    print(f'模型加载失败: {str(e)}')
                    return False
            return True

    # 其余方法保持不变...
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

                            # 绘制检测结果
                            cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                            cv2.putText(frame, label, (x1, y1 - 10),
                                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

                return frame, detections
            except Exception as e:
                print(f"检测失败: {str(e)}")
                return frame, []