#include <QApplication>
#include <QMessageBox>
#include <QSplashScreen>
#include <QDesktopWidget>
#include "mainwindow.h"
#include "logindialog.h"
#include "myapp.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //启动画面--------------------------------------------------------------------------------
    QPixmap pixmap("screen.png");
    QSplashScreen screen(pixmap);
    screen.setStyleSheet("font: 10pt \"微软雅黑\";");
    screen.showMessage(QObject::tr("系统启动中～Copyright @ 2015-2016 DSPring"), Qt::AlignCenter, Qt::red);
    screen.show();

    app.setApplicationName(myApp::AppName);         //设置应用程序名称
    app.setApplicationVersion(myApp::AppVer);           //设置应用程序版本



    //创建共享内存,判断是否已经运行程序
    QSharedMemory mem(myApp::AppName);
    if (!mem.create(1)) {
        QMessageBox::warning(NULL, QObject::tr("运行错误"), QObject::tr("系统已经在运行~请勿同时打开多个~点击OK关闭！"), QMessageBox::Ok);
        return false;
    }

    //赋值当前应用程序路径
    myApp::AppPath = QApplication::applicationDirPath() + "/";
    QDesktopWidget* desktop = QApplication::desktop();
    //QRect SRect = desktop->screenGeometry();                 //屏幕分辨率
    QRect SARect = desktop->availableGeometry();          //高度-Window任务栏40

    myApp::DeskWidth = SARect.width();
    myApp::DeskHeight = SARect.height();

    //qDebug() << "   Screen Geometry: " << desktop->screenGeometry();
    //qDebug() << "Screen AGeometry: " << desktop->availableGeometry();
    //qDebug() << "                Geometry: " << desktop->geometry();
    //qDebug() << "             Frame Sizes: " << desktop->frameSize();
    //qDebug() << "    Frame Geometry: " << desktop->frameGeometry();

    //程序加载时先加载所有配置信息
    myApp::ReadConfig();

    //判断数据库连接和读写日志文件是否正确，不正确则不允许
    if (!myApp::CreateConnection())
    {
        QMessageBox::critical(NULL, QObject::tr("连接错误"), QObject::tr("数据库连接异常~请确保网络连接正常或咨询管理员！"), QMessageBox::Ok);
        return false;
    }
    if (!myApp::CreateLogXML())
    {
        QMessageBox::critical(NULL, QObject::tr("日志错误"), QObject::tr("写日志文件异常~请确保日志目录/SystemLog/下磁盘空间足够或咨询管理员！"), QMessageBox::Ok);
        return false;
    }

    //启动系统登录界面
    LoginDialog dlg;
        //当在LoginDialog中运行QDialog::accept()函数时，会发送QDialog::Accepted信号，
        //当接收到时，再创建调用MainWindow界面

    //启动画面结束----------------------------------------------------------------------------
    screen.finish(&dlg);

        if (dlg.exec() == QDialog::Accepted)
        {
            //主窗口创建移到这里，登录成功再创建（也是为4个用户参数的传递，放在外面是初始值）
            MainWindow *mainWin = new MainWindow();

            mainWin->show();

            return app.exec();
        }
        else
        {
            return false;
        }
}
