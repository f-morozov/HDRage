/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri 19. Apr 13:38:14 2013
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDockWidget>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *actionAbout;
    QAction *actionOpen;
    QAction *actionMake;
    QAction *actionHelp;
    QAction *actionRestore;
    QWidget *centralWidget;
    QHBoxLayout *centralLayout;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuView;
    QDockWidget *settings_widget;
    QWidget *dockWidgetContents_4;
    QGroupBox *file_box;
    QPushButton *open_button;
    QPushButton *make_button;
    QPushButton *rad_button;
    QGroupBox *tonemap_box;
    QComboBox *algo_select;
    QDoubleSpinBox *param_value_2;
    QLabel *param_label_2;
    QDoubleSpinBox *param_value_1;
    QLabel *param_label_1;
    QPushButton *tonemap_button;
    QLabel *param_label_3;
    QDoubleSpinBox *param_value_3;
    QGroupBox *FPS_box;
    QLabel *last_fps_label;
    QLabel *mean_fps_label;
    QLCDNumber *last_fps_display;
    QLCDNumber *mean_fps_display;
    QPushButton *save_hdr_button;
    QPushButton *save_ldr_button;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
        MainWindowClass->resize(800, 636);
        MainWindowClass->setMinimumSize(QSize(800, 600));
        actionAbout = new QAction(MainWindowClass);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionOpen = new QAction(MainWindowClass);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionMake = new QAction(MainWindowClass);
        actionMake->setObjectName(QString::fromUtf8("actionMake"));
        actionHelp = new QAction(MainWindowClass);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionRestore = new QAction(MainWindowClass);
        actionRestore->setObjectName(QString::fromUtf8("actionRestore"));
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralLayout = new QHBoxLayout(centralWidget);
        centralLayout->setSpacing(6);
        centralLayout->setContentsMargins(11, 11, 11, 11);
        centralLayout->setObjectName(QString::fromUtf8("centralLayout"));
        centralLayout->setContentsMargins(9, -1, -1, -1);
        MainWindowClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindowClass->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 18));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        MainWindowClass->setMenuBar(menuBar);
        settings_widget = new QDockWidget(MainWindowClass);
        settings_widget->setObjectName(QString::fromUtf8("settings_widget"));
        settings_widget->setMinimumSize(QSize(160, 500));
        settings_widget->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QString::fromUtf8("dockWidgetContents_4"));
        file_box = new QGroupBox(dockWidgetContents_4);
        file_box->setObjectName(QString::fromUtf8("file_box"));
        file_box->setGeometry(QRect(0, 0, 151, 151));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        file_box->setFont(font);
        open_button = new QPushButton(file_box);
        open_button->setObjectName(QString::fromUtf8("open_button"));
        open_button->setGeometry(QRect(10, 30, 111, 31));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(false);
        font1.setWeight(50);
        open_button->setFont(font1);
        open_button->setAutoDefault(false);
        open_button->setDefault(false);
        open_button->setFlat(false);
        make_button = new QPushButton(file_box);
        make_button->setObjectName(QString::fromUtf8("make_button"));
        make_button->setGeometry(QRect(10, 110, 111, 31));
        make_button->setFont(font1);
        rad_button = new QPushButton(file_box);
        rad_button->setObjectName(QString::fromUtf8("rad_button"));
        rad_button->setGeometry(QRect(10, 70, 111, 31));
        rad_button->setFont(font1);
        tonemap_box = new QGroupBox(dockWidgetContents_4);
        tonemap_box->setObjectName(QString::fromUtf8("tonemap_box"));
        tonemap_box->setGeometry(QRect(0, 220, 151, 251));
        tonemap_box->setFont(font);
        algo_select = new QComboBox(tonemap_box);
        algo_select->setObjectName(QString::fromUtf8("algo_select"));
        algo_select->setGeometry(QRect(10, 20, 121, 21));
        algo_select->setFont(font1);
        param_value_2 = new QDoubleSpinBox(tonemap_box);
        param_value_2->setObjectName(QString::fromUtf8("param_value_2"));
        param_value_2->setGeometry(QRect(10, 120, 101, 21));
        param_value_2->setFont(font1);
        param_value_2->setMinimum(-1000);
        param_value_2->setMaximum(1000);
        param_value_2->setSingleStep(0.1);
        param_label_2 = new QLabel(tonemap_box);
        param_label_2->setObjectName(QString::fromUtf8("param_label_2"));
        param_label_2->setGeometry(QRect(10, 100, 101, 16));
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(true);
        font2.setWeight(75);
        param_label_2->setFont(font2);
        param_value_1 = new QDoubleSpinBox(tonemap_box);
        param_value_1->setObjectName(QString::fromUtf8("param_value_1"));
        param_value_1->setGeometry(QRect(10, 70, 101, 21));
        param_value_1->setFont(font1);
        param_value_1->setMinimum(-1000);
        param_value_1->setMaximum(1000);
        param_value_1->setSingleStep(0.1);
        param_label_1 = new QLabel(tonemap_box);
        param_label_1->setObjectName(QString::fromUtf8("param_label_1"));
        param_label_1->setGeometry(QRect(10, 50, 101, 21));
        param_label_1->setFont(font2);
        tonemap_button = new QPushButton(tonemap_box);
        tonemap_button->setObjectName(QString::fromUtf8("tonemap_button"));
        tonemap_button->setGeometry(QRect(10, 210, 101, 31));
        tonemap_button->setFont(font1);
        param_label_3 = new QLabel(tonemap_box);
        param_label_3->setObjectName(QString::fromUtf8("param_label_3"));
        param_label_3->setGeometry(QRect(10, 150, 101, 16));
        param_label_3->setFont(font2);
        param_value_3 = new QDoubleSpinBox(tonemap_box);
        param_value_3->setObjectName(QString::fromUtf8("param_value_3"));
        param_value_3->setGeometry(QRect(10, 170, 101, 21));
        param_value_3->setFont(font1);
        param_value_3->setMinimum(-1000);
        param_value_3->setMaximum(1000);
        param_value_3->setSingleStep(0.1);
        FPS_box = new QGroupBox(dockWidgetContents_4);
        FPS_box->setObjectName(QString::fromUtf8("FPS_box"));
        FPS_box->setGeometry(QRect(0, 480, 151, 91));
        FPS_box->setFont(font);
        last_fps_label = new QLabel(FPS_box);
        last_fps_label->setObjectName(QString::fromUtf8("last_fps_label"));
        last_fps_label->setGeometry(QRect(10, 20, 51, 16));
        QFont font3;
        font3.setPointSize(10);
        last_fps_label->setFont(font3);
        mean_fps_label = new QLabel(FPS_box);
        mean_fps_label->setObjectName(QString::fromUtf8("mean_fps_label"));
        mean_fps_label->setGeometry(QRect(70, 20, 61, 16));
        mean_fps_label->setFont(font3);
        last_fps_display = new QLCDNumber(FPS_box);
        last_fps_display->setObjectName(QString::fromUtf8("last_fps_display"));
        last_fps_display->setGeometry(QRect(10, 40, 61, 41));
        last_fps_display->setNumDigits(4);
        last_fps_display->setDigitCount(4);
        mean_fps_display = new QLCDNumber(FPS_box);
        mean_fps_display->setObjectName(QString::fromUtf8("mean_fps_display"));
        mean_fps_display->setGeometry(QRect(70, 40, 61, 41));
        mean_fps_display->setNumDigits(4);
        mean_fps_display->setDigitCount(4);
        save_hdr_button = new QPushButton(dockWidgetContents_4);
        save_hdr_button->setObjectName(QString::fromUtf8("save_hdr_button"));
        save_hdr_button->setGeometry(QRect(10, 160, 111, 21));
        save_hdr_button->setFont(font1);
        save_ldr_button = new QPushButton(dockWidgetContents_4);
        save_ldr_button->setObjectName(QString::fromUtf8("save_ldr_button"));
        save_ldr_button->setGeometry(QRect(10, 190, 111, 21));
        save_ldr_button->setFont(font1);
        settings_widget->setWidget(dockWidgetContents_4);
        MainWindowClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), settings_widget);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionMake);
        menuHelp->addAction(actionHelp);
        menuHelp->addAction(actionAbout);
        menuView->addAction(actionRestore);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("MainWindowClass", "About", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainWindowClass", "Open Image", 0, QApplication::UnicodeUTF8));
        actionMake->setText(QApplication::translate("MainWindowClass", "Make HDR Image", 0, QApplication::UnicodeUTF8));
        actionHelp->setText(QApplication::translate("MainWindowClass", "Help", 0, QApplication::UnicodeUTF8));
        actionRestore->setText(QApplication::translate("MainWindowClass", "Restore", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindowClass", "File", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindowClass", "Help", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("MainWindowClass", "View", 0, QApplication::UnicodeUTF8));
        file_box->setTitle(QApplication::translate("MainWindowClass", "HDR Image", 0, QApplication::UnicodeUTF8));
        open_button->setText(QApplication::translate("MainWindowClass", "Open", 0, QApplication::UnicodeUTF8));
        make_button->setText(QApplication::translate("MainWindowClass", "Make", 0, QApplication::UnicodeUTF8));
        rad_button->setText(QApplication::translate("MainWindowClass", "Calculate", 0, QApplication::UnicodeUTF8));
        tonemap_box->setTitle(QApplication::translate("MainWindowClass", "Tonemapping", 0, QApplication::UnicodeUTF8));
        algo_select->clear();
        algo_select->insertItems(0, QStringList()
         << QApplication::translate("MainWindowClass", "Linear", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindowClass", "Logarithmic", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindowClass", "Bilateral", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindowClass", "Histogram", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindowClass", "Perceptual", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindowClass", "Blur", 0, QApplication::UnicodeUTF8)
        );
        param_label_2->setText(QApplication::translate("MainWindowClass", "Param2:", 0, QApplication::UnicodeUTF8));
        param_label_1->setText(QApplication::translate("MainWindowClass", "Param1:", 0, QApplication::UnicodeUTF8));
        tonemap_button->setText(QApplication::translate("MainWindowClass", "Go!", 0, QApplication::UnicodeUTF8));
        param_label_3->setText(QApplication::translate("MainWindowClass", "Param3:", 0, QApplication::UnicodeUTF8));
        FPS_box->setTitle(QApplication::translate("MainWindowClass", "FPS", 0, QApplication::UnicodeUTF8));
        last_fps_label->setText(QApplication::translate("MainWindowClass", "Last:", 0, QApplication::UnicodeUTF8));
        mean_fps_label->setText(QApplication::translate("MainWindowClass", "Mean:", 0, QApplication::UnicodeUTF8));
        save_hdr_button->setText(QApplication::translate("MainWindowClass", "Save HDR", 0, QApplication::UnicodeUTF8));
        save_ldr_button->setText(QApplication::translate("MainWindowClass", "Save LDR", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
