#ifndef TICHU_CLIENT_NETWORK_THREAD_H
#define TICHU_CLIENT_NETWORK_THREAD_H

#include <functional>
#include <QThread>
#include <QDebug>

#include "sockpp/tcp_socket.h"
#include "sockpp/tcp_connector.h"


class ResponseListenerThread : public QThread {

    Q_OBJECT

public:
    ResponseListenerThread(sockpp::tcp_connector* connection);
    ~ResponseListenerThread();

    void run();

private:
    sockpp::tcp_connector* _connection;

signals:
    void outputError(std::string title, std::string message);
    void ouputServerMessage(std::string message);

};

#endif //TICHU_CLIENT_NETWORK_THREAD_H
