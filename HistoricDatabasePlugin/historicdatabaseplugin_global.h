#ifndef HISTORICDATABASEPLUGIN_GLOBAL_H
#define HISTORICDATABASEPLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HISTORICDATABASEPLUGIN_LIBRARY)
#  define HISTORICDATABASEPLUGINSHARED_EXPORT Q_DECL_EXPORT
#else
#  define HISTORICDATABASEPLUGINSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // HISTORICDATABASEPLUGIN_GLOBAL_H