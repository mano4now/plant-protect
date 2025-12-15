/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.15
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QGroupBox *systemStatusGroup;
    QGridLayout *systemStatusLayout;
    QLabel *dateTimeLabel;
    QLabel *uptimeLabel;
    QLabel *networkStatusLabel;
    QGroupBox *lightControlGroup;
    QVBoxLayout *lightControlLayout;
    QComboBox *roomComboBox;
    QPushButton *lightToggleButton;
    QLabel *brightnessLabel;
    QSlider *brightnessSlider;
    QProgressBar *brightnessProgressBar;
    QGroupBox *temperatureControlGroup;
    QVBoxLayout *temperatureControlLayout;
    QLabel *currentTempLabel;
    QLabel *targetTempLabel;
    QSlider *targetTempSlider;
    QComboBox *acModeComboBox;
    QGroupBox *securityMonitorGroup;
    QVBoxLayout *securityMonitorLayout;
    QLabel *cameraLabel;
    QLabel *motionIndicatorLabel;
    QComboBox *securityModeComboBox;
    QTextEdit *alarmHistoryTextEdit;
    QGroupBox *scheduleManagementGroup;
    QHBoxLayout *scheduleManagementLayout;
    QCalendarWidget *calendarWidget;
    QVBoxLayout *scheduleRightLayout;
    QLabel *eventListLabel;
    QListWidget *eventListWidget;
    QHBoxLayout *eventInputLayout;
    QLineEdit *eventLineEdit;
    QTimeEdit *eventTimeEdit;
    QPushButton *addEventButton;
    QSpacerItem *verticalSpacer;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1200, 800);
        MainWindow->setStyleSheet(QString::fromUtf8("QMainWindow {\n"
"    background-color: #f0f0f0;\n"
"}\n"
"QGroupBox {\n"
"    font-weight: bold;\n"
"    border: 2px solid #cccccc;\n"
"    border-radius: 5px;\n"
"    margin-top: 1ex;\n"
"    padding-top: 10px;\n"
"}\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 5px 0 5px;\n"
"}\n"
"QPushButton {\n"
"    background-color: #4CAF50;\n"
"    border: none;\n"
"    color: white;\n"
"    padding: 8px 16px;\n"
"    text-align: center;\n"
"    font-size: 14px;\n"
"    border-radius: 4px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #45a049;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #3d8b40;\n"
"}\n"
"QSlider::groove:horizontal {\n"
"    border: 1px solid #999999;\n"
"    height: 8px;\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);\n"
"    margin: 2px 0;\n"
"    border-radius: 4px;\n"
"}\n"
"QSlider::handle:horizontal {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, s"
                        "top:1 #8f8f8f);\n"
"    border: 1px solid #5c5c5c;\n"
"    width: 18px;\n"
"    margin: -2px 0;\n"
"    border-radius: 9px;\n"
"}\n"
"QProgressBar {\n"
"    border: 2px solid grey;\n"
"    border-radius: 5px;\n"
"    text-align: center;\n"
"}\n"
"QProgressBar::chunk {\n"
"    background-color: #4CAF50;\n"
"    border-radius: 3px;\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        systemStatusGroup = new QGroupBox(centralwidget);
        systemStatusGroup->setObjectName(QString::fromUtf8("systemStatusGroup"));
        systemStatusLayout = new QGridLayout(systemStatusGroup);
        systemStatusLayout->setObjectName(QString::fromUtf8("systemStatusLayout"));
        dateTimeLabel = new QLabel(systemStatusGroup);
        dateTimeLabel->setObjectName(QString::fromUtf8("dateTimeLabel"));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(QFont::Bold);
        dateTimeLabel->setFont(font);

        systemStatusLayout->addWidget(dateTimeLabel, 0, 0, 1, 1);

        uptimeLabel = new QLabel(systemStatusGroup);
        uptimeLabel->setObjectName(QString::fromUtf8("uptimeLabel"));
        uptimeLabel->setFont(font);

        systemStatusLayout->addWidget(uptimeLabel, 0, 1, 1, 1);

        networkStatusLabel = new QLabel(systemStatusGroup);
        networkStatusLabel->setObjectName(QString::fromUtf8("networkStatusLabel"));
        networkStatusLabel->setFont(font);
        networkStatusLabel->setStyleSheet(QString::fromUtf8("color: green;"));

        systemStatusLayout->addWidget(networkStatusLabel, 1, 0, 1, 2);


        gridLayout->addWidget(systemStatusGroup, 0, 0, 1, 1);

        lightControlGroup = new QGroupBox(centralwidget);
        lightControlGroup->setObjectName(QString::fromUtf8("lightControlGroup"));
        lightControlLayout = new QVBoxLayout(lightControlGroup);
        lightControlLayout->setObjectName(QString::fromUtf8("lightControlLayout"));
        roomComboBox = new QComboBox(lightControlGroup);
        roomComboBox->addItem(QString());
        roomComboBox->addItem(QString());
        roomComboBox->addItem(QString());
        roomComboBox->addItem(QString());
        roomComboBox->setObjectName(QString::fromUtf8("roomComboBox"));

        lightControlLayout->addWidget(roomComboBox);

        lightToggleButton = new QPushButton(lightControlGroup);
        lightToggleButton->setObjectName(QString::fromUtf8("lightToggleButton"));

        lightControlLayout->addWidget(lightToggleButton);

        brightnessLabel = new QLabel(lightControlGroup);
        brightnessLabel->setObjectName(QString::fromUtf8("brightnessLabel"));

        lightControlLayout->addWidget(brightnessLabel);

        brightnessSlider = new QSlider(lightControlGroup);
        brightnessSlider->setObjectName(QString::fromUtf8("brightnessSlider"));
        brightnessSlider->setMinimum(0);
        brightnessSlider->setMaximum(100);
        brightnessSlider->setValue(50);
        brightnessSlider->setOrientation(Qt::Horizontal);

        lightControlLayout->addWidget(brightnessSlider);

        brightnessProgressBar = new QProgressBar(lightControlGroup);
        brightnessProgressBar->setObjectName(QString::fromUtf8("brightnessProgressBar"));
        brightnessProgressBar->setMinimum(0);
        brightnessProgressBar->setMaximum(100);
        brightnessProgressBar->setValue(50);

        lightControlLayout->addWidget(brightnessProgressBar);


        gridLayout->addWidget(lightControlGroup, 0, 1, 1, 1);

        temperatureControlGroup = new QGroupBox(centralwidget);
        temperatureControlGroup->setObjectName(QString::fromUtf8("temperatureControlGroup"));
        temperatureControlLayout = new QVBoxLayout(temperatureControlGroup);
        temperatureControlLayout->setObjectName(QString::fromUtf8("temperatureControlLayout"));
        currentTempLabel = new QLabel(temperatureControlGroup);
        currentTempLabel->setObjectName(QString::fromUtf8("currentTempLabel"));
        QFont font1;
        font1.setPointSize(14);
        font1.setBold(true);
        font1.setWeight(QFont::Bold);
        currentTempLabel->setFont(font1);

        temperatureControlLayout->addWidget(currentTempLabel);

        targetTempLabel = new QLabel(temperatureControlGroup);
        targetTempLabel->setObjectName(QString::fromUtf8("targetTempLabel"));

        temperatureControlLayout->addWidget(targetTempLabel);

        targetTempSlider = new QSlider(temperatureControlGroup);
        targetTempSlider->setObjectName(QString::fromUtf8("targetTempSlider"));
        targetTempSlider->setMinimum(16);
        targetTempSlider->setMaximum(30);
        targetTempSlider->setValue(24);
        targetTempSlider->setOrientation(Qt::Horizontal);

        temperatureControlLayout->addWidget(targetTempSlider);

        acModeComboBox = new QComboBox(temperatureControlGroup);
        acModeComboBox->addItem(QString());
        acModeComboBox->addItem(QString());
        acModeComboBox->addItem(QString());
        acModeComboBox->addItem(QString());
        acModeComboBox->setObjectName(QString::fromUtf8("acModeComboBox"));

        temperatureControlLayout->addWidget(acModeComboBox);


        gridLayout->addWidget(temperatureControlGroup, 0, 2, 1, 1);

        securityMonitorGroup = new QGroupBox(centralwidget);
        securityMonitorGroup->setObjectName(QString::fromUtf8("securityMonitorGroup"));
        securityMonitorLayout = new QVBoxLayout(securityMonitorGroup);
        securityMonitorLayout->setObjectName(QString::fromUtf8("securityMonitorLayout"));
        cameraLabel = new QLabel(securityMonitorGroup);
        cameraLabel->setObjectName(QString::fromUtf8("cameraLabel"));
        cameraLabel->setMinimumSize(QSize(200, 150));
        cameraLabel->setMaximumSize(QSize(200, 150));
        cameraLabel->setAlignment(Qt::AlignCenter);
        cameraLabel->setStyleSheet(QString::fromUtf8("border: 2px solid #cccccc; background-color: #000000; color: white;"));

        securityMonitorLayout->addWidget(cameraLabel);

        motionIndicatorLabel = new QLabel(securityMonitorGroup);
        motionIndicatorLabel->setObjectName(QString::fromUtf8("motionIndicatorLabel"));
        QFont font2;
        font2.setBold(true);
        font2.setWeight(QFont::Bold);
        motionIndicatorLabel->setFont(font2);
        motionIndicatorLabel->setStyleSheet(QString::fromUtf8("color: green;"));

        securityMonitorLayout->addWidget(motionIndicatorLabel);

        securityModeComboBox = new QComboBox(securityMonitorGroup);
        securityModeComboBox->addItem(QString());
        securityModeComboBox->addItem(QString());
        securityModeComboBox->addItem(QString());
        securityModeComboBox->setObjectName(QString::fromUtf8("securityModeComboBox"));

        securityMonitorLayout->addWidget(securityModeComboBox);

        alarmHistoryTextEdit = new QTextEdit(securityMonitorGroup);
        alarmHistoryTextEdit->setObjectName(QString::fromUtf8("alarmHistoryTextEdit"));
        alarmHistoryTextEdit->setMaximumSize(QSize(16777215, 100));
        alarmHistoryTextEdit->setReadOnly(true);

        securityMonitorLayout->addWidget(alarmHistoryTextEdit);


        gridLayout->addWidget(securityMonitorGroup, 1, 0, 1, 1);

        scheduleManagementGroup = new QGroupBox(centralwidget);
        scheduleManagementGroup->setObjectName(QString::fromUtf8("scheduleManagementGroup"));
        scheduleManagementLayout = new QHBoxLayout(scheduleManagementGroup);
        scheduleManagementLayout->setObjectName(QString::fromUtf8("scheduleManagementLayout"));
        calendarWidget = new QCalendarWidget(scheduleManagementGroup);
        calendarWidget->setObjectName(QString::fromUtf8("calendarWidget"));

        scheduleManagementLayout->addWidget(calendarWidget);

        scheduleRightLayout = new QVBoxLayout();
        scheduleRightLayout->setObjectName(QString::fromUtf8("scheduleRightLayout"));
        eventListLabel = new QLabel(scheduleManagementGroup);
        eventListLabel->setObjectName(QString::fromUtf8("eventListLabel"));
        eventListLabel->setFont(font2);

        scheduleRightLayout->addWidget(eventListLabel);

        eventListWidget = new QListWidget(scheduleManagementGroup);
        eventListWidget->setObjectName(QString::fromUtf8("eventListWidget"));
        eventListWidget->setMaximumSize(QSize(16777215, 200));

        scheduleRightLayout->addWidget(eventListWidget);

        eventInputLayout = new QHBoxLayout();
        eventInputLayout->setObjectName(QString::fromUtf8("eventInputLayout"));
        eventLineEdit = new QLineEdit(scheduleManagementGroup);
        eventLineEdit->setObjectName(QString::fromUtf8("eventLineEdit"));

        eventInputLayout->addWidget(eventLineEdit);

        eventTimeEdit = new QTimeEdit(scheduleManagementGroup);
        eventTimeEdit->setObjectName(QString::fromUtf8("eventTimeEdit"));
        eventTimeEdit->setTime(QTime(12, 0, 0));

        eventInputLayout->addWidget(eventTimeEdit);

        addEventButton = new QPushButton(scheduleManagementGroup);
        addEventButton->setObjectName(QString::fromUtf8("addEventButton"));

        eventInputLayout->addWidget(addEventButton);


        scheduleRightLayout->addLayout(eventInputLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        scheduleRightLayout->addItem(verticalSpacer);


        scheduleManagementLayout->addLayout(scheduleRightLayout);


        gridLayout->addWidget(scheduleManagementGroup, 1, 1, 1, 2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1200, 18));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\231\272\350\203\275\345\256\266\345\261\205\346\216\247\345\210\266\351\235\242\346\235\277", nullptr));
        systemStatusGroup->setTitle(QCoreApplication::translate("MainWindow", "\347\263\273\347\273\237\347\212\266\346\200\201", nullptr));
        dateTimeLabel->setText(QCoreApplication::translate("MainWindow", "\346\227\245\346\234\237\346\227\266\351\227\264: 2024-01-01 12:00:00", nullptr));
        uptimeLabel->setText(QCoreApplication::translate("MainWindow", "\350\277\220\350\241\214\346\227\266\351\227\264: 00:00:00", nullptr));
        networkStatusLabel->setText(QCoreApplication::translate("MainWindow", "\347\275\221\347\273\234\347\212\266\346\200\201: \345\267\262\350\277\236\346\216\245", nullptr));
        lightControlGroup->setTitle(QCoreApplication::translate("MainWindow", "\347\201\257\345\205\211\346\216\247\345\210\266", nullptr));
        roomComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "\345\256\242\345\216\205", nullptr));
        roomComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "\345\215\247\345\256\244", nullptr));
        roomComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "\345\216\250\346\210\277", nullptr));
        roomComboBox->setItemText(3, QCoreApplication::translate("MainWindow", "\344\271\246\346\210\277", nullptr));

        lightToggleButton->setText(QCoreApplication::translate("MainWindow", "\345\274\200\347\201\257", nullptr));
        brightnessLabel->setText(QCoreApplication::translate("MainWindow", "\344\272\256\345\272\246: 50%", nullptr));
        temperatureControlGroup->setTitle(QCoreApplication::translate("MainWindow", "\346\270\251\345\272\246\346\216\247\345\210\266", nullptr));
        currentTempLabel->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215\346\270\251\345\272\246: 22\302\260C", nullptr));
        targetTempLabel->setText(QCoreApplication::translate("MainWindow", "\347\233\256\346\240\207\346\270\251\345\272\246: 24\302\260C", nullptr));
        acModeComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "\350\207\252\345\212\250", nullptr));
        acModeComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "\345\210\266\345\206\267", nullptr));
        acModeComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "\345\210\266\347\203\255", nullptr));
        acModeComboBox->setItemText(3, QCoreApplication::translate("MainWindow", "\351\200\232\351\243\216", nullptr));

        securityMonitorGroup->setTitle(QCoreApplication::translate("MainWindow", "\345\256\211\345\205\250\347\233\221\346\216\247", nullptr));
        cameraLabel->setText(QCoreApplication::translate("MainWindow", "\346\221\204\345\203\217\345\244\264\347\224\273\351\235\242", nullptr));
        motionIndicatorLabel->setText(QCoreApplication::translate("MainWindow", "\347\247\273\345\212\250\346\243\200\346\265\213: \346\255\243\345\270\270", nullptr));
        securityModeComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "\345\261\205\345\256\266\346\250\241\345\274\217", nullptr));
        securityModeComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "\345\244\226\345\207\272\346\250\241\345\274\217", nullptr));
        securityModeComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "\347\235\241\347\234\240\346\250\241\345\274\217", nullptr));

        scheduleManagementGroup->setTitle(QCoreApplication::translate("MainWindow", "\346\227\245\347\250\213\347\256\241\347\220\206", nullptr));
        eventListLabel->setText(QCoreApplication::translate("MainWindow", "\346\227\245\347\250\213\344\272\213\344\273\266:", nullptr));
        eventLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\276\223\345\205\245\344\272\213\344\273\266\345\206\205\345\256\271...", nullptr));
        addEventButton->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\344\272\213\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
