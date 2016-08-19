#include "mainwindow.h"
#include "mainwork.h"
#include "user.h"
#include "myapp.h"
#include <QtWidgets>
#include <QDebug>
#include <QRect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QString mainWTitle = QObject::tr("%1 %2 %3").arg(myApp::AppCompany).arg(myApp::AppTitle).arg(myApp::AppVer);
    setWindowTitle(mainWTitle);

    //隐藏最大化最小化按钮
    setWindowFlags(windowFlags()
                   &~ Qt::WindowMinMaxButtonsHint
                   &~ Qt::MSWindowsFixedSizeDialogHint);
                //&~ Qt::WindowSystemMenuHint
    //设置窗口图标
    setWindowIcon(QIcon(":/images/Icon128.png"));
    //窗口最大化
    setWindowState(Qt::WindowMaximized);

    //设置样式
    //setStyleSheet("font: 10pt \"微软雅黑\"; ");

    //设置背景
    setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/images/BackgroundT1.gif");
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    setPalette(palette);

    //创建工具栏\状态栏
    createActions();
    createToolBars();
    createStatusBar();

}

MainWindow::~MainWindow()
{

}

void MainWindow::createActions()
{
    mainAct = new QAction(QIcon(":/images/MainWork.png"), tr("修改单价"), this);
    mainAct->setStatusTip(tr("主要工作显示区!"));
    connect(mainAct, SIGNAL(triggered()), this, SLOT(mainWork()));

    userAct = new QAction(QIcon(":/images/Password.png"), tr("用户密码"), this);
    userAct->setStatusTip(tr("增加用户和修改密码!"));
    connect(userAct, SIGNAL(triggered()), this, SLOT(user()));

    exitAct = new QAction(QIcon(":/images/Exit.png"), tr("退出系统"), this);
    exitAct->setStatusTip(tr("退出系统!"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::createToolBars()
{
    mainToolBar = addToolBar(tr("主工具"));
    mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    mainToolBar->addSeparator();
    mainToolBar->addAction(mainAct);
    mainToolBar->addAction(userAct);
    mainToolBar->addSeparator();

    //增加一个空白窗体以控制前面工具图标和最后退出工具图标之间的距离
    //这是一个非常实用的技巧
    QWidget *SpaceWidget = new QWidget(this);
    SpaceWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    mainToolBar->addWidget(SpaceWidget);
    mainToolBar->addSeparator();
    mainToolBar->addAction(exitAct);
    //下面这个是设置左侧工具栏和右侧工具栏
    //this->addToolBar(Qt::LeftToolBarArea, mainToolBar);
    //this->addToolBar(Qt::LeftToolBarArea, otherToolBar);

    //设置工具栏图标大小
    int toolHeight = 64;
    QSize toolBarSize(toolHeight, toolHeight);
    mainToolBar->setIconSize(toolBarSize);
    mainToolBar->setMovable(false);
    //遮盖测试
    //QBitmap bMap = QBitmap(":/images/Long.png");
    //mainToolBar->setMask(bMap);

}

void MainWindow::createStatusBar()
{
    statusBar();

    statusBar()->setFixedHeight(32);
    //statusBar()->setStyleSheet("{background: brown; }");

    S1Label = new QLabel;
    S2Label = new QLabel;

    S2Label->setStyleSheet( "background-color: beige; border: 1px solid green;  border-radius: 5px; color : red; " );
    statusBar()->addPermanentWidget(S1Label, 1);
    statusBar()->addPermanentWidget(S2Label, 3);

    //初始化状态栏文字
    QString status1T = tr("【用户】%1(%2) 【数据库】%3(%4)")
            .arg(myApp::userName)
            .arg(myApp::userCName)
            .arg(myApp::DBServer)
            .arg((myApp::DBName).left(3));
    S1Label->setText(status1T);
    this->S1InfoText("这里显示运作状态记录！");

}

void MainWindow::S1InfoText(const QString &str)
{
    this->S2Label->setText(str);
}

void MainWindow::mainWork()
{
    //if(!mainDialog)
    {
        mainDialog = new MainWork;
        connect(mainDialog, SIGNAL(S1InfoSend(const QString &)), this, SLOT(S1InfoText(const QString &))); //消息发送与接收
    }
    this->setCentralWidget(mainDialog);
    //mainDialog->showMaximized();
    mainDialog->show();
}

void MainWindow::user()
{
    //如果设置了setAttribute(Qt::WA_DeleteOnClose)退出时销毁，就每次直接创建新实例
    //那么就不需要在退出时清理原有的内容了
    //设置退出销毁会出错!
    //if(!userDialog)
    userDialog = new User(this);
    userDialog->setAttribute(Qt::WA_DeleteOnClose);
    userDialog->exec();
}

