#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_lightOn(false)
    , m_brightness(50)
    , m_currentTemp(22)
    , m_targetTemp(24)
    , m_networkConnected(true)
    , m_motionDetected(false)
{
    ui->setupUi(this);
    
    // 初始化系统运行时间
    m_systemUptime.start();
    
    // 设置UI和连接
    setupUI();
    setupTimers();
    setupConnections();
    initializeValues();
    createCameraImage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    // 设置窗口标题和大小
    setWindowTitle("智能家居控制面板");
    resize(1200, 800);
    
    // 初始化摄像头图像
    ui->cameraLabel->setPixmap(m_cameraImage);
    
    // 设置报警历史记录
    ui->alarmHistoryTextEdit->append("系统启动 - " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::setupTimers()
{
    // 创建定时器
    m_dateTimeTimer = new QTimer(this);
    m_uptimeTimer = new QTimer(this);
    m_networkTimer = new QTimer(this);
    m_temperatureTimer = new QTimer(this);
    m_cameraTimer = new QTimer(this);
    m_motionTimer = new QTimer(this);
    m_reminderTimer = new QTimer(this);
    
    // 设置定时器间隔
    m_dateTimeTimer->setInterval(1000);      // 1秒更新一次时间
    m_uptimeTimer->setInterval(1000);        // 1秒更新一次运行时间
    m_networkTimer->setInterval(5000);       // 5秒检查一次网络状态
    m_temperatureTimer->setInterval(3000);   // 3秒更新一次温度
    m_cameraTimer->setInterval(2000);        // 2秒更新一次摄像头画面
    m_motionTimer->setInterval(10000);       // 10秒检查一次移动检测
    m_reminderTimer->setInterval(60000);     // 1分钟检查一次提醒
    
    // 启动定时器
    m_dateTimeTimer->start();
    m_uptimeTimer->start();
    m_networkTimer->start();
    m_temperatureTimer->start();
    m_cameraTimer->start();
    m_motionTimer->start();
    m_reminderTimer->start();
}

void MainWindow::setupConnections()
{
    // 系统状态相关连接
    connect(m_dateTimeTimer, &QTimer::timeout, this, &MainWindow::updateDateTime);
    connect(m_uptimeTimer, &QTimer::timeout, this, &MainWindow::updateSystemUptime);
    connect(m_networkTimer, &QTimer::timeout, this, &MainWindow::updateNetworkStatus);
    
    // 灯光控制相关连接
    connect(ui->roomComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onRoomChanged);
    connect(ui->lightToggleButton, &QPushButton::clicked, this, &MainWindow::onLightToggle);
    connect(ui->brightnessSlider, &QSlider::valueChanged, this, &MainWindow::onBrightnessChanged);
    
    // 温度控制相关连接
    connect(ui->targetTempSlider, &QSlider::valueChanged, this, &MainWindow::onTemperatureChanged);
    connect(ui->acModeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onACModeChanged);
    connect(m_temperatureTimer, &QTimer::timeout, this, &MainWindow::updateCurrentTemperature);
    
    // 安全监控相关连接
    connect(m_cameraTimer, &QTimer::timeout, this, &MainWindow::updateSecurityCamera);
    connect(m_motionTimer, &QTimer::timeout, this, &MainWindow::checkMotionDetection);
    connect(ui->securityModeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onSecurityModeChanged);
    
    // 日程管理相关连接
    connect(ui->calendarWidget, &QCalendarWidget::selectionChanged, this, &MainWindow::onCalendarSelectionChanged);
    connect(ui->addEventButton, &QPushButton::clicked, this, &MainWindow::onAddEvent);
    connect(m_reminderTimer, &QTimer::timeout, this, &MainWindow::checkEventReminders);
}

void MainWindow::initializeValues()
{
    // 初始化当前房间
    m_currentRoom = ui->roomComboBox->currentText();
    
    // 初始化空调模式
    m_acMode = ui->acModeComboBox->currentText();
    
    // 初始化安全模式
    m_securityMode = ui->securityModeComboBox->currentText();
    
    // 初始化温度显示
    updateCurrentTemperature();
}

void MainWindow::createCameraImage()
{
    // 创建模拟摄像头图像
    m_cameraImage = QPixmap(200, 150);
    m_cameraImage.fill(Qt::black);
    
    QPainter painter(&m_cameraImage);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 12));
    painter.drawText(m_cameraImage.rect(), Qt::AlignCenter, "摄像头画面\n模拟图像");
    
    ui->cameraLabel->setPixmap(m_cameraImage);
}

