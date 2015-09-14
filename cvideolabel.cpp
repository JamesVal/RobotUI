
#include "cvideolabel.h"

cVideoLabel::cVideoLabel(QWidget *parent) :
  QLabel(parent)
{
  setFocusPolicy(Qt::ClickFocus);

  //JJV DEBUG - hardcoded stuff
  this->moveForwardKey = Qt::Key_E;
  this->moveBackwardKey = Qt::Key_D;
  this->moveLeftKey = Qt::Key_S;
  this->moveRightKey = Qt::Key_F;
}

void cVideoLabel::updateImage(QPixmap curImage)
{
  int w = this->width();
  int h = this->height();
  this->setPixmap(curImage.scaled(w,h,Qt::KeepAspectRatio));
}

void cVideoLabel::videoError(QString errorStr)
{
}

void cVideoLabel::mousePressEvent(QMouseEvent *ev)
{
  xCoord = ev->pos().x();
  yCoord = ev->pos().y();
  if (ev->button() == Qt::LeftButton) {
    emit leftClick();
  }
  if (ev->button() == Qt::RightButton) {
    xInitialCoord = ev->pos().x();
    yInitialCoord = ev->pos().y();
    setCursor(Qt::BlankCursor);
    emit rightClick();
  }
}

void cVideoLabel::mouseMoveEvent(QMouseEvent *ev)
{
  xCoord = ev->pos().x();
  yCoord = ev->pos().y();

  //JJV DEBUG - there seems to be an issue here the cursor will appear with negative coordinates - the max positive limits work fine

  if (xCoord > width()) {
    xCoord = width();
  }

  if (xCoord <= 0) {
    xCoord = 0;
  }

  if (yCoord > height()) {
    yCoord = height();
  }

  if (yCoord <= 0) {
    yCoord = 0;
  }

  QCursor::setPos(mapToGlobal(QPoint(xCoord,yCoord)));

  emit mouseMoving();
}

void cVideoLabel::mouseReleaseEvent(QMouseEvent *ev)
{
  xCoord = ev->pos().x();
  yCoord = ev->pos().y();
  if (ev->button() == Qt::LeftButton) {
    emit leftRelease();
  }
  if (ev->button() == Qt::RightButton) {
    QCursor::setPos(mapToGlobal(QPoint(xInitialCoord,yInitialCoord)));
    setCursor(Qt::ArrowCursor);
    emit rightRelease();
  }
}

void cVideoLabel::keyPressEvent(QKeyEvent *ev)
{
  if ((ev->key() == this->moveForwardKey) && !ev->isAutoRepeat()) {
    emit forwardKeyPressed();
  } else if ((ev->key() == this->moveBackwardKey) && !ev->isAutoRepeat()) {
    emit backwardKeyPressed();
  } else if ((ev->key() == this->moveLeftKey) && !ev->isAutoRepeat()) {
    emit leftKeyPressed();
  } else if ((ev->key() == this->moveRightKey) && !ev->isAutoRepeat()) {
    emit rightKeyPressed();
  } else {
    QLabel::keyPressEvent(ev);
  }
}

void cVideoLabel::keyReleaseEvent(QKeyEvent *ev)
{
  if ((ev->key() == this->moveForwardKey) && !ev->isAutoRepeat()) {
    emit forwardKeyReleased();
  } else if ((ev->key() == this->moveBackwardKey) && !ev->isAutoRepeat()) {
    emit backwardKeyReleased();
  } else if ((ev->key() == this->moveLeftKey) && !ev->isAutoRepeat()) {
    emit leftKeyReleased();
  } else if ((ev->key() == this->moveRightKey) && !ev->isAutoRepeat()) {
    emit rightKeyReleased();
  } else {
    QLabel::keyPressEvent(ev);
  }
}

void cVideoLabel::startStream(void)
{
/*
  //Setup the OpenCV thread
  this->thread = new QThread;
  this->openCVObj = new OPENCV_HANDLER();

  this->openCVObj->moveToThread(this->thread);
  connect(this->openCVObj, SIGNAL(error(QString)), this, SLOT(videoError(QString)));
  connect(this->thread, SIGNAL(started()), this->openCVObj, SLOT(process()));
  connect(this->openCVObj, SIGNAL(finished()), this->thread, SLOT(quit()));
  connect(this->openCVObj, SIGNAL(finished()), this->openCVObj, SLOT(deleteLater()));
  connect(this->thread, SIGNAL(finished()), this->thread, SLOT(deleteLater()));

  connect(this->openCVObj, SIGNAL(imageReady(QPixmap)), this, SLOT(updateImage(QPixmap)));

  this->thread->start();
*/
}

void cVideoLabel::stopStream(void)
{
  /*
  this->openCVObj->stopStream();

  //delete this->openCVObj;
  //delete this->thread;
  */
}
