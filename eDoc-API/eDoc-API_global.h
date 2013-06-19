#ifndef EDOCAPI_GLOBAL_H
#define EDOCAPI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EDOCAPI_LIBRARY)
#  define EDOCAPISHARED_EXPORT Q_DECL_EXPORT
#else
#  define EDOCAPISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EDOCAPI_GLOBAL_H
