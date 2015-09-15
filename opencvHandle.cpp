
#include <iostream>
#include <stdio.h>

#include "opencvHandle.h"

OPENCV_HANDLER::OPENCV_HANDLER(void)
{
  this->curState = WAIT_FOR_START_CMD;
  this->playVideoLoop = 0;
}

void OPENCV_HANDLER::process()
{
  cv::Mat incFrame, newFrame;
  QPixmap imgToShow;
  int w,h;
  cv::VideoCapture *videoStream;

  for (;;)
  {
    switch ( this->curState )
    {
      case WAIT_FOR_START_CMD:
        if (this->playVideoLoop) this->curState = INITIALIZE_CAM;
        break;

      case INITIALIZE_CAM:
        videoStream = new cv::VideoCapture(0);

        if ( !videoStream->isOpened() )
        {
            printf("Capture failed!");
            delete videoStream;
            emit error("No capture");
            this->curState = ERROR_STATE;
        }
        this->curState = GET_FRAME;
        break;

      case GET_FRAME:
        *videoStream >> incFrame;

        if ( !incFrame.empty() )
        {
          //Convert the Mat object to something QT understands
          cv::cvtColor(incFrame,newFrame,CV_BGR2RGB);
          imgToShow = QPixmap::fromImage(QImage((unsigned char*) newFrame.data, newFrame.cols, newFrame.rows, QImage::Format_RGB888));
          emit imageReady(imgToShow);
          incFrame.release();
        }

        this->curState = CHECK_STOP;
        break;

      case CHECK_STOP:
        if (this->playVideoLoop == 0) this->curState = CLEAR_INTERFACE;
        else this->curState = GET_FRAME;
        break;

      case CLEAR_INTERFACE:
        videoStream->release();
        delete videoStream;
        this->curState = WAIT_FOR_START_CMD;
        break;

      case ERROR_STATE:
        this->playVideoLoop = 0;
        break;
    }
  }

  emit finished();
}

void OPENCV_HANDLER::startStream(void)
{
    this->playVideoLoop = 1;
}

void OPENCV_HANDLER::stopStream(void)
{
    this->playVideoLoop = 0;
}
