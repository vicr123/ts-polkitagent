#include "authenticate.h"
#include "ui_authenticate.h"

Authenticate::Authenticate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Authenticate)
{
    ui->setupUi(this);

    ui->errorIcon->setPixmap(QIcon::fromTheme("dialog-error").pixmap(16, 16));
    ui->errorFrame->setVisible(false);
    fadeEffect = new QGraphicsOpacityEffect(this);
    ui->authFrame->setGraphicsEffect(fadeEffect);
}

Authenticate::~Authenticate()
{
    delete ui;
}

void Authenticate::showFullScreen(bool showError) {
    ui->errorFrame->setVisible(showError);

    ui->authFrame->setVisible(true);
    QPropertyAnimation *a = new QPropertyAnimation(fadeEffect, "opacity");
    a->setDuration(250);
    a->setStartValue(0);
    a->setEndValue(1);
    a->start();
    connect(a, SIGNAL(finished()), a, SLOT(deleteLater()));

    QDialog::showFullScreen();
}

void Authenticate::setMessage(QString message) {
    ui->message->setText(message);
}

void Authenticate::on_pushButton_2_clicked()
{
    //this->accept();
    QPropertyAnimation *a = new QPropertyAnimation(fadeEffect, "opacity");
    a->setDuration(250);
    a->setStartValue(1);
    a->setEndValue(0);
    a->start();
    connect(a, SIGNAL(finished()), a, SLOT(deleteLater()));
    connect(a, &QPropertyAnimation::finished, [=]() {
        ui->authFrame->setVisible(false);
    });

    emit okClicked();
}

void Authenticate::on_pushButton_clicked()
{
    this->reject();
}

QString Authenticate::getPassword() {
    QString ret = ui->lineEdit->text();
    ui->lineEdit->setText("");
    return ret;
}

void Authenticate::on_lineEdit_returnPressed()
{
    ui->pushButton_2->click();
}
