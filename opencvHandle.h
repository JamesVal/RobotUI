
#ifndef _OPENCVHANDLE_H
#define _OPENCVHANDLE_H

#include <QObject>
#include <QPixmap>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv/cv.h"

class OPENCV_HANDLER : public QObject
{
  Q_OBJECT

public:
  void stopStream();

public slots:
  void process();

signals:
  void finished();
  void error(QString err);
  void imageReady(QPixmap curImage);

private:
  int playVideoLoop;
};


#endif

