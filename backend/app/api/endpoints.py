# backend/app/api/endpoints.py
from flask import Blueprint, request, jsonify
from werkzeug.utils import secure_filename
import os
import requests
import json
from ..core.detector import CropDetector

api = Blueprint('api', __name__)
detector = CropDetector(model_path="../../models/best.pt")


@api.route('/detect', methods=['POST'])
def detect():
    if 'file' not in request.files:
        return jsonify({"error": "No file uploaded"}), 400

    file = request.files['file']
    filename = secure_filename(file.filename)
    temp_path = os.path.join("tmp_uploads", filename)
    os.makedirs(os.path.dirname(temp_path), exist_ok=True)
    file.save(temp_path)

    try:
        result = detector.predict(temp_path)
        return jsonify({
            "status": "success",
            "data": result
        })
    finally:
        if os.path.exists(temp_path):
            os.remove(temp_path)


@api.route('/terrain/analysis', methods=['POST'])
def terrain_analysis():
    """
    地势分析API
    接收地块边界坐标，返回地势起伏特征
    """
    try:
        data = request.get_json()
        
        if not data or 'bounds' not in data:
            return jsonify({"error": "缺少边界数据"}), 400
            
        bounds = data['bounds']
        api_key = data.get('apiKey', 'b9824a931dff18b4dbd6386eaec5ecb1')
        
        # 从边界计算中心点
        center_lng = (bounds['southwest'][0] + bounds['northeast'][0]) / 2
        center_lat = (bounds['southwest'][1] + bounds['northeast'][1]) / 2
        
        # 计算边界范围
        lng_range = bounds['northeast'][0] - bounds['southwest'][0]
        lat_range = bounds['northeast'][1] - bounds['southwest'][1]
        
        # 生成采样点网格（5x5网格）
        sample_points = []
        for i in range(5):
            for j in range(5):
                lng = bounds['southwest'][0] + (lng_range * i / 4)
                lat = bounds['southwest'][1] + (lat_range * j / 4)
                sample_points.append([lng, lat])
        
        # 调用高德地图高程API获取地势数据
        elevations = []
        for point in sample_points:
            elevation = get_elevation_from_gaode(point[0], point[1], api_key)
            if elevation is not None:
                elevations.append(elevation)
        
        if not elevations:
            # 如果无法获取真实数据，返回模拟数据
            terrain_data = generate_mock_terrain_data(bounds)
        else:
            terrain_data = analyze_terrain_data(elevations)
        
        return jsonify({
            "status": "success",
            "data": terrain_data
        })
        
    except Exception as e:
        return jsonify({"error": f"地势分析失败: {str(e)}"}), 500


def get_elevation_from_gaode(lng, lat, api_key):
    """
    从高德地图API获取高程数据
    """
    try:
        url = f"https://restapi.amap.com/v3/elevation"
        params = {
            'key': api_key,
            'locations': f"{lng},{lat}",
            'output': 'json'
        }
        
        response = requests.get(url, params=params, timeout=5)
        if response.status_code == 200:
            data = response.json()
            if data.get('status') == '1' and data.get('locations'):
                return float(data['locations'][0].get('elevation', 0))
        
        return None
    except Exception as e:
        print(f"获取高程数据失败: {e}")
        return None


def analyze_terrain_data(elevations):
    """
    分析地势数据，计算地形特征
    """
    if not elevations:
        return generate_mock_terrain_data()
    
    max_elevation = max(elevations)
    min_elevation = min(elevations)
    avg_elevation = sum(elevations) / len(elevations)
    
    # 计算高程变化（简单坡度估算）
    elevation_range = max_elevation - min_elevation
    
    # 根据高程变化判断地形复杂度
    if elevation_range < 10:
        complexity = 'low'
        average_slope = 2.5
    elif elevation_range < 50:
        complexity = 'medium'
        average_slope = 8.5
    else:
        complexity = 'high'
        average_slope = 15.2
    
    return {
        "maxElevation": round(max_elevation, 1),
        "minElevation": round(min_elevation, 1),
        "averageElevation": round(avg_elevation, 1),
        "elevationRange": round(elevation_range, 1),
        "averageSlope": round(average_slope, 1),
        "complexity": complexity,
        "samplePoints": len(elevations)
    }


def generate_mock_terrain_data(bounds=None):
    """
    生成模拟地势数据（当无法获取真实数据时使用）
    """
    import random
    
    # 生成模拟高程数据
    base_elevation = random.uniform(50, 200)
    elevation_variation = random.uniform(5, 30)
    
    max_elevation = base_elevation + elevation_variation
    min_elevation = base_elevation - elevation_variation
    
    # 根据变化幅度判断复杂度
    if elevation_variation < 10:
        complexity = 'low'
        average_slope = random.uniform(2, 5)
    elif elevation_variation < 20:
        complexity = 'medium'
        average_slope = random.uniform(5, 12)
    else:
        complexity = 'high'
        average_slope = random.uniform(12, 20)
    
    return {
        "maxElevation": round(max_elevation, 1),
        "minElevation": round(min_elevation, 1),
        "averageElevation": round(base_elevation, 1),
        "elevationRange": round(elevation_variation * 2, 1),
        "averageSlope": round(average_slope, 1),
        "complexity": complexity,
        "samplePoints": 25,
        "note": "使用模拟数据"
    }
