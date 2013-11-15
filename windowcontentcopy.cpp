#include "windowcontentcopy.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QPixmap>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include "windowutils.h"
WindowContentCopy::WindowContentCopy(QWidget *parent) :
    QWidget(parent), m_hasWindow(false)
{
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(20);
}

void WindowContentCopy::paintEvent(QPaintEvent *event)
{
    checkTargetWindow();
    if (!m_hasWindow) {
        event->accept();
        return;
    }
    QPainter painter(this);
    QPixmap windowPixmap = WindowUtils::grabWindow(targetWindow());
    //qDebug() << windowPixmap.size() << "for target window" << targetWindow();
    painter.drawPixmap(QPoint(), windowPixmap);
    event->accept();
}

void WindowContentCopy::checkTargetWindow()
{
    if (!m_hasWindow) return;
    if (!WindowUtils::isValid(targetWindow())) {
        m_hasWindow = false;
        window()->setWindowTitle("window-copy");
    }
}

void WindowContentCopy::copyTitle()
{
    window()->setWindowTitle(
                WindowUtils::windowTitle(targetWindow()) + "#");
}

// todo add signal to be sent when target window closes

void WindowContentCopy::update()
{
    checkTargetWindow();
    if (!m_hasWindow) return;
    QWidget::update();
    //qDebug() << WindowUtils::windowGeometry(targetWindow()).size();
    window()->resize(WindowUtils::windowGeometry(targetWindow()).size());
    copyTitle();
}

void WindowContentCopy::setTargetWindow(WId arg)
{
    m_window = arg;
    m_hasWindow = true;
//    copyTitle();
}

WId WindowContentCopy::targetWindow() const
{
    return m_window;
}
