#ifndef MYAPP_H
#define MYAPP_H

#include <QtSql>
#include <QtXml>

class QString;

class myApp
{
public:
    static int AppFontSize;                     //应用程序字体大小
    static QString AppFontName;         //应用程序字体名称

    static QString AppPath;                 //应用程序路径
    static int DeskWidth;                   //桌面可用宽度
    static int DeskHeight;                  //桌面可用高度

    static QString AppCompany;        //公司名称
    static QString AppName;             //应用名称
    static QString AppTitle;                //界面标题
    static QString AppVer;                 //应用版本总
    static QString AppVerA;                 //应用版本大
    static QString AppVerB;                 //应用版本小

    static QString userName;          //登录用户代码
    static QString userCName;         //用户中文名
    static QString userPaswword;    //用户密码
    static QString userPermission;   //用户权限

    static QString DBServer;       //数据库服务器
    static QString DBName;       //数据库名
    static QString DBReadUser;         //数据库用户名
    static QString DBReadUserPwd;   //数据库用户密码

    //工作参数
    static QString workCustomer;       //客户简称
    static QString workProdNO;          //产品型号
    static QString workOPCSPrice;      //原PCS单价
    static QString workMPCSPrice;     //修改后PCS单价
    static QString workSqlString;           //前一个SQL语句
    static QDomDocument doc;            //DOM文件句柄

    //日志相关
    static bool CreateConnection();  //数据库连接
    static bool CreateLogXML();                                        //创建日志文件
    static bool ReadXML();                                                  // 读取XML文档
    static bool WriteXML();                                                 //写XML文件
    static void WriteLOG();                                                  //写日志文件
    static void CreateNodes(QDomElement &date);     //增加节点
    static void ReadConfig();               //读取配置文件
    static void WriteConfig();              //写入配置文件

    static bool FileIsExist(QString strFile); //检测文件是否存在
    static void Sleep(int sec) ;            //延时
    //读取时间变成字符串
    enum DateTimeType{Time, Date, DateTime, Date8};   //定义时间的枚举类型
    static QString GetDateTime(DateTimeType type);

    //加解密
    static QString getXorEncryptDecrypt(const QString &, const char &);
    static QString byteToQString(const QByteArray &byte);
    static QByteArray qstringToByte(const QString &strInfo);

};

#endif // MYAPP_H
