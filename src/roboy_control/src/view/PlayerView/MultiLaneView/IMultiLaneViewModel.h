#ifndef IMULTILANEVIEWMODEL_H
#define IMULTILANEVIEWMODEL_H

#include <QObject>

#include "DataTypes.h"

class IMultiLaneViewModel : public QObject
{
    Q_OBJECT

public:
    virtual void   initializeWidget   () = 0;
    virtual qint8  addLane            () = 0;
    virtual qint8  insertLane         (qint32 index) = 0;
    virtual qint8  removeLane         (qint32 index) = 0;
    virtual qint8  insertBehaviorExec (qint32 laneIndex, quint64 ulTimestamp, RoboyBehavior behavior) = 0;
    virtual qint8  removeBehaviorExec (qint32 laneIndex, qint32 itemIndex) = 0;
    virtual qint8  removeBehaviorExec (qint32 laneIndex, qint64 lId) = 0;

    virtual qint32   laneCount () = 0;
    virtual qint32   itemCount (qint32 laneIndex) = 0;
    virtual QVariant data      (qint32 laneIndex, qint32 itemIndex, qint32 role) = 0;

    virtual RoboyBehaviorPlan getBehaviorPlan () = 0;

signals:
    void laneInserted   (qint32 index);
    void laneRemoved    (qint32 index);
    void itemInserted   (qint32 laneIndex, qint32 itemIndex);
    void itemRemoved    (qint32 laneIndex, qint32 itemIndex);
};

Q_DECLARE_INTERFACE(IMultiLaneViewModel, "IMultiLaneViewModel")

#endif // IMULTILANEVIEWMODEL_H
