#ifndef ROBOYVIEW_H
#define ROBOYVIEW_H

#include <QObject>
#include <QQmlApplicationEngine>

#include "../BehaviorListModel.h"
#include "ControllerStateListModel.h"
#include "DataTypes.h"
#include "IModelService.h"
#include "IObserver.h"
#include "ViewController.h"

class ViewController;

class RoboyView : public QObject, public IObserver {
    
    Q_OBJECT

public:
    explicit RoboyView(IModelService * pModelService, ViewController * pViewController, QQmlApplicationEngine * pAppEngine, QObject * pParent = 0);
    ~RoboyView();

    void notify();
    void controllerStatusUpdated(qint32 motorId, ControllerState state);
    void dataPoolReset();

    Q_INVOKABLE void initalizeButtonClicked();

private:
    ControllerStateListModel * m_pStateModel;
	ViewController           * m_pViewController;
    IModelService            * m_pModelService;
    QQmlApplicationEngine    * m_pAppEngine;

};

#endif // ROBOYVIEW_H