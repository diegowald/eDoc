#ifndef EDOCTCPMESSAGES_GLOBAL_H
#define EDOCTCPMESSAGES_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EDOCTCPMESSAGES_LIBRARY)
#  define EDOCTCPMESSAGESSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EDOCTCPMESSAGESSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EDOCTCPMESSAGES_GLOBAL_H
