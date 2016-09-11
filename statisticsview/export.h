#ifndef EXPORT_H
#define EXPORT_H

#include <QtCore/QtGlobal>

#if defined(STATISTIC_VIEW_LIBRARY)
#  define STATISTIC_VIEW_EXPORT Q_DECL_EXPORT
#else
#  define STATISTIC_VIEW_EXPORT Q_DECL_IMPORT
#endif

#endif // EXPORT_H
