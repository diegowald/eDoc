#ifndef EDOCTCPCLIENT_GLOBAL_H
#define EDOCTCPCLIENT_GLOBAL_H

#include <QtGlobal>

#if defined(EDOCTCPCLIENT_LIBRARY)
#  define EDOCTCPCLIENTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EDOCTCPCLIENTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EDOCTCPCLIENT_GLOBAL_H

