#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    COMMAND_HANDLER testCmdHandler;

    ui->setupUi(this);

    //Initialize all necessary variables
    ui->conIPBtn->setDefaultAction(ui->actConIPAddr);
    ui->sendDataBtn->setDefaultAction(ui->actSendData);
    ui->playVideoBtn->setDefaultAction(ui->actPlayVideo);
    ui->stopVideoBtn->setDefaultAction(ui->actStopVideo);

    //Setup signals and slots for objects not on the screen!
    QObject::connect(ui->videoLabel, SIGNAL(leftClick()), this, SLOT(videoLeftClick()));
    QObject::connect(ui->videoLabel, SIGNAL(rightClick()), this, SLOT(videoRightClick()));
    QObject::connect(ui->videoLabel, SIGNAL(leftRelease()), this, SLOT(videoLeftRelease()));
    QObject::connect(ui->videoLabel, SIGNAL(rightRelease()), this, SLOT(videoRightRelease()));
    QObject::connect(ui->videoLabel, SIGNAL(mouseMoving()), this, SLOT(videoMouseMoving()));
    QObject::connect(ui->videoLabel, SIGNAL(forwardKeyPressed()), this, SLOT(videoForwardKeyPressed()));
    QObject::connect(ui->videoLabel, SIGNAL(forwardKeyReleased()), this, SLOT(videoForwardKeyReleased()));
    QObject::connect(ui->videoLabel, SIGNAL(backwardKeyPressed()), this, SLOT(videoBackwardKeyPressed()));
    QObject::connect(ui->videoLabel, SIGNAL(backwardKeyReleased()), this, SLOT(videoBackwardKeyReleased()));
    QObject::connect(ui->videoLabel, SIGNAL(leftKeyPressed()), this, SLOT(videoLeftKeyPressed()));
    QObject::connect(ui->videoLabel, SIGNAL(leftKeyReleased()), this, SLOT(videoLeftKeyReleased()));
    QObject::connect(ui->videoLabel, SIGNAL(rightKeyPressed()), this, SLOT(videoRightKeyPressed()));
    QObject::connect(ui->videoLabel, SIGNAL(rightKeyReleased()), this, SLOT(videoRightKeyReleased()));
    QObject::connect(ui->actPlayVideo,SIGNAL(triggered()),ui->videoLabel,SLOT(startStreaming()));
    QObject::connect(ui->actStopVideo,SIGNAL(triggered()),ui->videoLabel,SLOT(stopStreaming()));

    //JJV DEBUG - There seems to be a bug of some sorts with using OpenCV in Qt where I MUST do a dummy call to cvtColor() before it actually works properly
    cv::Mat dummyFrame(10,10,CV_8UC3, cv::Scalar(0,0,0));
    cv::cvtColor(dummyFrame,dummyFrame,CV_BGR2RGB);

    testCmdHandler.UnitTests();
    fflush(stdout);//Force flush of debug
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addDebug(char *dbgStr)
{
    ui->dbgTextEdit->append(dbgStr);
}

void MainWindow::on_actConIPAddr_triggered()
{
  addDebug("Connect to IP Address");
}

void MainWindow::on_actSendData_triggered()
{
  addDebug("Send Data");
/*
    if ( curSocket.state() == QAbstractSocket::ConnectedState )
    {
        commandHandler.BuildPoll(CMD_TESTIMG, pollData);
        curSocket.write((char*)pollData.data(), pollData.size());
        addDebug("Send Test Poll");
    }
    else
    {
        addDebug("No Socket!!!");
    }
*/
}

void MainWindow::on_actPlayVideo_triggered()
{ 
  addDebug("Start video");
}

void MainWindow::on_actStopVideo_triggered()
{
  addDebug("Stop video");
}

void MainWindow::videoLeftClick(void)
{
  char tempBfr[64];
  sprintf(tempBfr,"Left Click %d %d",ui->videoLabel->xCoord,ui->videoLabel->yCoord);
  addDebug(tempBfr);

  //JJV DEBUG - DONT NEED THIS
  sprintf(tempBfr,"Widget Dimensions %d %d",ui->videoLabel->width(),ui->videoLabel->height());
  addDebug(tempBfr);
}

void MainWindow::videoRightClick(void)
{
  char tempBfr[64];
  sprintf(tempBfr,"Right Click %d %d",ui->videoLabel->xCoord,ui->videoLabel->yCoord);
  addDebug(tempBfr);
}

void MainWindow::videoLeftRelease(void)
{
  char tempBfr[64];
  sprintf(tempBfr,"Left Release %d %d",ui->videoLabel->xCoord,ui->videoLabel->yCoord);
  addDebug(tempBfr);
}

void MainWindow::videoRightRelease(void)
{
  char tempBfr[64];
  sprintf(tempBfr,"Right Release %d %d",ui->videoLabel->xCoord,ui->videoLabel->yCoord);
  addDebug(tempBfr);
}

void MainWindow::videoMouseMoving(void)
{
  char tempBfr[64];
  sprintf(tempBfr,"Mouse Moving %d %d",ui->videoLabel->xCoord,ui->videoLabel->yCoord);
  addDebug(tempBfr);
}

void MainWindow::videoForwardKeyPressed(void)
{
  //JJV DEBUG - We'll probably need an event filter of some sort to unfocus the screen if anything else is clicked on the app
  addDebug("Forward Key Pressed");
}

void MainWindow::videoForwardKeyReleased(void)
{
  addDebug("Forward Key Released");
}

void MainWindow::videoBackwardKeyPressed(void)
{
  addDebug("Backward Key Pressed");
}

void MainWindow::videoBackwardKeyReleased(void)
{
  addDebug("Backward Key Released");
}

void MainWindow::videoLeftKeyPressed(void)
{
  addDebug("Left Key Pressed");
}

void MainWindow::videoLeftKeyReleased(void)
{
  addDebug("Left Key Released");
}

void MainWindow::videoRightKeyPressed(void)
{
  addDebug("Right Key Pressed");
}

void MainWindow::videoRightKeyReleased(void)
{
  addDebug("Right Key Released");
}


