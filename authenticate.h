#ifndef AUTHENTICATE_H
#define AUTHENTICATE_H

#include <QDialog>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

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

signals:
    void okClicked();

public slots:
    void setMessage(QString message);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_lineEdit_returnPressed();

private:
    Ui::Authenticate *ui;
    QGraphicsOpacityEffect *fadeEffect;
};

#endif // AUTHENTICATE_H
