//
// Created by bruh on 1/21/16.
//

#include "MyoController.h"

MyoController::MyoController() {
    MYOCONTROLLER_DBG << "Initialize Myo-Controller";

    // Create Myo-Master Transceiver
    QString name;
    name.sprintf("myomaster");
    m_myoMasterTransceiver = new ROSMessageTransceiverService(0, name);
    m_myoMasterTransceiver->setDelegate(this);
    m_myoMasterTransceiver->start();

    // Create Controller Status Structs
    MYOCONTROLLER_DBG << "Instantiate ROSControllers:";
    QList<ROSController> controllers = RoboyControlConfiguration::instance().getControllersConfig();
    for(ROSController controller : controllers) {
        controller.state = STATUS::UNDEFINED;
        name.clear();
        name.sprintf("motor%i", controller.id);
        controller.transceiver = new ROSMessageTransceiverService(controller.id, name);
        controller.transceiver->setDelegate(this);
        controller.transceiver->start();
        m_mapControllers.insert(controller.id, controller);
        MYOCONTROLLER_DBG << "\t- " << controller.toString();
    }
}

MyoController::~MyoController() {
    delete m_myoMasterTransceiver;
    for(ROSController & controller : m_mapControllers.values())
        delete controller.transceiver;
}

// RoboyController Interface
bool MyoController::initializeControllers() {
    MYOCONTROLLER_DBG << "Send Initialize Request to Myo-Master";
    bool result = false;
    m_myoMasterTransceiver->sendInitializeRequest(m_mapControllers.values().toStdList());

    while(!m_bInitializationComplete){
        m_mutexCVTransceiver.lock();
        m_conditionTransceiver.wait(&m_mutexCVTransceiver);
        m_mutexCVTransceiver.unlock();
    }

    if(isInitializedCorrectly()) {
        MYOCONTROLLER_SUC << "Initialization completed successful" ;
        result = true;
    } else {
        MYOCONTROLLER_WAR << "Initialization failed";
        result = false;
    }

    MYOCONTROLLER_DBG << "Controller States:";
    for(ROSController controller : m_mapControllers.values())
        MYOCONTROLLER_DBG << "\t- " << controller.toString();

    return result;
}

bool MyoController::sendRoboyPlan(RoboyBehaviorPlan & behaviorPlan) {
    bool result = false;
    MYOCONTROLLER_DBG << "Send Behavior Plan";
    MYOCONTROLLER_DBG << "Get Flattended Trajectories";
    QMap<qint32, Trajectory> mapTrajectories = behaviorPlan.getTrajectories();

    for(qint32 id : mapTrajectories.keys()) {
        ROSController & controller = m_mapControllers[id];
        controller.state = STATUS::PREPROCESS_TRAJECTORY;
        controller.transceiver->sendTrajectory(mapTrajectories.value(id));
    }

    while(!m_bReceivedAllControllerStates) {
        m_mutexCVTransceiver.lock();
        m_conditionTransceiver.wait(&m_mutexCVTransceiver);
        m_mutexCVTransceiver.unlock();
    }

    MYOCONTROLLER_DBG << "Received all Controller Status Updates";
    m_bReceivedAllControllerStates = false;

    if(isReadyToPlay(behaviorPlan)) {
        MYOCONTROLLER_SUC << "Plan ready to play.";
        result = true;
        MYOCONTROLLER_DBG << "Send Steering Message: PLAY";
        m_myoMasterTransceiver->sendSteeringMessage(SteeringCommand::PLAY_TRAJECTORY);
    } else {
        MYOCONTROLLER_WAR << "Failed to Transmit Plan. Abort.";
        for(ROSController & controller : m_mapControllers.values())
            controller.state = STATUS::INITIALIZED;
        result = false;
    }
    return result;
}

// ITransceiverServiceDelegate - Callback-Interface Implementation
void MyoController::receivedControllerStatusUpdate(const QList<ROSController> & controllers) {
    for(ROSController controller : controllers) {
        m_mapControllers[controller.id].state = controller.state;
        MYOCONTROLLER_DBG << "Update Controller: " << controller.toString();
    }

    m_mutexCVTransceiver.lock();
    m_bInitializationComplete = true;
    m_conditionTransceiver.wakeAll();
    m_mutexCVTransceiver.unlock();
}

void MyoController::receivedControllerStatusUpdate(const ROSController &controller) {
    MYOCONTROLLER_DBG << "Update Controller: " << controller.toString();
    m_mapControllers[controller.id].state = controller.state;

    if(didReceiveAllStatusUpdates()) {
        m_mutexCVTransceiver.lock();
        m_bReceivedAllControllerStates = true;
        m_conditionTransceiver.wakeAll();
        m_mutexCVTransceiver.unlock();
    }
}

// private
bool MyoController::isInitializedCorrectly() {
    for(ROSController & controller : m_mapControllers.values())
        if (controller.state != STATUS::INITIALIZED)
            return false;

    return true;
}

bool MyoController::didReceiveAllStatusUpdates() {
    for(ROSController & controller : m_mapControllers.values()) {
        if(controller.state == STATUS::PREPROCESS_TRAJECTORY)
            return false;
    }
    return true;
}

bool MyoController::isReadyToPlay(RoboyBehaviorPlan & plan) {
    for(qint32 motorId : plan.getTrajectories().keys()) {
        if(m_mapControllers[motorId].state != STATUS::TRAJECTORY_READY){
            MYOCONTROLLER_WAR << "Cannot run plan: Controller not ready";
            MYOCONTROLLER_WAR << m_mapControllers[motorId].toString();
            return false;
        }
    }
    return true;
}