#include "mainwork.h"
#include "ui_mainwork.h"
#include "myapp.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>

MainWork::MainWork(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWork)
{
    ui->setupUi(this);

    //设置标题
    this->setWindowTitle(tr("修改订单"));
    //设置样式
    //this->setAutoFillBackground(true);
    //this->setStyleSheet(" font: 10pt \"微软雅黑\"; background-image:url(:/images/BackgroundM1.gif); ");

    //设置表格最大化（未搞定，回头再研究）
    //最后研究发现SizePlocy设置为Expanding即可

    //初始化控件状态
    this->InitForm();

    //初始化表格
    this->InitMainTable();

}

MainWork::~MainWork()
{
    delete ui;
}
//初始化窗口控件
void MainWork::InitForm()
{
    ui->lineEditCust->setText(myApp::workCustomer);

    ui->lineEditProdPCSPrice->clear();
    ui->lineEditProdSETPrice->clear();
    ui->lineEditProdBX->clear();
    ui->lineEditProdBXM->clear();
    ui->lineEditProdBY->clear();
    ui->lineEditProdBYM->clear();
    ui->lineEditProdSETPriceERT->clear();
    ui->lineEditProdPCSPriceM->clear();
    ui->lineEditProdSETPriceM->clear();
    //焦点在生产型号上
    ui->lineEditProdNo->setFocus();
    //修改单价无效
    ui->pushButtonPMYES->setEnabled(false);

}

//初始化table
void MainWork::InitMainTable()
{

    //最后一列自动填充
    ui->tableViewMain->horizontalHeader()->setStretchLastSection(true);
    //奇数偶数行不同背景色
    //ui->tableViewMain->setAlternatingRowColors(true);

    //列宽适合文字
    ui->tableViewMain->resizeColumnsToContents();
    //禁止编辑
    //ui->tableViewMain->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //选中整行,每次只允许选中一行
    ui->tableViewMain->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewMain->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableViewMain->setContextMenuPolicy(Qt::CustomContextMenu); //设置为自定义模式
    //将右键和自定义动作槽连接
    //connect(ui->tableViewMain, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(RightClick()));

    queryModel = new QSqlQueryModel(this);
    QString sqlStr = "SELECT [rkey] "
                         ",[order_no]"
                         ",[cust_code] "
                         ",[abbr_name] "
                         ",[prod_no] "
                         ",[prod_ver] "
                         ",[pcs_qty] "
                         ",[x] "
                         ",[y] "
                         ",[set_qty] "
                         ",[pcs_price] "
                         ",[set_price] "
                         ",[ex_rate] "
                         ",[so_style] "
                         ",[status] "
                         ",[prod_verA] "
                         ",[MP_remark] "
                         ",[D50Rkey] "
                     "FROM [v_orderprice] WHERE [abbr_name] = ";
    sqlStr += "'" +myApp::workCustomer +"' ";
    sqlStr += " ORDER BY [prod_no], [prod_ver] DESC";
    oldSqlStr = sqlStr;
    LoadData(sqlStr);

    QString columnNames[20]; //列名数组
    int columnWidths[20];    //列宽数组

    //初始化表格列名和列宽
    columnNames[0] = "系统编号";
    columnNames[1] = "订单号";
    columnNames[2] = "客户代码";
    columnNames[3] = "客户简称";
    columnNames[4] = "产品型号";
    columnNames[5] = "版";
    columnNames[6] = "订单PCS数";
    columnNames[7] = "拼";
    columnNames[8] = "板";
    columnNames[9] = "订单SET数";
    columnNames[10] = "PCS单价";
    columnNames[11] = "SET单价";
    columnNames[12] = "汇率";
    columnNames[13] = "状";
    columnNames[14] = "态";
    columnNames[15] = "整版";
    columnNames[16] = "单价修订备注";
    columnNames[17] = "D50Rkey";

    int width = myApp::DeskWidth - 246;
    columnWidths[0] = width * 0.06;
    columnWidths[1] = width * 0.09;
    columnWidths[2] = width * 0.08;
    columnWidths[3] = width * 0.12;
    columnWidths[4] = width * 0.07;
    columnWidths[5] = width * 0.02;
    columnWidths[6] = width * 0.06;
    columnWidths[7] = width * 0.03;
    columnWidths[8] = width * 0.03;
    columnWidths[9] = width * 0.06;
    columnWidths[10] = width * 0.06;
    columnWidths[11] = width * 0.06;
    columnWidths[12] = width * 0.07;
    columnWidths[13] = width * 0.02;
    columnWidths[14] = width * 0.02;
    columnWidths[15] = width * 0.04;
    columnWidths[16] = width * 0.20;
    columnWidths[17] = width * 0.06;

    //依次设置列标题列宽
    for (int i = 0; i < 18; i++) {
        queryModel->setHeaderData(i, Qt::Horizontal, columnNames[i]);
        ui->tableViewMain->setColumnWidth(i, columnWidths[i]);
    }
}
//装载表格数据
void MainWork::LoadData(const QString &sqlStr)
{

    queryModel->setQuery(sqlStr);
    ui->tableViewMain->setModel(queryModel);
    //隐藏0行rkey
    //ui->tableViewMain->setColumnHidden(0, true);

    QHeaderView *header = ui->tableViewMain->horizontalHeader();
    //header = ui->tableViewMain->verticalHeader();    //垂直
    //最后栏位占据所有可见空间，水平方向方向由上面句设定
    header->setStretchLastSection(true);

    emit S1InfoSend(sqlStr);

}

