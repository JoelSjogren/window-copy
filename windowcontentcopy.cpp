#include "windowcontentcopy.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QPixmap>
#include <QDebug>
#include <QxtGui/QxtWindowSystem>
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
    QPixmap windowPixmap = QPixmap::grabWindow(targetWindow());
    painter.drawPixmap(QPoint(), windowPixmap);
    event->accept();
}

void WindowContentCopy::checkTargetWindow()
{
    if (!m_hasWindow) return;
    if (QxtWindowSystem::windowGeometry(targetWindow()).isNull()) {
        m_hasWindow = false;
        window()->setWindowTitle("window-copy");
    }
}

void WindowContentCopy::copyTitle()
{
    window()->setWindowTitle(
                QxtWindowSystem::windowTitle(targetWindow()) + "#");
}

// todo add signal to be sent when target window closes

void WindowContentCopy::update()
{
    checkTargetWindow();
    if (!m_hasWindow) return;
    QWidget::update();
    qDebug() << QxtWindowSystem::windowGeometry(targetWindow()).size();
    window()->resize(QxtWindowSystem::windowGeometry(targetWindow()).size());
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
