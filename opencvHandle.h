
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
  explicit OPENCV_HANDLER(void);
  void startStream();
  void stopStream();

public slots:
  void process();

signals:
  void finished();
  void error(QString err);
  void imageReady(QPixmap curImage);

private:
  typedef enum {
    WAIT_FOR_START_CMD,
    INITIALIZE_CAM,
    GET_FRAME,
    CHECK_STOP,
    CLEAR_INTERFACE,
    ERROR_STATE
  } OPEN_CV_STATE;

  OPEN_CV_STATE curState;

  int playVideoLoop;

};


#endif

