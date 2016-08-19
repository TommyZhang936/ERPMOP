#ifndef USER_H
#define USER_H

#include <QDialog>

namespace Ui {
class User;
}

class User : public QDialog
{
    Q_OBJECT

public:
    explicit User(QWidget *parent = 0);
    ~User();

private slots:
    void InitForm();
    void on_pushButtonE_clicked();

    void on_pushButtonM_clicked();

    void on_lineEditPR_textChanged(/*const QString &arg1*/);

    void on_lineEditAP2_textChanged(/*const QString &arg1*/);

    void on_lineEditA1_editingFinished();

    void on_pushButtonA_clicked();

private:
    Ui::User *ui;
};

#endif // USER_H
