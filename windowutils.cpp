#include "windowutils.h"

#if defined(__linux__)
#include <QxtGui/QxtWindowSystem>
#include <QList>
QList<WId> WindowUtils::windows()
{
    return QxtWindowSystem::windows();
}
QString WindowUtils::windowTitle(WId id)
{
    return QxtWindowSystem::windowTitle(id);
}
bool WindowUtils::isValid(WId id)
{
    return !QxtWindowSystem::windowGeometry(id).isNull();
}
QRect WindowUtils::windowGeometry(WId id)
{
    return QxtWindowSystem::windowGeometry(id);
}

#elif defined(__APPLE__)

#endif
