#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QtNetwork/QTcpSocket>
#include <QHostAddress>
#include <QObject>
#include <QTcpSocket>

class TCPSocket : public QObject
{
public:
  TCPSocket();
  int setIPAddress(QString newIP);

public slots:
  void socketOnConnect(void);
  void socketReadyRead(void);

private:
  QTcpSocket * curSocket;
  std::vector<unsigned char> pollData;
  std::vector<unsigned char> rcvData;

private slots:
};

#endif // TCPSOCKET_H
