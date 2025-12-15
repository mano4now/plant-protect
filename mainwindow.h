#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <QDateTime>
#include <QComboBox>
#include <QPushButton>
#include <QSlider>
#include <QProgressBar>
#include <QLabel>
#include <QCalendarWidget>
#include <QListWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTextEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QTimeEdit>
#include <QLineEdit>
#include <QMessageBox>
#include <QPixmap>
#include <QPainter>
#include <QColor>
#include <QTime>
#include <cstdlib>
#include <ctime>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 系统状态相关槽函数
    void updateDateTime();
    void updateSystemUptime();
    void updateNetworkStatus();
    
    // 灯光控制相关槽函数
    void onRoomChanged();
    void onLightToggle();
    void onBrightnessChanged(int value);
    
    // 温度控制相关槽函数
    void onTemperatureChanged(int value);
    void onACModeChanged();
    void updateCurrentTemperature();
    
    // 安全监控相关槽函数
    void updateSecurityCamera();
    void checkMotionDetection();
    void onSecurityModeChanged();
    void addAlarmRecord();
    
    // 日程管理相关槽函数
    void onCalendarSelectionChanged();
    void onAddEvent();
    void checkEventReminders();

private:
    Ui::MainWindow *ui;
    
    // 定时器
    QTimer *m_dateTimeTimer;
    QTimer *m_uptimeTimer;
    QTimer *m_networkTimer;
    QTimer *m_temperatureTimer;
    QTimer *m_cameraTimer;
    QTimer *m_motionTimer;
    QTimer *m_reminderTimer;
    
    // 系统运行时间
    QElapsedTimer m_systemUptime;
    
    // 当前状态变量
    QString m_currentRoom;
    bool m_lightOn;
    int m_brightness;
    int m_currentTemp;
    int m_targetTemp;
    QString m_acMode;
    bool m_networkConnected;
    bool m_motionDetected;
    QString m_securityMode;
    
    // 模拟数据
    QPixmap m_cameraImage;
    QList<QString> m_alarmHistory;
    QList<QString> m_eventList;
    
    // 初始化函数
    void setupUI();
    void setupTimers();
    void setupConnections();
    void initializeValues();
    void createCameraImage();
};
#endif // MAINWINDOW_H
