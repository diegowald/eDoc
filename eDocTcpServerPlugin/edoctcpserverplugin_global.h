#ifndef EDOCTCPSERVERPLUGIN_GLOBAL_H
#define EDOCTCPSERVERPLUGIN_GLOBAL_H

#include <QtGlobal>

#if defined(EDOCTCPSERVERPLUGIN_LIBRARY)
#  define EDOCTCPSERVERPLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define EDOCTCPSERVERPLUGIN_EXPORT Q_DECL_IMPORT
#endif

#endif // EDOCTCPSERVERPLUGIN_GLOBAL_H
