#include "dialog.h"
#include "ui_dialog.h"
#include <QPixmap>
#include <QDebug>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>
#include <QxtGui/QxtWindowSystem>
#include <QStackedWidget>
#include <QMouseEvent>
#include "preferences.h"
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle("window-copy");
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::mousePressEvent(QMouseEvent *event)
{
    Preferences prefs;
    prefs.setModal(true);
    if (prefs.exec()) {
        ui->content->setTargetWindow(prefs.selected());
    }
    event->accept();
}







