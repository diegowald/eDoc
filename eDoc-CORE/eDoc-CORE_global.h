#ifndef EDOCCORE_GLOBAL_H
#define EDOCCORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EDOCCORE_LIBRARY)
#  define EDOCCORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define EDOCCORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EDOCCORE_GLOBAL_H
