#include "tcpsocket.h"

TCPSocket::TCPSocket()
{
  this->curSocket = new QTcpSocket();
  connect(this->curSocket, SIGNAL(connected()), this, SLOT(socketOnConnect()));
  connect(this->curSocket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
}

int TCPSocket::ConnectToIPAddress(QString newIP)
{

  QHostAddress myHost(newIP.toStdString().c_str());

  char tempBfr[64];

  if ( this->curSocket->state() != QAbstractSocket::ConnectedState )
  {
    this->curSocket->connectToHost(myHost, 9000);
  }
}

void TCPSocket::socketOnConnect(void)
{
  //addDebug("Connected!");
}

void TCPSocket::socketReadyRead(void)
{
    int bytesRead;
    char tempStr[10];
    std::string debugStr;
    std::vector<unsigned char>::iterator it;
    int responseRes;
    int w,h;

    this->rcvData.resize(11000,0);
    //addDebug("Rx Data Ready");
    while ( this->curSocket->bytesAvailable() > 0 )
    {
        bytesRead = this->curSocket->read((char*)this->rcvData.data(), this->rcvData.size());
        this->rcvData.resize(bytesRead);
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
        /*
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
        */
    }
}

//JJV DEBUG - will need something to close connections

