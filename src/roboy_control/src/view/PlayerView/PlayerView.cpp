#include "PlayerView.h"
#include "ui_PlayerView.h"

#include "ViewController.h"

#include "DataTypes.h"

#include "MultiLaneView/MultiLaneView.h"
#include "MultiLaneView/RoboyMultiLaneModel.h"

/**
* @brief constructor
* @param the IModelService providing the roboy behaviors
* @param non mandatory parent
*/
PlayerView::PlayerView(IModelService *modelService, ViewController * pViewController, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerView)
{
    m_pViewController = pViewController;

	this->ui->setupUi(this);
    this->modelService = modelService;
    this->behaviorListModel = new BehaviorListModel(this->modelService);
    this->ui->behaviorListView->setModel(this->behaviorListModel);
    this->setupConnections();

    this->multiLaneModel = new RoboyMultiLaneModel();
    this->ui->multiLaneView->setModel(this->multiLaneModel);

    this->multiLaneModel->addLane();
    this->multiLaneModel->addLane();
    this->multiLaneModel->addLane();

    QList<RoboyBehaviorMetadata> metaData = modelService->getBehaviorList();

    for (int i = 0; i < metaData.count(); i++) {
        RoboyBehavior behavior = modelService->retrieveRoboyBehavior(metaData.at(i));
        this->multiLaneModel->insertBehaviorExec(i, i*50, behavior);
    }
}

/**
*@brief desctructor
**/
PlayerView::~PlayerView()
{
    delete this->ui;
    delete this->behaviorListModel;
}

/**
*@brief method to notify about data changes implemented from IObserver interface
**/
void PlayerView::notify() 
{
	this->behaviorListModel->notify();
}

/* ui connections */

/**
*@brief method to connect all ui controls to their handler functions
**/
void PlayerView::setupConnections()
{
	/* buttons */
	QObject::connect(ui->playButton, SIGNAL(clicked()), this, SLOT(playButtonClicked()));
	QObject::connect(ui->pauseButton, SIGNAL(clicked()), this, SLOT(pauseButtonClicked()));
	QObject::connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stopButtonClicked()));
	QObject::connect(ui->skipButton, SIGNAL(clicked()), this, SLOT(skipButtonClicked()));
	/* behavior listview */
	connect(ui->behaviorListView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showBehaviorListItemMenu(const QPoint&)));
    QObject::connect(ui->behaviorListView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(behaviorListViewCurrentRowChanged(const QModelIndex &)));

}

/**
*@brief click handler for the play button
**/
void PlayerView::playButtonClicked()
{
	m_pViewController->playBehaviorPlan();
}

/**
*@brief click handler for the pause button
**/
void PlayerView::pauseButtonClicked()
{
	
}

/**
*@brief click handler for the stop button
**/
void PlayerView::stopButtonClicked()
{
	
}

/**
*@brief click handler for the skip button
**/
void PlayerView::skipButtonClicked()
{
	
}

/**
*@brief method for filling the behaviorDetailView with information about the selected behavior
*@param index index of the currently selected behavior in the behaviorListView
**/
void PlayerView::behaviorListViewCurrentRowChanged(const QModelIndex & index)
{	
	this->currentlyDisplayedBehaviorMetaData = this->behaviorListModel->getBehaviorMetaData(index.row());
	this->currentlyDisplayedBehavior = this->modelService->retrieveRoboyBehavior(this->currentlyDisplayedBehaviorMetaData);
	this->ui->addToQueueButton->setEnabled(true);
	ui->behaviorNameValueLabel->setText(this->currentlyDisplayedBehavior.m_metadata.m_sBehaviorName);
	ui->idValueLabel->setText(QString::number(this->currentlyDisplayedBehavior.m_metadata.m_ulBehaviorId));
	ui->motorCountValueLabel->setText(QString::number(this->currentlyDisplayedBehavior.m_mapMotorWaypoints.count()));

	ui->motorListView->clear();
	QString description;
	for(u_int32_t iterator : this->currentlyDisplayedBehavior.m_mapMotorWaypoints.keys())
	{
		ui->motorListView->addItem(QString("MOTOR ID %1 WAYPOINT COUNT %2").arg(iterator).arg(this->currentlyDisplayedBehavior.m_mapMotorWaypoints.value(iterator).count()));
	}
}

/**
*@brief
**/
void PlayerView::showBehaviorListItemMenu(const QPoint& pos)
{
    QPoint globalPos = ui->behaviorListView->mapToGlobal(pos);
    QModelIndex selectedIndex = ui->behaviorListView->indexAt(pos);
    
    if (selectedIndex.isValid()) {
    	QMenu behaviorListItemMenu;
        QAction addAction(QIcon(":/add-img.png"), "add to queue", NULL);
        addAction.setIconVisibleInMenu(true);
        behaviorListItemMenu.addAction(&addAction);
    	QAction *selectedItem = behaviorListItemMenu.exec(globalPos);

    	if (selectedItem) {
    		if (selectedItem->text() == "add to queue") {
                // add item to multi lane view here
            }
		}
	}
}
