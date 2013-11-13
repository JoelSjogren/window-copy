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
 *
*/
//#include </System/Library/Frameworks/ApplicationServices.framework/Frameworks/CoreGraphics.framework/Headers/CGWindow.h>
#include <ApplicationServices/ApplicationServices.h>
QList<WId> WindowUtils::windows()
{
    QList<WId> result;
    CFArrayRef array = CGWindowListCreate(kCGWindowListOptionAll, kCGNullWindowID);
    qDebug() << "Found " << CFArrayGetCount(array) << " windows";
    for (int i = 0; i < CFArrayGetCount(array); i++) {
        // what type is this?
        result << *((CGWindowID *) CFArrayGetValueAtIndex(array, i));
    }
    return result;
}
QString WindowUtils::windowTitle(WId id)
{
    //CFArrayRef array = CGWindowListCopyWindowInfo(); // expensive
    CFArrayRef infoArray = CGWindowListCreateDescriptionFromArray
}
bool WindowUtils::isValid(WId id)
{
    return true;
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
    return result;
}
#else
#error "Operating system not supported."
#endif






