#include "logindialog.h"
#include "ui_logindialog.h"
#include "myapp.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    setWindowTitle(tr("系统登录"));
    setWindowIcon(QIcon(":/images/Icon.png"));
    ui->label->setText("<html><head/><body><p>"+ myApp::AppTitle + "</p></body></html>");

    //设置窗口背景
    setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/images/BackgroundM1.gif");
    palette.setBrush(QPalette::Background, QBrush(pixmap));
    setPalette(palette);

    ui->userLineEdit->setText(myApp::userName);
    if (myApp::userName.isEmpty())
        ui->userLineEdit->setFocus();
    else
        ui->pwdLineEdit->setFocus();
    ui->loginBtn->setDefault(true);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

// 登录按钮
void LoginDialog::on_loginBtn_clicked()
{
    if (ui->pwdLineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("密码缺失"),
                                 tr("你连密码都没有输入哦~登录什么呢?！"), QMessageBox::Ok);
        ui->pwdLineEdit->setFocus();
    }
    else
    {
        QSqlQuery query;
        //这里不能直接比较myApp::userName，因为用户可能在这里修改登录名字，但是没有验证前原来保存的name并没改变
        QString nameStr =  ui->userLineEdit->text();
        QString sqlStr = QString("select [name], [password], [cname], [permission] from [myuser] where [name] = '%1'").arg(nameStr);

        //qDebug() << sqlStr;
        query.exec(sqlStr);

        if(query.next())
        {
            if ((query.value(0).toString() == ui->userLineEdit->text()) and (query.value(1).toString() == ui->pwdLineEdit->text()))
            {
                //在这里赋值后面操作所需的登录用户的4个全局参数
                //每次改变登录用户成功后，需要在myApp::userName赋值后写参数文件，以便保留
                if(nameStr != myApp::userName)
                {
                    myApp::userName = query.value(0).toString();
                    myApp::WriteConfig();
                }
                myApp::userCName = query.value(2).toString();
                myApp::userPaswword = query.value(1).toString();
                myApp::userPermission = query.value(3).toString();
                //qDebug() << "L --- " << myApp::userCName << myApp::userName;
                QDialog::accept();      //用户密码正确，发送接受信号
            }
            else
            {
                QMessageBox::warning(this, tr("登录错误"), tr("不要玩了~请输入正确的用户代码和正确密码再登录！"), QMessageBox::Ok);
                ui->pwdLineEdit->clear();            //用户不清除，清除密码
                ui->pwdLineEdit->setFocus();     //给一个输入焦点到用户输入框上
            }
        }
        else
        {
            QMessageBox::critical(NULL, QObject::tr("数据库错误"), QObject::tr("数据库错误！请联系管理员？！"), QMessageBox::Ok);
            QDialog::reject();
        }
    }
}

// 退出按钮
void LoginDialog::on_quitBtn_clicked()
{
    QDialog::reject();    //隐藏对话框，发送拒绝信号
}