//查询订单按钮函数
void MainWork::on_pushButton_clicked()
{


    //检验客户简称不能为空
    if (ui->lineEditCust->text().isEmpty())
    {
        QMessageBox::warning(NULL, QObject::tr("条件为空"), QObject::tr("客户简称不能为空！请输入正确数据再查询？！"), QMessageBox::Ok);
        ui->lineEditCust->setFocus();
        return;
    }

/*
    //检验客户简称和产品型号都不能为空
    if (ui->lineEditCust->text().isEmpty() or ui->lineEditProdNo->text().isEmpty())
    {
        QMessageBox::warning(NULL, QObject::tr("条件为空"), QObject::tr("客户简称和产品型号/版本都不能为空！请输入正确数据再查询？！"), QMessageBox::Ok);
        ui->lineEditCust->setFocus();
        return;
    }
*/

    if(myApp::workCustomer != ui->lineEditCust->text())
    {
        //如果查的是不同客户，保留客户简称
        myApp::workCustomer = ui->lineEditCust->text();
        myApp::WriteConfig();
    }

    QSqlQuery query;
    QString sqlStr ="SELECT "

                    "[rkey] "
                        ",[order_no]"
                        ",[cust_code] "
                        ",[abbr_name] "
                        ",[prod_no] "
                        ",[prod_ver] "
                        ",[pcs_qty] "
                        ",[x] "
                        ",[y] "
                        ",[set_qty] "
                        ",[pcs_price] "
                        ",[set_price] "
                        ",[ex_rate] "
                        ",[so_style] "
                        ",[status] "
                        ",[prod_verA] "
                        ",[MP_remark] "
                        ",[D50Rkey] "

                        "FROM [v_orderprice] "
                        "WHERE [abbr_name] = ";
    sqlStr += "'" +myApp::workCustomer +"' ";
    if(! ui->lineEditProdNo->text().isEmpty())
            sqlStr += "AND [prod_no] = '" + ui->lineEditProdNo->text() + "' ";
    if(! ui->lineEditPNVer->text().isEmpty())
            sqlStr += "AND [prod_ver] = '" + ui->lineEditPNVer->text() +"'";
    sqlStr += " ORDER BY [prod_no], [prod_ver] DESC";
    oldSqlStr = sqlStr;
    //qDebug() << sqlStr;
    //查询出错停止
    if (!query.exec(sqlStr))
    {
        QMessageBox::critical(NULL, QObject::tr("SQL错误"), QObject::tr("SQL查询错误！请联系管理员？！"), QMessageBox::Ok);
        ui->lineEditProdNo->setFocus();
        return;
    }
    //没有结果停止
    if(!query.next())
    {
                 QString dispStr = QObject::tr("没有符合要求的客户： %1 的产品 %2 之 %3 版本的""未完成""订单数据！\n请重新输入条件！")\
                         .arg(ui->lineEditCust->text())
                         .arg(ui->lineEditProdNo->text())
                         .arg(ui->lineEditPNVer->text());
                 QMessageBox::warning(NULL, QObject::tr("业务错误"), dispStr, QMessageBox::Ok);
                 if(!(ui->lineEditProdPCSPrice->text().isEmpty()))
                 {
                     InitForm(); //初始化窗口控件
                 }
                 return;
    }
    //OK，提取相关内容，修改单价
                 //设置单价/拼板数/汇率
                 QString PCSPrice = query.value(10).toString();
                 QString SETPrice = query.value(11).toString();
                 QString X = query.value(7).toString();
                 QString Y = query.value(8).toString();
                 QString Rate = query.value(12).toString();
                 D50RKEY = query.value(17).toInt();
                 MPriceMark = query.value(16).toString();

                 ui->lineEditProdPCSPrice->setText(PCSPrice);
                 ui->lineEditProdSETPrice->setText(SETPrice);
                 ui->lineEditProdBX->setText(X);
                 ui->lineEditProdBXM->setText(X);
                 ui->lineEditProdBY->setText(Y);
                 ui->lineEditProdBYM->setText(Y);
                 ui->lineEditProdSETPriceERT->setText(Rate);

                 //焦点在输入新的单价
                 ui->lineEditProdPCSPriceM->setEnabled(true);
                 ui->lineEditProdPCSPriceM->setFocus();
                 ui->pushButtonPMYES->setEnabled(true);
                 //装载table数据
                 LoadData(sqlStr);
}

