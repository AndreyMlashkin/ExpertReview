#ifndef EXPERTREVIEW_EXPORT_H
#define EXPERTREVIEW_EXPORT_H

#include <QtCore/QtGlobal>

#if defined(EXPERTREVIEW_LIBRARY)
#  define EXPERTREVIEW_EXPORT Q_DECL_EXPORT
#else
#  define EXPERTREVIEW_EXPORT Q_DECL_IMPORT
#endif

#endif // EXPERTREVIEW_EXPORT_H
