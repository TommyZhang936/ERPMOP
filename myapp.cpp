#include "myapp.h"
//#include "myhelper.h"

#ifdef Q_OS_WIN
int myApp::AppFontSize = 22;                     //字符图标大小
QString myApp::AppFontName = "Microsoft YaHei";
#endif

#ifdef Q_OS_LINUX
#ifdef __arm__
int myApp::AppFontSize = 11;
QString myApp::AppFontName = "WenQuanYi Micro Hei";
#else
int myApp::AppFontSize = 11;
QString myApp::AppFontName = "WenQuanYi";
#endif
#endif

#ifdef Q_OS_MAC
int myApp::AppFontSize = 11;
QString myApp::AppFontName = "Heiti SC";
#endif

QString myApp::AppPath = "";
int myApp::DeskWidth = 1024;
int myApp::DeskHeight = 768;

QString myApp::AppCompany = "【深圳统信电路】";
QString myApp::AppName = "ERPMOP";
QString myApp::AppTitle = "ERP系统客户订单修订系统";
QString myApp::AppVerA = "Ver 1.2.0";
QString myApp::AppVerB = "160101";
QString myApp::AppVer = myApp::AppVerA + "." + myApp::AppVerB;

QString myApp::userName = "";          //登录用户代码
QString myApp::userCName = "";         //用户中文名
QString myApp::userPaswword = "";    //用户密码
QString myApp::userPermission = "";   //用户权限

QString myApp::DBServer = "127.0.0.1";       //数据库服务器
QString myApp::DBName = "PCBERP";       //数据库名
QString myApp::DBReadUser = "sa";         //数据库用户名
QString myApp::DBReadUserPwd = "lxb2249984";   //数据库用户密码

QString myApp::workCustomer = "富士康汇总";   //客户简称
QString myApp::workProdNO = "";                         //产品型号
QString myApp::workOPCSPrice = "";                    //原PCS单价
QString myApp::workMPCSPrice = "";                   //修改后PCS单价
QString myApp::workSqlString = "";                      //前一个SQL语句
QDomDocument myApp::doc;

//QString myApp::NVRType = "深广;宝学;敏达;泰杰;汇迪;银江;海康;大华;宇视;皓维;银海";
//QString myApp::IPCType = "泰杰;敏达;银江;海康;大华;宇视";
//QString myApp::AppStyle = ":/image/silvery.css";

//读取配置文件
void myApp::ReadConfig()
{
    QString fileName = myApp::AppPath + "WorkLog/config.txt";
    //如果配置文件不存在,则以初始值继续运行
    if (!myApp::FileIsExist(fileName))
    {
        return;
    }

    QSettings *set = new QSettings(fileName, QSettings::IniFormat);

    //系统信息只写不读
 /*
    set->beginGroup("AppConfig");
    //系统参数2
    myApp::AppTitle = set->value("AppTitle").toString();
    myApp::AppName = set->value("AppName").toString();
    myApp::AppVer = set->value("AppVer").toString();
    set->endGroup();
*/
    set->beginGroup("UserConfig");
    //用户参数1
    myApp::userName = set->value("UserName").toString();
    set->endGroup();

    set->beginGroup("WorkConfig");
    //工作参数1
    myApp::workCustomer = set->value("CustomerName").toString();
    set->endGroup();

    set->beginGroup("DBConfig");
    //DBServer参数1
    QString cStr1, cStr2;
    myApp::DBServer = set->value("DBServer").toString();
    myApp::DBName = set->value("DBName").toString();
    cStr1 = set->value("DBUS1").toString();
    myApp::DBReadUser = myApp::getXorEncryptDecrypt(cStr1, 11);
    cStr2 = set->value("DBUS2").toString();
    myApp::DBReadUserPwd = myApp::getXorEncryptDecrypt(cStr2, 11);
    set->endGroup();
}
//注意读写对应(搞清楚了，没有问题，QSetting函数是按照项目名称对应的)
//写入配置文件
//现在这种方式保证读写一致性困难，大型项目必须要有专门人员维护
//感觉这个应该用一个map实现更好，回头研究下方法
void myApp::WriteConfig()
{
    QString fileName = myApp::AppPath + "WorkLog/config.txt";
    QSettings *set = new QSettings(fileName, QSettings::IniFormat);

    set->beginGroup("AppConfig");
    //系统参数3
    set->setValue("AppTitle", myApp::AppTitle);
    set->setValue("AppName", myApp::AppName);
    set->setValue("AppVer", myApp::AppVer);
    set->endGroup();

    set->beginGroup("UserConfig");
    //用户参数1
    set->setValue("UserName", myApp::userName);
    set->endGroup();

    set->beginGroup("WorkConfig");
    //工作参数1
    set->setValue("CustomerName", myApp::workCustomer);
    set->endGroup();
//似乎不应该改写DB参数
    set->beginGroup("DBConfig");
    //DBServer参数2
    QString cStr1, cStr2;
    set->setValue("DBServer", myApp::DBServer);
    set->setValue("DBName", myApp::DBName);
    cStr1 = myApp::getXorEncryptDecrypt(myApp::DBReadUser, 11);
    set->setValue("DBUS1", cStr1 );
    cStr2 = myApp::getXorEncryptDecrypt(myApp::DBReadUserPwd, 11);
    set->setValue("DBUS2", cStr2);
    set->endGroup();
//
}
//-------------------------------------------------------------------------------------------
//数据库连接
bool myApp::CreateConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    db.setDatabaseName(QString("DRIVER={SQL SERVER};"
                               "SERVER=%1;"
                               "DATABASE=%2;"
                               "UID=%3;"
                               "PWD=%4;")
                        .arg(myApp::DBServer)
                       .arg(myApp::DBName)
                       .arg(myApp::DBReadUser)
                       .arg(myApp::DBReadUserPwd));
    if (!db.open())
    {
        qDebug()<<"Error: Connot open Database!";
        qDebug() << myApp::DBServer << myApp::DBName << myApp::DBReadUser << myApp::DBReadUserPwd;
        return false;
    }
    else
    {
        qDebug()<<"OK! Connect to Database Success !";
        return true;
    }
}
//-------------------------------------------------------------------------------------------
//日志文件
bool myApp::CreateLogXML()
{
    QString fileStr = myApp::AppPath + "WorkLog/WorkLog.xml";
    //qDebug() << fileStr;
    QFile file(fileStr);
    if(file.exists()) return true;
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return false;
    }
    //QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = myApp::doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    myApp::doc.appendChild(instruction);
    QDomElement root = myApp::doc.createElement(QString("单价修改日操作记录"));
    myApp::doc.appendChild(root);

    QTextStream out(&file);
    myApp::doc.save(out,4);
    file.close();
    return true;
}

