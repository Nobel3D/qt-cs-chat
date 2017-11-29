#ifndef LIBCS_GLOBAL_H
#define LIBCS_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QObject>

#define NET_TIMEOUT 10000

#if defined(LIBCS_LIBRARY)
#  define LIBCSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBCSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBCS_GLOBAL_H
