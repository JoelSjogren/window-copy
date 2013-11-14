#include "windowutils.h"
#if defined(__linux__)
#include <QxtGui/QxtWindowSystem>
#include <QList>
#include <QPixmap>
QList<WId> WindowUtils::windows()
{
    return QxtWindowSystem::windows();
}
QString WindowUtils::windowTitle(WId id)
{
    return QxtWindowSystem::windowTitle(id);
}
QRect WindowUtils::windowGeometry(WId id)
{
    return QxtWindowSystem::windowGeometry(id);
}
bool WindowUtils::isValid(WId id)
{
    return !windowGeometry(id).isNull();
}
QPixmap WindowUtils::grabWindow(WId id)
{
    return QPixmap::grabWindow(id);
}
#elif defined(__APPLE__)
/* Resources:
 *  http://stackoverflow.com/questions/2919629/mac-os-x-linker-error-in-qt-coregraphics-cgwindowlistcreate
 *  https://developer.apple.com/library/mac/documentation/corefoundation/Reference/CFArrayRef/Reference/reference.html#//apple_ref/c/func/CFArrayGetTypeID
 *  http://stackoverflow.com/questions/14666123/take-screenshot-without-window
 * CF = CoreFoundation
 * CG = CoreGraphics -> Quartz
*/
//#include </System/Library/Frameworks/ApplicationServices.framework/Frameworks/CoreGraphics.framework/Headers/CGWindow.h>
#include <ApplicationServices/ApplicationServices.h>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>
// the following function was indirectly taken from src/corelib/kernel/qcore_mac.cpp
QString toQString(CFStringRef str)
{
    if (!str) return QString();
    CFIndex length = CFStringGetLength(str);
    if (length == 0) return QString();
    QString string(length, Qt::Uninitialized);
    CFStringGetCharacters(str, CFRangeMake(0, length), reinterpret_cast<UniChar *>
        (const_cast<QChar *>(string.unicode())));
    return string;
}
CFArrayRef createWindowDescription(CGWindowID id) {
    CGWindowID idCArray[1] = { id };
    CFArrayRef idArray = CFArrayCreate(NULL, (const void **) idCArray, 1, NULL);
    CFArrayRef result = CGWindowListCreateDescriptionFromArray(idArray);
    CFRelease(idArray);
    return result;
}
QList<WId> WindowUtils::windows()
{
    /*  enum
        {
           kCGWindowListOptionAll                 = 0,
           kCGWindowListOptionOnScreenOnly        = (1 << 0), <----
           kCGWindowListOptionOnScreenAboveWindow = (1 << 1),
           kCGWindowListOptionOnScreenBelowWindow = (1 << 2),
           kCGWindowListOptionIncludingWindow     = (1 << 3),
           kCGWindowListExcludeDesktopElements    = (1 << 4)  <----
        }
    */
    QList<WId> result;
    CFArrayRef array = CGWindowListCreate(kCGWindowListOptionAll | kCGWindowListExcludeDesktopElements | kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
    //qDebug() << "Found " << CFArrayGetCount(array) << " windows";
    for (int i = 0; i < CFArrayGetCount(array); i++) {
        // what type is this?
        result << (CGWindowID) (uintptr_t) CFArrayGetValueAtIndex(array, i);
    }
    CFRelease(array);
    return result;
}
QString WindowUtils::windowTitle(WId id)
{
    // http://stackoverflow.com/questions/1606321/how-do-i-get-a-list-of-the-window-titles-on-the-mac-osx
    // kCGWindowOwnerName for application name; kCGWindowName for window title however seldom set.
    //CFArrayRef array = CGWindowListCopyWindowInfo(); // expensive
    //CFArrayRef infoArray = CGWindowListCreateDescriptionFromArray
    CFArrayRef windows = createWindowDescription(id);
    CFDictionaryRef window = (CFDictionaryRef) CFArrayGetValueAtIndex(windows, 0);
    CFStringRef nameRef = (CFStringRef) CFDictionaryGetValue(window, kCGWindowName);
    if (!nameRef) nameRef = (CFStringRef) CFDictionaryGetValue(window, kCGWindowOwnerName);
    QString result = toQString(nameRef);
    if (result.isEmpty()) result = QString::number(id);
    CFRelease(windows);
    return result;
}
bool WindowUtils::isValid(WId id)
{
    bool result;
    CFArrayRef match = createWindowDescription(id);
    if (match == NULL) return false;
    if (CFArrayGetCount(match) == 0) result = false;
    else result = true;
    CFRelease(match);
    return result;
}
QRect WindowUtils::windowGeometry(WId id)
{
    CGRect rect = CGRectMake(0, 0, 100, 100); // default
    CGWindowID idCArray[1] = { id };
    CFArrayRef idArray = CFArrayCreate(NULL, (const void **) idCArray, 1, NULL);
    CFArrayRef descArray = CGWindowListCreateDescriptionFromArray(idArray);
    CFDictionaryRef description = (CFDictionaryRef) CFArrayGetValueAtIndex(descArray, 0);
    if (CFDictionaryContainsKey(description, kCGWindowBounds)) { // always true?
        CFDictionaryRef bounds = (CFDictionaryRef) CFDictionaryGetValue(description, kCGWindowBounds);
        if (bounds) {
            CGRectMakeWithDictionaryRepresentation(bounds, &rect);
        }
    }
    QRect result(CGRectGetMinX(rect), CGRectGetMinY(rect),
                 CGRectGetWidth(rect), CGRectGetHeight(rect));
    CFRelease(descArray);
    CFRelease(idArray);
    return result;
}
QPixmap WindowUtils::grabWindow(WId id)
{
    QRect r = windowGeometry(id);
    WId d = QApplication::desktop()->winId();
    return QPixmap::grabWindow(d, r.x(), r.y(), r.width(), r.height());
}
#else
#error "Operating system not supported."
#endif