// 系统状态相关槽函数实现
void MainWindow::updateDateTime()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    ui->dateTimeLabel->setText("日期时间: " + currentDateTime.toString("yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::updateSystemUptime()
{
    qint64 elapsed = m_systemUptime.elapsed();
    int hours = elapsed / 3600000;
    int minutes = (elapsed % 3600000) / 60000;
    int seconds = (elapsed % 60000) / 1000;
    
    QString uptimeText = QString("运行时间: %1:%2:%3")
                        .arg(hours, 2, 10, QChar('0'))
                        .arg(minutes, 2, 10, QChar('0'))
                        .arg(seconds, 2, 10, QChar('0'));
    
    ui->uptimeLabel->setText(uptimeText);
}

void MainWindow::updateNetworkStatus()
{
    // 模拟网络状态变化
    static bool networkToggle = true;
    networkToggle = !networkToggle;
    m_networkConnected = networkToggle;
    
    if (m_networkConnected) {
        ui->networkStatusLabel->setText("网络状态: 已连接");
        ui->networkStatusLabel->setStyleSheet("color: green;");
    } else {
        ui->networkStatusLabel->setText("网络状态: 断开");
        ui->networkStatusLabel->setStyleSheet("color: red;");
    }
}

// 灯光控制相关槽函数实现
void MainWindow::onRoomChanged()
{
    m_currentRoom = ui->roomComboBox->currentText();
    ui->lightToggleButton->setText(m_lightOn ? "关灯" : "开灯");
}

void MainWindow::onLightToggle()
{
    m_lightOn = !m_lightOn;
    ui->lightToggleButton->setText(m_lightOn ? "关灯" : "开灯");
    
    // 更新亮度滑块和进度条状态
    ui->brightnessSlider->setEnabled(m_lightOn);
    ui->brightnessProgressBar->setEnabled(m_lightOn);
    
    if (!m_lightOn) {
        ui->brightnessSlider->setValue(0);
        ui->brightnessProgressBar->setValue(0);
        ui->brightnessLabel->setText("亮度: 0%");
    } else {
        ui->brightnessSlider->setValue(m_brightness);
        ui->brightnessProgressBar->setValue(m_brightness);
        ui->brightnessLabel->setText(QString("亮度: %1%").arg(m_brightness));
    }
}

void MainWindow::onBrightnessChanged(int value)
{
    m_brightness = value;
    ui->brightnessLabel->setText(QString("亮度: %1%").arg(value));
    ui->brightnessProgressBar->setValue(value);
}

// 温度控制相关槽函数实现
void MainWindow::onTemperatureChanged(int value)
{
    m_targetTemp = value;
    ui->targetTempLabel->setText(QString("目标温度: %1°C").arg(value));
}

void MainWindow::onACModeChanged()
{
    m_acMode = ui->acModeComboBox->currentText();
}

void MainWindow::updateCurrentTemperature()
{
    // 模拟温度变化，向目标温度靠近
    if (m_currentTemp < m_targetTemp) {
        m_currentTemp++;
    } else if (m_currentTemp > m_targetTemp) {
        m_currentTemp--;
    }
    
    ui->currentTempLabel->setText(QString("当前温度: %1°C").arg(m_currentTemp));
}

// 安全监控相关槽函数实现
void MainWindow::updateSecurityCamera()
{
    // 创建动态摄像头图像
    m_cameraImage = QPixmap(200, 150);
    m_cameraImage.fill(Qt::black);
    
    QPainter painter(&m_cameraImage);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 10));
    
    // 添加一些随机像素点模拟摄像头画面
    static bool seeded = false;
    if (!seeded) {
        srand(time(nullptr));
        seeded = true;
    }
    for (int i = 0; i < 50; ++i) {
        int x = rand() % 200;
        int y = rand() % 150;
        int gray = rand() % 256;
        painter.setPen(QColor(gray, gray, gray));
        painter.drawPoint(x, y);
    }
    
    painter.setPen(Qt::white);
    painter.drawText(m_cameraImage.rect(), Qt::AlignCenter, "摄像头画面\n" + QTime::currentTime().toString("hh:mm:ss"));
    
    ui->cameraLabel->setPixmap(m_cameraImage);
}

