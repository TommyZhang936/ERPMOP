#ifndef MAINWORK_H
#define MAINWORK_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QModelIndex>

namespace Ui {
class MainWork;
}

class MainWork : public QWidget
{
    Q_OBJECT

public:
    explicit MainWork(QWidget *parent = 0);
    ~MainWork();

signals:
    void S1InfoSend(const QString &);     //定义S1状态栏数据发送信号
    //void customContextMenuRequested();  //右键菜单信号

private slots:
    void on_pushButton_clicked();      //查询订单click()

    void on_pushButtonPMYES_clicked();    //修改订单click()

    void on_tableViewMain_doubleClicked(const QModelIndex &index);

    void on_tableViewMain_clicked(const QModelIndex &index);

    void on_pushButton_clearMark_clicked(); //清除全部备注信息

    //void RightClick();   //右键槽函数

    //后来发现可以使用UI内置的customContextMenuRequested信号槽
    void on_tableViewMain_customContextMenuRequested(/*const QPoint &pos*/);

private:
    void InitForm();            //初始化窗口控件
    void InitMainTable();   //初始化table
    void LoadData(const QString &);   //装载table数据

    QString MPriceMark;  //定义修改单价备注，用于修改单价语句
    int D50RKEY;               //定义型号D50表代码，用于修改单价语句
    QString oldSqlStr;     //保留SQL语句

    QSqlQueryModel *queryModel;
    QSqlQueryModel *oldModel;

    Ui::MainWork *ui;
};

#endif // MAINWORK_H
