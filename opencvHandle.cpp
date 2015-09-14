
#include <iostream>
#include <stdio.h>

#include "opencvHandle.h"

//JJV DEBUG - REWRITE THIS PROCESS

void OPENCV_HANDLER::process()
{
  cv::Mat incFrame, newFrame;
  QPixmap imgToShow;
  int w,h;
  cv::VideoCapture *videoStream;

  printf("Play Video");

  videoStream = new cv::VideoCapture(0);

  if ( !videoStream->isOpened() )
  {
      printf("Capture failed!");
      delete videoStream;
      emit error("No capture");
      return;
  }

  playVideoLoop = 1;

  while (playVideoLoop)
  {
    //This seems to be really slow and the interface because unresponsive..we'll probably have to limit these calls a little bit
    *videoStream >> incFrame;

    if ( !incFrame.empty() )
    {
      //Convert the Mat object to something QT understands
      cv::cvtColor(incFrame,newFrame,CV_BGR2RGB);
      imgToShow = QPixmap::fromImage(QImage((unsigned char*) newFrame.data, newFrame.cols, newFrame.rows, QImage::Format_RGB888));
      emit imageReady(imgToShow);
      incFrame.release();
    }
  }

  videoStream->release();
  delete videoStream;

  printf("End Video");

  emit finished();
}

void OPENCV_HANDLER::stopStream(void)
{
    playVideoLoop = 0;
}
