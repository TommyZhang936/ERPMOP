#include "user.h"
#include "ui_user.h"
#include "myapp.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>

User::User(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::User)
{
    ui->setupUi(this);

    //设置窗口背景
    this->setStyleSheet(" font: 10pt \"微软雅黑\"; background-image:url(:/images/BackgroundM3.gif); ");

    InitForm();

}

User::~User()
{
    delete ui;
}

//初始化窗体
void User::InitForm()
{
    //初始化窗口
    ui->lineEditM1->setText(myApp::userName);
    ui->lineEditM2->setText(myApp::userCName);

    //下拉框赋值（比较丑，回头修正，比如从数据库读数据之类）
    QStringList permissionA;
    permissionA <<"A" << "B" << "C" << "D";

    ui->lineEditPO->clear();
    ui->lineEditPN->clear();
    ui->lineEditPR->clear();

    ui->lineEditA1->clear();
    ui->lineEditA2->clear();
    ui->comboBoxP->clear();
    ui->lineEditAP1->clear();
    ui->lineEditAP2->clear();

    ui->groupBoxM->setEnabled(true);

    if(myApp::userPermission == "A")
    {
        ui->groupBoxA->setEnabled(true);
        ui->comboBoxP->addItems(permissionA);
    }
}

void User::on_pushButtonE_clicked()
{
    //清除输入框内容，否则下次进入还在，或者明天改成每次重新生成
    //构建时设置setAttribute(Qt::WA_DeleteOnClose)退出销毁窗体即可
    //这样简单很多，就不知道有没有什么原则或隐患
    //明天和群里兄弟们讨教下看
    InitForm();

    this->close();
}

void User::on_pushButtonM_clicked()
{
    //判断原来密码是否正确
    QString newPWD = ui->lineEditPN->text();
    if( newPWD == ui->lineEditPR->text())
        if(ui->lineEditPO->text() == myApp::userPaswword)
        {
            //旧密码不同于新密码才保存
            if(newPWD == myApp::userPaswword)
            {
                QMessageBox::warning(NULL, QObject::tr("重置错误"), QObject::tr("新密码和旧密码完全一样！你改个什么东西？！"), QMessageBox::Ok);
                //返回重新设置新密码
                ui->lineEditPN->clear();
                ui->lineEditPR->clear();
                ui->lineEditPN->setFocus();
            }
            else
            {
                //构造修改密码sql语句
                QSqlQuery query;
                QString sqlStr = "update [myuser] set";
                sqlStr += " [password] = '" + newPWD;
                sqlStr += "' where [name] = '" + myApp::userName + "'";
                //执行sql语句
                qDebug() << sqlStr;
                //检验sql语句操作是否成功
                if (query.exec(sqlStr))
                {
                    //成功略延迟
                    myApp::userPaswword = newPWD;
                    //！非常重要：更改全局参数myApp:userPassword，否则再次修改密码将会出错
                    myApp::Sleep(100);
                    QMessageBox::information(NULL, tr("密码修订"), tr("恭喜你～密码修改成功～请务必记得新的密码哦！"), QMessageBox::Ok);
                }
                else
                {
                    QMessageBox::warning(NULL, QObject::tr("数据库错误"), QObject::tr("数据库更改出错了！密码修改不成功！请联系管理员!"), QMessageBox::Ok);
                }
                //close();
            }
        }
        else
        {
            QMessageBox::warning(NULL, QObject::tr("密码错误"), QObject::tr("你不是忘记了原来的密码?!那可不能修改啊！请重新输入正确的密码!"), QMessageBox::Ok);
            //如果旧密码错误则全部清除重新设置，以避免重新输入旧密码时忘记新设的密码导致问题
            ui->lineEditPN->clear();
            ui->lineEditPR->clear();
            ui->lineEditPO->clear();
            ui->lineEditPO->setFocus();
        }
    else
    //判断两次是否录入正确的密码
    {
        QMessageBox::warning(NULL, QObject::tr("重置错误"), QObject::tr("认真点落~两次输入密码不一致！请重新输入正确的密码！"), QMessageBox::Ok);
        ui->lineEditPN->clear();
        ui->lineEditPR->clear();
        ui->lineEditPN->setFocus();
    }
}

