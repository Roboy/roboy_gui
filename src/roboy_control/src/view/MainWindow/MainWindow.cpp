#include "MainWindow.h"
#include "ui_MainWindow.h"

/**
 * @brief MainWindow::MainWindow cosntructor
 * @param modelService modelService from which the RoboyBehaviors are retrieved
 * @param pViewController controller of the ui component
 * @param parent non mandatory parent for the MainWindow
 */
MainWindow::MainWindow(IModelService *modelService, ViewController * pViewController, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) 
{
    modelService->subscribe(this);
    this->ui->setupUi(this);
    this->mainTabWidget = new QTabWidget();
    this->playerView = new PlayerView(modelService, pViewController);
    this->recorderView = new RecorderView(modelService);
    this->editorView = new EditorView(modelService);
    this->setContentsMargins(9,9,9,9);
    this->setCentralWidget(mainTabWidget);
    this->mainTabWidget->addTab(playerView, "player");
    this->mainTabWidget->addTab(recorderView, "recorder");
    this->mainTabWidget->addTab(editorView, "editor");
}

/**
 * @brief MainWindow::~MainWindow destructor
 */
MainWindow::~MainWindow() 
{
	delete this->playerView;
	delete this->recorderView;
	delete this->editorView;
	delete this->mainTabWidget;
    delete this->ui;
}

/**
 * @brief MainWindow::notify method to notify about data changes implemented from IObserver interface
 */
void MainWindow::notify() 
{
    qDebug() << "MainViewController: Notified on data changed";
    this->playerView->notify();
	this->recorderView->notify();
	this->editorView->notify();
}

