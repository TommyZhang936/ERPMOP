#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "mainwork.h"
#include "user.h"

class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void S1InfoText(const QString &);  //子窗口消息接收槽函数

private slots:
    void mainWork();
    void user();

private:
    void createActions();
    void createToolBars();
    void createStatusBar();

    //状态栏label
    QLabel *S1Label;
    QLabel *S2Label;

    QAction *mainAct;
    QAction *userAct;
    QAction *exitAct;

    QToolBar *mainToolBar;

    //自定义窗口
    User *userDialog;
    MainWork *mainDialog;
};



#endif // MAINWINDOW_H
