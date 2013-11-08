#ifndef WINDOWCONTENTCOPY_H
#define WINDOWCONTENTCOPY_H

#include <QWidget>
#include <QDebug>
class WindowContentCopy : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(WId targetWindow READ targetWindow WRITE setTargetWindow)
    WId m_window;
    bool m_hasWindow;
public:
    explicit WindowContentCopy(QWidget *parent = 0);
    WId targetWindow() const;
signals:
public slots:
    void setTargetWindow(WId arg);
    void update();
protected:
    void paintEvent(QPaintEvent *event);
private:
    void checkTargetWindow();
    void copyTitle();
};

#endif // WINDOWCONTENTCOPY_H








