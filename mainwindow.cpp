#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    COMMAND_HANDLER testCmdHandler;

    ui->setupUi(this);

    //Initialize all necessary variables
    ui->addIPBtn->setDefaultAction(ui->actionAdd_IP_Address);
    ui->sendDataBtn->setDefaultAction(ui->actionSend_Data);
    ui->playVideoBtn->setDefaultAction(ui->actionPlayVideo);
    ui->stopVideoBtn->setDefaultAction(ui->actionStopVideo);

    //Setup signals and slots for objects not on the screen!
    QObject::connect(&curSocket, SIGNAL(connected()), this, SLOT(socketOnConnect()));
    QObject::connect(&curSocket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
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
    QObject::connect(this, SIGNAL(startStream()), ui->videoLabel, SLOT(startStreaming()));
    QObject::connect(this, SIGNAL(stopStream()), ui->videoLabel, SLOT(stopStreaming()));

    //JJV DEBUG - There seems to be a bug of some sorts with using openCV in Qt where I MUST do a dummy call to cvtColor() before it actually works properly
    cv::Mat dummyFrame(10,10,CV_8UC3, cv::Scalar(0,0,0));
    cv::cvtColor(dummyFrame,dummyFrame,CV_BGR2RGB);

    testCmdHandler.UnitTests();
    fflush(stdout);//Force flush of debug
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::verifyIPAddr(char *IPADDR)
{
    int ret = 0;



    //JJV DEBUG FOR NOW
    ret = 1;
    return (ret);
}

void MainWindow::addDebug(char *dbgStr)
{
    ui->dbgTextEdit->append(dbgStr);
}

void MainWindow::socketOnConnect(void)
{
    addDebug("Connected!");
}

void MainWindow::on_actionAdd_IP_Address_triggered()
{
    //QHostAddress myHost(dbgStr);
    QHostAddress myHost("127.0.0.1");//JJV DEBUG

    char tempBfr[64];

    //sprintf(tempBfr, "Connect to IP Address: %s", ui->addIPEdit->text().toStdString().c_str());

    if ( curSocket.state() != QAbstractSocket::ConnectedState )
    {
        sprintf(tempBfr, "Connect to LocalHost"); //JJV DEBUG
        addDebug(tempBfr);
        curSocket.connectToHost(myHost, 9000);
    }
}

void MainWindow::socketReadyRead(void)
{
    int bytesRead;
    char tempStr[10];
    std::string debugStr;
    std::vector<unsigned char>::iterator it;
    int responseRes;
    QPixmap updateImg;
    int w,h;

    rcvData.resize(11000,0);
    //addDebug("Rx Data Ready");
    while ( curSocket.bytesAvailable() > 0 )
    {
        bytesRead = curSocket.read((char*)rcvData.data(), rcvData.size());
        //addDebug("Recevied:");
        rcvData.resize(bytesRead);
/*
        for ( it = rcvData.begin(); it != rcvData.end(); it++ )
        {
            sprintf(tempStr, "[%02X]", *it);
            debugStr.append(tempStr);
        }
        addDebug((char*)debugStr.c_str());
*/
        //sprintf(tempStr, "%d bytes", bytesRead);
        //addDebug(tempStr);

        //Check if this is a valid poll
        responseRes = commandHandler.ParsePoll(rcvData, pollData);
        if ( responseRes > 0 )
        {
            addDebug("validResponse");
            cv::Mat incMat = cv::imdecode(pollData, CV_LOAD_IMAGE_COLOR);
            cv::cvtColor(incMat,incMat,CV_BGR2RGB); //Convert the color scaling
            updateImg = QPixmap::fromImage(QImage((unsigned char*) incMat.data, incMat.cols, incMat.rows, QImage::Format_RGB888));

            //Get the size of the label, we want to adjust the MAT object to match it
            w = ui->videoLabel->width();
            h = ui->videoLabel->height();

            //Display the image
            ui->videoLabel->setPixmap(updateImg.scaled(w,h,Qt::KeepAspectRatio));
        }
        else if ( responseRes == 0 )
        {
            //addDebug("needMoreData");
        }
    }
}

void MainWindow::on_actionSend_Data_triggered()
{
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
}

void MainWindow::on_actionPlayVideo_triggered()
{ 
  addDebug("Start video");
  emit startStream();
}

void MainWindow::on_actionStopVideo_triggered()
{
  addDebug("Stop video");
  emit stopStream();
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


