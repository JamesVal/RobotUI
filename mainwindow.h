#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QThread>

#include <stdio.h>
#include <string>
#include <list>

#include "tcpsocket.h"
#include "commandHandle.h"
#include "opencvHandle.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv/cv.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addDebug(char *dbgStr);
    int verifyIPAddr(char *IPADDR);
    QTcpSocket curSocket;

signals:
    void startStream(void);
    void stopStream(void);

private slots:
    void on_actionAdd_IP_Address_triggered();
    void on_actionSend_Data_triggered();
    void socketOnConnect(void);
    void socketReadyRead(void);

    void on_actionPlayVideo_triggered();
    void on_actionStopVideo_triggered();

    void videoLeftClick(void);
    void videoRightClick(void);
    void videoLeftRelease(void);
    void videoRightRelease(void);
    void videoMouseMoving(void);
    void videoForwardKeyPressed(void);
    void videoForwardKeyReleased(void);
    void videoBackwardKeyPressed(void);
    void videoBackwardKeyReleased(void);
    void videoLeftKeyPressed(void);
    void videoLeftKeyReleased(void);
    void videoRightKeyPressed(void);
    void videoRightKeyReleased(void);

private:
    Ui::MainWindow *ui;
    std::vector<unsigned char> pollData;
    std::vector<unsigned char> rcvData;
    std::vector<unsigned char> bigBuffer;
    COMMAND_HANDLER commandHandler;

};



#endif // MAINWINDOW_H
