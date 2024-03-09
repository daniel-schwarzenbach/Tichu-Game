#ifndef TICHU_CLIENTNETWORKMANAGER_H
#define TICHU_CLIENTNETWORKMANAGER_H


#include <string>
#include <QObject>
#include "ResponseListenerThread.h"
#include "SignalHandler.h"
#include "../../common/network/requests/client_request.h"


class ClientNetworkManager: public QObject {

    Q_OBJECT

public:
    static void init(const std::string& host, const uint16_t port);

    static void sendRequest(const client_request& request);



private:
    static bool connect(const std::string& host, const uint16_t port);

    static sockpp::tcp_connector* _connection;

    static bool _connectionSuccess;

    static bool _failedToConnect;

    static SignalHandler* _signalHandler;

};


#endif //TICHU_CLIENTNETWORKMANAGER_H
