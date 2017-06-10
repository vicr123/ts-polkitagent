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

    //Set up clock
    QTimer* clockTimer = new QTimer();
    clockTimer->setInterval(1000);
    connect(clockTimer, &QTimer::timeout, [=]() {
        ui->dateLabel->setText(QDateTime::currentDateTime().toString("ddd dd MMM yyyy"));
        ui->timeLabel->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
    });
    clockTimer->start();

    connect(tVirtualKeyboard::instance(), &tVirtualKeyboard::keyboardVisibleChanged, [=](bool visible) {
        if (visible) {
            QRect desktopRect = QApplication::desktop()->screenGeometry();
            desktopRect.setHeight(desktopRect.height() - tVirtualKeyboard::instance()->height());
            this->setGeometry(desktopRect);
        } else {
            QRect desktopRect = QApplication::desktop()->screenGeometry();
            this->setGeometry(desktopRect);
        }
    });
}

Authenticate::~Authenticate()
{
    delete ui;
}

void Authenticate::showFullScreen(bool showError) {
    ui->errorFrame->setVisible(showError);
    ui->lineEdit->setText("");

    ui->authFrame->setVisible(true);
    QPropertyAnimation *a = new QPropertyAnimation(fadeEffect, "opacity");
    a->setDuration(250);
    a->setStartValue(0);
    a->setEndValue(1);
    a->start();
    connect(a, SIGNAL(finished()), a, SLOT(deleteLater()));

    //Show onscreen keyboard if it is running
    tVirtualKeyboard::instance()->showKeyboard();

    ui->keyboardButton->setVisible(tVirtualKeyboard::instance()->isKeyboardRunning());

    QRect desktopRect = QApplication::desktop()->screenGeometry();
    desktopRect.setHeight(desktopRect.height() - tVirtualKeyboard::instance()->height());
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    //QDialog::showFullScreen();
    this->show();
    this->setGeometry(desktopRect);
    ui->lineEdit->setFocus();
}

void Authenticate::setMessage(QString message) {
    ui->message->setText(message);
}

void Authenticate::setIcon(QIcon icon) {
    ui->polkitIcon->setPixmap(icon.pixmap(22, 22));
}

void Authenticate::on_pushButton_2_clicked()
{
    QPropertyAnimation *a = new QPropertyAnimation(fadeEffect, "opacity");
    a->setDuration(250);
    a->setStartValue(1);
    a->setEndValue(0);
    a->start();
    connect(a, SIGNAL(finished()), a, SLOT(deleteLater()));
    connect(a, &QPropertyAnimation::finished, [=]() {
        ui->authFrame->setVisible(false);
        ui->lineEdit->setText("");
    });

    emit okClicked();
}

void Authenticate::setUser(QString user) {
    ui->authenticationUser->setText(user);
}

void Authenticate::on_pushButton_clicked()
{
    this->reject();
}

QString Authenticate::getPassword() {
    QString ret = ui->lineEdit->text();
    return ret;
}

void Authenticate::on_lineEdit_returnPressed()
{
    ui->pushButton_2->click();
}


void Authenticate::setGeometry(int x, int y, int w, int h) { //Use wmctrl command because KWin has a problem with moving windows offscreen.
    QDialog::setGeometry(x, y, w, h);
    QProcess::execute("wmctrl -r " + this->windowTitle() + " -e 0," +
                      QString::number(x) + "," + QString::number(y) + "," +
                      QString::number(w) + "," + QString::number(h));
}

void Authenticate::setGeometry(QRect geometry) {
    this->setGeometry(geometry.x(), geometry.y(), geometry.width(), geometry.height());
}

void Authenticate::on_keyboardButton_clicked()
{
    tVirtualKeyboard::instance()->showKeyboard();
}
