#ifndef AUTHENTICATE_H
#define AUTHENTICATE_H

#include <QDialog>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QIcon>
#include <QTimer>
#include <QDateTime>
#include <QDBusInterface>
#include <QDesktopWidget>
#include <QApplication>
#include <QProcess>
#include <QPushButton>
#include "tvirtualkeyboard.h"

namespace Ui {
class Authenticate;
}

class Authenticate : public QDialog
{
    Q_OBJECT

public:
    explicit Authenticate(QWidget *parent = 0);
    ~Authenticate();

    QString getPassword();
    void showFullScreen(bool showError = false);

    void setGeometry(int x, int y, int w, int h);
    void setGeometry(QRect geometry);

signals:
    void okClicked();

public slots:
    void setMessage(QString message);

    void setUser(QString user);

    void setIcon(QIcon icon);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_lineEdit_returnPressed();

    void on_keyboardButton_clicked();

private:
    Ui::Authenticate *ui;
    QGraphicsOpacityEffect *fadeEffect;
};

#endif // AUTHENTICATE_H
