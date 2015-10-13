#ifndef LIBQMCV_GLOBAL_H
#define LIBQMCV_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBQMCV_LIBRARY)
#  define LIBQMCVSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBQMCVSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBQMCV_GLOBAL_H