//"修改单价"按钮函数
void MainWork::on_pushButtonPMYES_clicked()
{
    //检验修改价格合理性
    //取原单价/原生产型号rkey
    double OPCSPrice = ui->lineEditProdPCSPrice->text().toDouble();

    double MPCSPrice = ui->lineEditProdPCSPriceM->text().toDouble();
    double MX = ui->lineEditProdBXM->text().toDouble();
    double MY = ui->lineEditProdBYM->text().toDouble();
    double MSETPrice = MPCSPrice * MX * MY;
    ui->lineEditProdSETPriceM->setText(QString::number(MSETPrice, 10, 6));
    //判断价格是否有问题！比如为0
    if(MSETPrice == 0 or MPCSPrice == OPCSPrice)
    {
        QMessageBox::warning(NULL, QObject::tr("价格错误"), QObject::tr("输入单价不合规范（必须是有效且和原来单价不同的数值）！\n请输入正确数值再核准修改！"), QMessageBox::Ok);
        ui->lineEditProdPCSPriceM->clear();
        ui->lineEditProdPCSPriceM->setFocus();
        return;
    }

    bool pmConfirm = QMessageBox::question(NULL,
                                  tr("修改单价"),
                                  tr("你真的确定修改型号【 %1 】的单价吗？！").arg(ui->lineEditProdNo->text().toUpper()),
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::No) == QMessageBox::Yes;

    if (pmConfirm)
    {
        //修改单价，保存修改记录
        MPriceMark = QString(tr("%1到%2←%3"))
                .arg(myApp::GetDateTime(myApp::Date8))
                .arg(ui->lineEditProdPCSPriceM->text())
                .arg(ui->lineEditProdPCSPrice->text());
        QSqlQuery query;
        QString sqlStr = tr("UPDATE DATA0060 SET "
                            "[PART_PRICE] = %1, "
                            "[SET_PRICE] = %2, "
                            "[ANALYSIS_CODE_5] = '%3' "
                            "WHERE [CUST_PART_PTR] = %4 "
                            "AND SO_STYLE = '0' "
                            "AND [STATUS] NOT IN (3, 4, 5)")
                //视图上加了这个条件，这里就不用了
                //这里犯了个严重错误，视图过滤后，修正数据时必须加上这个条件
                //以后都要引以为戒，视图查询到的数据在修改时需要增加正确的过滤条件！
                .arg(MPCSPrice)
                .arg(MSETPrice)
                .arg(MPriceMark)
                .arg(D50RKEY);

        //qDebug() << sqlStr;

        if (query.exec(sqlStr)) //
        {
            //成功略延迟
            myApp::Sleep(200);
            //写日志
            myApp::workOPCSPrice = ui->lineEditProdPCSPrice->text();
            myApp::workMPCSPrice = ui->lineEditProdPCSPriceM->text();
            myApp::workProdNO = ui->lineEditProdNo->text().toUpper() + ui->lineEditPNVer->text().toUpper();
            myApp::workSqlString = sqlStr;
            myApp::WriteLOG();
            QMessageBox::information(NULL, tr("改价成功"), tr("恭喜恭喜～修改型号【 %1 】单价成功！")
                                     .arg(ui->lineEditProdNo->text()).toUpper(),
                                     QMessageBox::Ok);
            ui->pushButton_clearMark->setEnabled(true);
            //应该刷新table数据
            LoadData(oldSqlStr);
            emit S1InfoSend(sqlStr);
            //初始化控件
            InitForm();
        }
        else
        {
            QMessageBox::critical(NULL, QObject::tr("数据库错误"), QObject::tr("数据库更新出错了！修改单价不成功！请联系管理员!"), QMessageBox::Ok);
        }
    }
}
//双击跟单击一样功能(取型号到左边输入框)
void MainWork::on_tableViewMain_doubleClicked(const QModelIndex &index)
{
   this->on_tableViewMain_clicked(index);
}
//单击取型号到左边输入框
void MainWork::on_tableViewMain_clicked(const QModelIndex &index)
{
    if(index.isValid())
    {
        //取整行数据到modelList(感觉这个有点复杂了，不知道还有没有更好的办法？
        QModelIndexList modelList = ui->tableViewMain->selectionModel()->selectedIndexes();
        QString prodNo = modelList[4].data().toString();
        QString prodNoVer = modelList[5].data().toString();
        QString prodVERALL = modelList[15].data().toString();

        ui->lineEditProdNo->setText(prodNo);
        ui->lineEditPNVer->setText(prodNoVer);
        ui->lineEditPNVerIn->setText(prodVERALL);
        //修改状态栏信息
        QString sqlStr = tr("产品型号【%1 %2（%3）】PCS价格为：%4 拼板数：%5 * %6")
                .arg(prodNo)
                .arg(prodNoVer)
                .arg(prodVERALL)
                .arg(modelList[10].data().toDouble())
                .arg(modelList[7].data().toString())
                .arg(modelList[8].data().toString());

        emit S1InfoSend(sqlStr);
    }
}