void User::on_lineEditPR_textChanged(/*const QString &arg1*/)
{
    ui->pushButtonM->setEnabled(true);
}

void User::on_lineEditAP2_textChanged(/*const QString &arg1*/)
{
    ui->groupBoxM->setEnabled(false);
    ui->pushButtonA->setEnabled(true);
}

void User::on_lineEditA1_editingFinished()
{
    QSqlQuery query;
    QString sqlStr = "select [name] from [myuser] where ";
    sqlStr += " [name] = '" + ui->lineEditA1->text() + "'";
    query.exec(sqlStr);
    if (query.next())
    {
        QString dispStr = QObject::tr("已经有代号 %1 这个用户了！不能重复！请重新输入新的用户代码！").arg(ui->lineEditA1->text());
        QMessageBox::warning(NULL, QObject::tr("用户错误"), dispStr, QMessageBox::Ok);
        ui->lineEditA1->clear();
        ui->lineEditA1->setFocus();
    }
}

void User::on_pushButtonA_clicked()
{
    //bool checkOK = false;
    //用户代码不能为空
    if(ui->lineEditA1->text().isEmpty())
    {
        QMessageBox::warning(NULL, QObject::tr("用户代码空"), QObject::tr("搞啥子鬼哦~用户代码都不得~加啥子鬼嘛?！"), QMessageBox::Ok);
        ui->lineEditA1->setFocus();
        return;
    }
    //中文名不能为空
    if(ui->lineEditA2->text().isEmpty())
    {
        QMessageBox::warning(NULL, QObject::tr("用户名空"), QObject::tr("用户不能没有中文名字哦！请补充完整！"), QMessageBox::Ok);
        ui->lineEditA2->setFocus();
        return;
    }
    //权限不能为空
    if(ui->comboBoxP->currentText().isEmpty())
    {
        QMessageBox::warning(NULL, QObject::tr("没有权限"), QObject::tr("没有为用户选择权限！请补充完整！"), QMessageBox::Ok);
        ui->comboBoxP->setFocus();
        return;
    }
    //两次密码需要一致且不能为空
    if(ui->lineEditAP1->text().isEmpty() or ui->lineEditAP1->text() != ui->lineEditAP2->text())
    {
        QMessageBox::warning(NULL, QObject::tr("密码不对"), QObject::tr("两次密码必须一致且密码不能为空！请重新设置密码！"), QMessageBox::Ok);
        ui->lineEditAP1->clear();
        ui->lineEditAP2->clear();
        ui->lineEditAP1->setFocus();
        return;
    }

    //都检查OK，保存数据
    QSqlQuery query;
    QString sqlStr = "insert into myuser([name],[cname],[password],[permission]) values (";
    sqlStr += "'" + ui->lineEditA1->text() +"',";
    sqlStr += "'" + ui->lineEditA2->text() +"',";
    sqlStr += "'" + ui->lineEditAP1->text() +"',";
    sqlStr += "'" + ui->comboBoxP->currentText() +"')";
    //qDebug() << sqlStr;
    if (query.exec(sqlStr))
    {
        //成功略延迟
        myApp::Sleep(100);
        QMessageBox::information(NULL, tr("增加用户"), tr("恭喜你～新增用户成功～请务必记得新的用户名和密码哦！"), QMessageBox::Ok);
    }
    else
    {
        QMessageBox::warning(NULL, QObject::tr("数据库错误"), QObject::tr("数据库插入出错了！新增不成功！请联系管理员!"), QMessageBox::Ok);
    }
    //可继续增加
    ui->lineEditA1->clear();
    ui->lineEditA2->clear();
    ui->comboBoxP->clear();
    ui->lineEditAP1->clear();
    ui->lineEditAP2->clear();
    ui->lineEditA1->setFocus();

}
