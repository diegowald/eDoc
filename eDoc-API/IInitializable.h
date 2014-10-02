#ifndef IINITIALIZABLE_H
#define IINITIALIZABLE_H

#include "edocapi_global.h"
#include "forward.h"
#include "../eDoc-Configuration/IXMLContent.h"
#include "../eDoc-Configuration/qobjectlgging.h"

class IInitializable {
public:
    virtual void initialize(IXMLContentPtr configuration, IFactory* factory) = 0;

    virtual ~IInitializable() {}
};

#endif // IINITIALIZABLE_H
