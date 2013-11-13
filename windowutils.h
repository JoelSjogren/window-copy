#ifndef WINDOWUTILS_H
#define WINDOWUTILS_H
#include <QWidget>
namespace WindowUtils
{
    // todo encapsulate in class?
    QList<WId> windows();
    QString windowTitle(WId);
    bool isValid(WId);
    QRect windowGeometry(WId);
}

#endif // WINDOWUTILS_H
