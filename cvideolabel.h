#ifndef VIDEOLABEL_H
#define VIDEOLABEL_H

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QThread>

#include "opencvHandle.h"

class cVideoLabel : public QLabel
{
  Q_OBJECT
public:
  explicit cVideoLabel(QWidget *parent = 0);

  enum keyState {
    keyReleased,
    keyPressed
  };

  int xCoord;
  int yCoord;
signals:
  void leftClick(void);
  void rightClick(void);
  void leftRelease(void);
  void rightRelease(void);
  void mouseMoving(void);
  void forwardKeyPressed(void);
  void forwardKeyReleased(void);
  void backwardKeyPressed(void);
  void backwardKeyReleased(void);
  void leftKeyPressed(void);
  void leftKeyReleased(void);
  void rightKeyPressed(void);
  void rightKeyReleased(void);

public slots:
  void startStreaming(void);
  void stopStreaming(void);

private:
  int xInitialCoord;
  int yInitialCoord;
  Qt::Key moveForwardKey;
  Qt::Key moveBackwardKey;
  Qt::Key moveLeftKey;
  Qt::Key moveRightKey;
  QThread* thread;
  OPENCV_HANDLER* openCVObj; 

private slots:
  void updateImage(QPixmap curImage);
  void videoError(QString errorStr);

protected:
  void mousePressEvent(QMouseEvent *ev);
  void mouseMoveEvent(QMouseEvent *ev);
  void mouseReleaseEvent(QMouseEvent *ev);
  void keyPressEvent(QKeyEvent *ev);
  void keyReleaseEvent(QKeyEvent *ev);
};

#endif // VIDEOLABEL_H
