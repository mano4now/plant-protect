import cv2
import numpy as np
import os
import time
from datetime import datetime
import json
from collections import defaultdict

class UnknownInsectDetector:

    def __init__(self, output_dir="unknown_insects", db_service=None):
        """
        未知昆虫检测器
        Args:
            output_dir: 未知昆虫图片保存目录
            db_service: 数据库服务实例
        """
        self.output_dir = output_dir
        os.makedirs(output_dir, exist_ok=True)
        self.db_service = db_service
        
        # 检测参数
        self.min_area = 500          # 最小面积（从100提高到500，过滤掉太小的对象）
        self.max_area = 200000       # 最大面积（从20000提高到200000，确保大比例害虫能被检出）
        self.aspect_ratio_range = (0.2, 5.0)  # 长宽比范围（适当放宽）
        self.circularity_range = (0.2, 1.0)   # 圆形度范围（适当放宽）
        self.confidence_threshold = 0.25      # 置信度阈值（略微降低，让大虫体更容易通过）
        
        # 统计信息
        self.unknown_insects_count = 0
        self.session_unknown_insects = defaultdict(list)
        
    def is_insect_shape(self, contour):
        """
        判断轮廓是否符合昆虫形状特征
        """
        area = cv2.contourArea(contour)
        if area < self.min_area or area > self.max_area:
            return False
            
        perimeter = cv2.arcLength(contour, True)
        if perimeter == 0:
            return False
            
        # 计算圆形度
        circularity = 4 * np.pi * area / (perimeter * perimeter)
        
        # 计算长宽比
        x, y, w, h = cv2.boundingRect(contour)
        aspect_ratio = w / h if h > 0 else 0
        
        # 昆虫形状判断
        return (
            self.circularity_range[0] < circularity < self.circularity_range[1] and
            self.aspect_ratio_range[0] < aspect_ratio < self.aspect_ratio_range[1]
        )
    
    def has_insect_texture(self, image_patch):
        """
        分析图像纹理，判断是否为昆虫
        """
        if image_patch is None or image_patch.size == 0:
            return False
            
        # 转换为灰度图
        if len(image_patch.shape) == 3:
            gray = cv2.cvtColor(image_patch, cv2.COLOR_BGR2GRAY)
        else:
            gray = image_patch
            
        # 计算图像统计特征
        mean_val = np.mean(gray)
        std_val = np.std(gray)
        
        # 计算边缘密度
        edges = cv2.Canny(gray, 50, 150)
        edge_density = np.sum(edges > 0) / edges.size
        
        # 昆虫纹理特征判断
        # 昆虫通常有适中的亮度和较高的边缘密度
        return (
            30 < mean_val < 200 and  # 适中的亮度
            std_val > 20 and         # 有一定的纹理变化
            edge_density > 0.05      # 较高的边缘密度
        )
    
    def extract_unknown_objects(self, frame, detected_bboxes):
        """
        从帧中提取未知对象（未被YOLO检测到的疑似昆虫）
        Args:
            frame: 输入帧
            detected_bboxes: YOLO检测到的边界框列表
        Returns:
            unknown_objects: 未知对象列表
        """
        # 如果YOLO已经检测到病虫害，就不需要寻找未检出对象
        if detected_bboxes and len(detected_bboxes) > 0:
            return []
        
        # 创建检测掩码（标记已被YOLO检测的区域）
        mask = np.zeros(frame.shape[:2], dtype=np.uint8)
        for bbox in detected_bboxes:
            x1, y1, x2, y2 = bbox
            mask[y1:y2, x1:x2] = 255
        
        # 转换为灰度图进行轮廓检测
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        
        # 使用自适应阈值进行二值化
        binary = cv2.adaptiveThreshold(
            gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY_INV, 11, 2
        )
        
        # 形态学操作去噪
        kernel = np.ones((3, 3), np.uint8)
        binary = cv2.morphologyEx(binary, cv2.MORPH_OPEN, kernel, iterations=1)
        binary = cv2.morphologyEx(binary, cv2.MORPH_CLOSE, kernel, iterations=1)
        
        # 查找轮廓
        contours, _ = cv2.findContours(binary, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        
        unknown_objects = []
        for contour in contours:
            # 检查是否在已检测区域内
            x, y, w, h = cv2.boundingRect(contour)
            roi = mask[y:y+h, x:x+w]
            if np.any(roi > 0):  # 如果与已检测区域重叠，跳过
                continue
                
            # 形状特征检查
            if not self.is_insect_shape(contour):
                continue
                
            # 提取图像区域
            image_patch = frame[y:y+h, x:x+w]
            
            # 纹理特征检查
            if not self.has_insect_texture(image_patch):
                continue
                
            # 计算置信度分数
            confidence_score = self.calculate_confidence_score(contour, image_patch)
            
            if confidence_score >= self.confidence_threshold:
                unknown_objects.append({
                    'bbox': [x, y, x+w, y+h],
                    'contour': contour,
                    'image_patch': image_patch,
                    'confidence_score': confidence_score,
                    'timestamp': time.time(),
                    'area': cv2.contourArea(contour)
                })
        
        return unknown_objects
    
    def calculate_confidence_score(self, contour, image_patch):
        """
        计算未知对象的置信度分数
        """
        score = 0.0
        
        # 面积分数
        area = cv2.contourArea(contour)
        if 200 <= area <= 5000:
            score += 0.3
        elif 100 <= area <= 10000:
            score += 0.2
            
        # 形状分数
        perimeter = cv2.arcLength(contour, True)
        if perimeter > 0:
            circularity = 4 * np.pi * area / (perimeter * perimeter)
            if 0.4 <= circularity <= 0.8:
                score += 0.3
                
        # 长宽比分数
        x, y, w, h = cv2.boundingRect(contour)
        aspect_ratio = w / h if h > 0 else 0
        if 0.5 <= aspect_ratio <= 2.5:
            score += 0.2
            
        # 纹理分数
        if self.has_insect_texture(image_patch):
            score += 0.2
            
        return score
    
    def save_unknown_insect(self, frame_to_save, unknown_object, session_id=None):
        """
        保存未知昆虫图片（保存完整的原始图片）
        Args:
            frame_to_save: 要保存的完整视频帧
            unknown_object: 检测到的未知对象信息
            session_id: 会话ID
        """
        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S_%f")[:-3]
        filename = f"unknown_insect_{timestamp}.jpg"
        filepath = os.path.join(self.output_dir, filename)
        
        # 保存完整的帧图片
        cv2.imwrite(filepath, frame_to_save)
        
        # 获取边界框信息（用于数据库记录，但不裁剪图片）
        bbox = unknown_object['bbox']
        confidence_score = unknown_object['confidence_score']
        area = unknown_object['area']
        
        # 如果有数据库服务，保存记录
        if self.db_service:
            self.db_service.add_unknown_insect({
                'timestamp': timestamp,
                'image_path': filename,
                'confidence_score': confidence_score,
                'bbox_x': bbox[0],
                'bbox_y': bbox[1],
                'bbox_width': bbox[2] - bbox[0],
                'bbox_height': bbox[3] - bbox[1],
                'area': area,
                'session_id': session_id
            })
        
        # 更新统计信息
        self.unknown_insects_count += 1
        if session_id:
            self.session_unknown_insects[session_id].append({
                'timestamp': timestamp,
                'image_path': filename,
                'bbox': bbox,
                'confidence_score': confidence_score
            })
        
        return filepath
    
    def process_frame(self, frame, detected_bboxes, session_id=None):
        """
        处理单帧，检测并保存未知昆虫
        """
        # 提取未知对象
        unknown_objects = self.extract_unknown_objects(frame, detected_bboxes)
        
        saved_insects = []
        if unknown_objects:
            # 选择置信度最高的未知对象
            best_unknown_object = max(unknown_objects, key=lambda x: x['confidence_score'])
            
            # 保存最佳未知对象（现在传递整个帧）
            saved_info = self.save_unknown_insect(frame, best_unknown_object, session_id)
            saved_insects.append(saved_info)

        return saved_insects
    
    def get_session_unknown_insects(self, session_id):
        """
        获取指定会话的未知昆虫列表
        """
        return self.session_unknown_insects.get(session_id, [])
    
    def get_statistics(self):
        """
        获取统计信息
        """
        return {
            'total_unknown_insects': self.unknown_insects_count,
            'sessions_with_unknown_insects': len(self.session_unknown_insects),
            'output_directory': self.output_dir
        }
    
    def clear_session_data(self, session_id):
        """
        清除指定会话的数据
        """
        if session_id in self.session_unknown_insects:
            del self.session_unknown_insects[session_id] 