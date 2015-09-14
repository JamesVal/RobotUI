#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QtNetwork/QTcpSocket>
#include <QHostAddress>

class TCPSocket
{
public:
    TCPSocket();
    QTcpSocket curSocket;
};

#endif // TCPSOCKET_H