void MainWindow::checkMotionDetection()
{
    // 随机模拟移动检测
    bool previousMotion = m_motionDetected;
    m_motionDetected = (rand() % 100 < 5); // 5%概率检测到移动
    
    if (m_motionDetected && !previousMotion) {
        ui->motionIndicatorLabel->setText("移动检测: 检测到移动!");
        ui->motionIndicatorLabel->setStyleSheet("color: red; font-weight: bold;");
        addAlarmRecord();
    } else if (!m_motionDetected) {
        ui->motionIndicatorLabel->setText("移动检测: 正常");
        ui->motionIndicatorLabel->setStyleSheet("color: green;");
    }
}

void MainWindow::onSecurityModeChanged()
{
    m_securityMode = ui->securityModeComboBox->currentText();
    QString message = QString("安全模式已切换到: %1").arg(m_securityMode);
    ui->alarmHistoryTextEdit->append(QDateTime::currentDateTime().toString("hh:mm:ss") + " - " + message);
}

void MainWindow::addAlarmRecord()
{
    QString alarmRecord = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + 
                         " - 检测到移动 - " + m_securityMode;
    ui->alarmHistoryTextEdit->append(alarmRecord);
    m_alarmHistory.append(alarmRecord);
    
    // 限制历史记录数量
    if (m_alarmHistory.size() > 10) {
        m_alarmHistory.removeFirst();
    }
}

// 日程管理相关槽函数实现
void MainWindow::onCalendarSelectionChanged()
{
    QDate selectedDate = ui->calendarWidget->selectedDate();
    ui->eventListWidget->clear();
    
    // 显示选中日期的事件
    for (const QString &event : m_eventList) {
        if (event.contains(selectedDate.toString("yyyy-MM-dd"))) {
            ui->eventListWidget->addItem(event);
        }
    }
}

void MainWindow::onAddEvent()
{
    QString eventText = ui->eventLineEdit->text().trimmed();
    QTime eventTime = ui->eventTimeEdit->time();
    QDate selectedDate = ui->calendarWidget->selectedDate();
    
    if (eventText.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入事件内容!");
        return;
    }
    
    QString eventString = QString("%1 %2 - %3")
                         .arg(selectedDate.toString("yyyy-MM-dd"))
                         .arg(eventTime.toString("hh:mm"))
                         .arg(eventText);
    
    m_eventList.append(eventString);
    ui->eventListWidget->addItem(eventString);
    ui->eventLineEdit->clear();
    
    QMessageBox::information(this, "成功", "事件已添加!");
}

void MainWindow::checkEventReminders()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QTime currentTime = currentDateTime.time();
    QDate currentDate = currentDateTime.date();
    
    for (const QString &event : m_eventList) {
        if (event.contains(currentDate.toString("yyyy-MM-dd"))) {
            // 提取时间部分
            QStringList parts = event.split(" ");
            if (parts.size() >= 2) {
                QTime eventTime = QTime::fromString(parts[1], "hh:mm");
                if (eventTime.isValid() && 
                    qAbs(currentTime.secsTo(eventTime)) < 60) { // 1分钟内提醒
                    QMessageBox::information(this, "事件提醒", 
                        QString("提醒: %1").arg(event));
                }
            }
        }
    }
}