//清除单价修订备注
void MainWork::on_pushButton_clearMark_clicked()
{
    bool cmConfirm = QMessageBox::question(NULL,
                                  tr("清除备注"),
                                  tr("你真的确定要清除所有单价修改备注么？！（警告：此操作不可恢复！)"),
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::No) == QMessageBox::Yes;

    if (cmConfirm)
    {
        QSqlQuery query;
        QString sqlStr = tr("UPDATE DATA0060 SET [ANALYSIS_CODE_5] = '' WHERE LEFT([ANALYSIS_CODE_5], 3) = '201' ");
        if (query.exec(sqlStr)) //
        {
            //成功略延迟
            myApp::Sleep(200);
            //写日志
            myApp::workOPCSPrice = myApp::userName;
            myApp::workMPCSPrice = myApp::userCName;
            myApp::workProdNO = "清除日志";
            myApp::workSqlString = sqlStr;
            myApp::WriteLOG();
            QMessageBox::information(NULL, tr("清除备注"), tr("恭喜恭喜～之前订单修订备注已经全部清除！"), QMessageBox::Ok);
            ui->pushButton_clearMark->setEnabled(false);
            LoadData(oldSqlStr);
        }
        else
        {
            QMessageBox::critical(NULL, QObject::tr("数据库错误"), QObject::tr("数据库更新出错了！清除订单修订备注不成功！请联系管理员!"), QMessageBox::Ok);
        }
    }
}
//右键修改单价
void MainWork::on_tableViewMain_customContextMenuRequested(/*const QPoint &pos*/)
{
    QModelIndex index = ui->tableViewMain->currentIndex();
    if(!index.isValid())
        return;
    //略延迟
    myApp::Sleep(200);

        //取整行数据到modelList(感觉这个有点复杂了，不知道还有没有更好的办法？
        QModelIndexList modelList = ui->tableViewMain->selectionModel()->selectedIndexes();
        int D60Rkey = modelList[0].data().toInt(0);
        QString orderNo = modelList[1].data().toString();
        QString prodNo = modelList[4].data().toString();
        QString prodNoVer = modelList[15].data().toString();
        double oldPrice = modelList[10].data().toDouble();
        int MX = modelList[7].data().toInt();
        int MY = modelList[8].data().toInt();

        bool isOK = 0;
        double  MPCSPrice = QInputDialog::getDouble(NULL, tr("输入单价"),
                                                   tr("订单 %1【 %2 %3 】原单价为：%4 \n\n请输入新单价！").arg(orderNo).arg(prodNo).arg(prodNoVer).arg(oldPrice),
                                                   oldPrice, -100, 100, 6, &isOK);

        if(!isOK)
            return;

        if(MPCSPrice == oldPrice)
        {
            QMessageBox::warning(NULL, QObject::tr("价格错误"), QObject::tr("新旧单价一样有啥必要修改？！"), QMessageBox::Ok);
            return;
        }

        //开始修改单价
        double MSETPrice = MPCSPrice * MX * MY;
        MPriceMark = QString(tr("%1特到%2←%3"))
                     .arg(myApp::GetDateTime(myApp::Date8))
                     .arg(oldPrice)
                     .arg(MPCSPrice);
        //qDebug() << D60Rkey << MPCSPrice << MSETPrice;
        QSqlQuery query;
        QString sqlStr = tr("UPDATE DATA0060 SET "
                                "[PART_PRICE] = %1, "
                                "[SET_PRICE] = %2, "
                                "[ANALYSIS_CODE_5] = '%3' "
                                "WHERE RKEY = %4")
                    .arg(MPCSPrice)
                    .arg(MSETPrice)
                    .arg(MPriceMark)
                    .arg(D60Rkey);

        if (!query.exec(sqlStr))
        {
            QMessageBox::critical(NULL, QObject::tr("数据库错误"), QObject::tr("数据库更新出错了！修改单价不成功！请联系管理员!"), QMessageBox::Ok);
             return;
        }
        //成功略延迟
        myApp::Sleep(200);
        //写日志
        myApp::workOPCSPrice = oldPrice;
        myApp::workMPCSPrice = MPCSPrice;
        myApp::workProdNO = prodNo + prodNoVer;
        myApp::workSqlString = sqlStr;
        myApp::WriteLOG();
        QMessageBox::information(NULL, tr("改价成功"), tr("恭喜恭喜～修改型号【 %1 】单价成功！")
                                         .arg(myApp::workProdNO).toUpper(),
                                         QMessageBox::Ok);
        ui->pushButton_clearMark->setEnabled(true);
        //应该刷新table数据
        LoadData(oldSqlStr);
        //更新状态栏信息
        emit S1InfoSend(sqlStr);
        //初始化控件
        InitForm();

}
