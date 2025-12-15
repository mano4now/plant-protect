import os

from flask import Flask, Response, jsonify, request, render_template_string
from flask_cors import CORS
import cv2
import time
import threading
from backend.app.core.detector import YOLOv8Detector

app = Flask(__name__)
CORS(app)  # 允许跨域请求
  # 安装: pip install flask-cors



# 或更精细的配置（推荐）：
CORS(app, resources={
    r"/switch_model": {"origins": ["http://localhost:8001"]}
})
# 配置参数
RTMP_URL = "rtmp://192.168.43.131:1935/live" # "rtmp://192.168.97.131/live"
USE_RTMP = False  # 设为False则使用本地摄像头

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
class VideoStream:
    def __init__(self, detector):
        self.source = RTMP_URL if USE_RTMP else 0
        self.capture = None
        self.running = False
        self.last_time = time.time()
        self.fps = 0
        self.lock = threading.Lock()
        self.retry_count = 0
        self.detector = detector
        self.current_frame = None
        self.current_detections = []
        # 使用您指定的模型配置


        self.model_configs = {
            "model1": {
                "model_path": os.path.join(BASE_DIR, "models", "best.pt"),
                "classes_path": os.path.join(BASE_DIR, "models", "classic.txt")
            },
            "model2": {
                "model_path": os.path.join(BASE_DIR, "models", "bug_best.pt"),
                "classes_path": os.path.join(BASE_DIR, "models", "bug_classic.txt")
            }
        }
        self.active_model = None

        print(f"尝试连接视频源: {self.source}")
        self.connect()

    def connect(self):
        try:
            self.capture = cv2.VideoCapture(self.source)
            self.capture.set(cv2.CAP_PROP_OPEN_TIMEOUT_MSEC, 10000)

            if not self.capture.isOpened():
                self.capture = cv2.VideoCapture(self.source, cv2.CAP_FFMPEG)
                if not self.capture.isOpened():
                    raise RuntimeError("无法打开视频源")
        except Exception as e:
            print(f"连接异常: {str(e)}")
            raise RuntimeError(f"无法连接视频源: {str(e)}")

    def __del__(self):
        self.stop()

    def stop(self):
        self.running = False
        if self.capture and self.capture.isOpened():
            self.capture.release()
            print("视频源已释放")

    def start(self):
        """开始视频流和检测"""
        self.running = True
        threading.Thread(target=self._update_frame, daemon=True).start()

    def _update_frame(self):
        """更新帧并进行检测"""
        while self.running:
            ret, frame = self.capture.read()
            if not ret:
                print("获取帧失败，尝试重新连接...")
                self.retry_count += 1
                if self.retry_count > 5:
                    self.running = False
                    break
                time.sleep(1)
                self.connect()
                continue

            self.retry_count = 0

            # 计算FPS
            current_time = time.time()
            self.fps = 1.0 / (current_time - self.last_time)
            self.last_time = current_time

            # 执行目标检测
            detected_frame, detections = self.detector.detect(frame)

            # 显示FPS
            cv2.putText(detected_frame, f"FPS: {self.fps:.2f}", (10, 30),
                        cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

            # 更新当前帧和检测结果
            with self.lock:
                self.current_frame = detected_frame
                self.current_detections = detections

    def get_frame(self):
        """获取当前帧"""
        with self.lock:
            if self.current_frame is None:
                return None
            ret, jpeg = cv2.imencode('.jpg', self.current_frame)
            return jpeg.tobytes()

    def generate_http_frames(self):
        """生成HTTP视频流的帧"""
        self.start()
        while self.running:
            frame = self.get_frame()
            if frame is not None:
                yield (b'--frame\r\n'
                       b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')
            else:
                time.sleep(0.1)

    def switch_model(self, model_name):
        """切换模型"""
        if model_name in self.model_configs:
            config = self.model_configs[model_name]
            success = self.detector.load_model(config["model_path"], config["classes_path"])
            if success:
                self.active_model = model_name
                print(f"已切换到模型: {model_name}")
                return True
        print(f"切换模型失败: {model_name}")
        return False


# 初始化检测器和视频流
detector = YOLOv8Detector()
video_stream = VideoStream(detector)


# API路由
@app.route('/video_feed')
def video_feed():
    """返回HTTP视频流"""
    return Response(
        video_stream.generate_http_frames(),
        mimetype='multipart/x-mixed-replace; boundary=frame'
    )


@app.route('/switch_model', methods=['POST'])
def switch_model():
    """切换模型接口"""
    data = request.get_json()
    model_name = data.get('model_name')

    if video_stream.switch_model(model_name):
        return jsonify({
            "status": "success",
            "model_name": model_name,
            "message": f"已切换到模型: {model_name}"
        })
    return jsonify({
        "status": "error",
        "message": "模型切换失败"
    }), 400
@app.route('/')
def index():
    return render_template_string('''
<!DOCTYPE html>
<html>
<head>
    <title>实时视频流与模型切换</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 20px; }
        .container { display: flex; flex-direction: column; align-items: center; }
        #videoFeed { border: 2px solid #333; margin-bottom: 20px; }
        button { padding: 10px 15px; background: #4CAF50; color: white; border: none; cursor: pointer; }
        button:hover { background: #45a049; }
    </style>
</head>
<body>
    <div class="container">
        <h1>实时目标检测演示</h1>
        <img id="videoFeed" src="{{ url_for('video_feed') }}" width="640" height="480">
    </div>
</body>
</html>
    ''')


if __name__ == '__main__':
    try:
        app.run(host='0.0.0.0', port=5000, debug=True)
    except KeyboardInterrupt:
        video_stream.stop()