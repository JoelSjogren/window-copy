#include "windowutils.h"
bool WindowUtils::isValid(WId id)
{
    return !windowGeometry(id).isNull();
}
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
QRect WindowUtils::windowGeometry(WId id)
{
    return QxtWindowSystem::windowGeometry(id);
}
#elif defined(__APPLE__)
/* Resources:
 *  http://stackoverflow.com/questions/2919629/mac-os-x-linker-error-in-qt-coregraphics-cgwindowlistcreate
 *  https://developer.apple.com/library/mac/documentation/corefoundation/Reference/CFArrayRef/Reference/reference.html#//apple_ref/c/func/CFArrayGetTypeID
 * CF = CoreFoundation
 * CG = CoreGraphics -> Quartz
*/
//#include </System/Library/Frameworks/ApplicationServices.framework/Frameworks/CoreGraphics.framework/Headers/CGWindow.h>
#include <ApplicationServices/ApplicationServices.h>
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
    CFArrayRef array = CGWindowListCreate(kCGWindowListOptionAll, kCGNullWindowID);
    //qDebug() << "Found " << CFArrayGetCount(array) << " windows";
    for (int i = 0; i < CFArrayGetCount(array); i++) {
        // what type is this?
        result << (CGWindowID) CFArrayGetValueAtIndex(array, i);
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
    CFStringRef titleRef = CFDictionaryGetValue(window, kCGWindowOwnerName);
    QString result = titleRef ? toQString(titleRef) : "Unknown application name";
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
    if (CFDescriptionContainsKey(description, kCGWindowBounds)) { // always true?
        CFDectionaryRef bounds = (CFDictionaryRef) CFDictionaryGetValue(description, kCGWindowBounds);
        if (bounds) {
            CGRectMakeWithDectionaryRepresentation(bounds, &rect);
        }
    }
    QRect result(CGRectGetMinX(rect), CGRectGetMinY(rect),
                 CGRectGetWidth(rect), CGRectGetHeight(rect));
    CFRelease(descArray);
    CFRelease(idArray);
    return result;
}
#else
#error "Operating system not supported."
#endif






