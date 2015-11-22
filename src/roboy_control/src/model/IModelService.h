#ifndef IMODELSERVICE_H
#define IMODELSERVICE_H

#include "../DataTypes.h"

class IModelService {

public:
    virtual void persistNewRoboyBehavior( const RoboyBehavior behavior ) = 0;
    virtual QList<RoboyBehaviorMetadata> getBehaviorList() = 0;
    virtual RoboyBehavior getBehavior( const RoboyBehaviorMetadata metadata ) = 0;
};

#endif // IMODELSERVICE_H