// 读取XML文档
bool myApp::ReadXML()
{
    QString fileStr = myApp::AppPath + "WorkLog/WorkLog.xml";
    QFile file(fileStr);
    if(!file.open(QIODevice::ReadOnly))
        return false;
    if(!myApp::doc.setContent(&file)){
        file.close();
        return false;
    }
    file.close();
    return true;
}

// 写入xml文档
bool myApp::WriteXML()
{
    QString fileStr = myApp::AppPath + "WorkLog/WorkLog.xml";
    QFile file(fileStr);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;
    QTextStream out(&file);
    myApp::doc.save(out,4);
    file.close();
    return true;
}

// 将当前操作写入文档
void myApp::WriteLOG()
{
    // 先从文件读取
    if (myApp::ReadXML()) {
        QString currentDate = myApp::GetDateTime(Date);
        QDomElement root = myApp::doc.documentElement();
        // 根据是否有日期节点进行处理
        if (!root.hasChildNodes())
        {
            QDomElement date = myApp::doc.createElement(QString("日期"));
            QDomAttr curDate = myApp::doc.createAttribute("date");
            curDate.setValue(currentDate);
            date.setAttributeNode(curDate);
            root.appendChild(date);
            myApp::CreateNodes(date);
        }
        else
        {
            QDomElement date = root.lastChild().toElement();
            // 根据是否已经有今天的日期节点进行处理
            if (date.attribute("date") == currentDate)
            {
                myApp::CreateNodes(date);
            }
            else
            {
                QDomElement date = myApp::doc.createElement(QString("日期"));
                QDomAttr curDate = myApp::doc.createAttribute("date");
                curDate.setValue(currentDate);
                date.setAttributeNode(curDate);
                root.appendChild(date);
                myApp::CreateNodes(date);
            }
        }
        // 写入到文件
        WriteXML();
    }
}

// 创建节点
void myApp::CreateNodes(QDomElement &date)
{
    QDomElement time = myApp::doc.createElement(QString("时间"));
    QDomAttr curTime = myApp::doc.createAttribute("time");
    curTime.setValue(myApp::GetDateTime(Time));
    time.setAttributeNode(curTime);
    date.appendChild(time);

    QDomElement type = myApp::doc.createElement(QString("修改内容"));
    QDomElement sql = myApp::doc.createElement(QString("SQL语句"));

    QDomText text;
    text = myApp::doc.createTextNode(QString("客户：" + myApp::workCustomer
                                 + " 型号：" + myApp::workProdNO
                                 + " 单价从：" + myApp::workOPCSPrice
                                 + " 变更为： " + myApp::workMPCSPrice
                                 + " 操作人：" + myApp::userName));

    type.appendChild(text);

    text = myApp::doc.createTextNode(myApp::workSqlString);

    sql.appendChild(text);

    time.appendChild(type);
    time.appendChild(sql);

}

// 获取当前的日期或者时间
QString myApp::GetDateTime(myApp::DateTimeType type)
{
    QDateTime datetime = QDateTime::currentDateTime();
    QString date = datetime.toString("yyyy-MM-dd");
    QString date8 = datetime.toString("yyyyMMdd");
    QString time = datetime.toString("hh:mm");
    QString dateAndTime = datetime.toString("yyyy-MM-dd dddd hh:mm");
    if(type == Date) return date;
    else if(type == Date8) return date8;
    else if(type == Time) return time;
    else return dateAndTime;
}

//延时
void myApp::Sleep(int sec)
{
    QTime dieTime = QTime::currentTime().addMSecs(sec);
    while ( QTime::currentTime() < dieTime ) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

bool myApp::FileIsExist(QString strFile)
{
    QFile tempFile(strFile);
    return tempFile.exists();
}

//加解密
QString myApp::getXorEncryptDecrypt(const QString &str, const char &key)
{
  QString result;
  QByteArray bs = myApp::qstringToByte(str);

  for(int i=0; i<bs.size(); i++)
  {
    bs[i] = bs[i] ^ key;
  }

  result = myApp::byteToQString(bs);
  return result;
}

QString myApp::byteToQString(const QByteArray &byte)
{
  QString result;
  if(byte.size() > 0)
  {
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    result = codec->toUnicode(byte);
  }

  return result;
}

QByteArray myApp::qstringToByte(const QString &strInfo)
{
  QByteArray result;
  if(strInfo.length() > 0)
  {
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    result = codec->fromUnicode(strInfo);
  }

  return result;
}
