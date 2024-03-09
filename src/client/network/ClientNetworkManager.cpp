#include "ClientNetworkManager.h"
#include <QApplication>

#include "../GameController.h"
#include "../../common/network/responses/server_response.h"
#include <QDebug>
#include <sockpp/exception.h>


// initialize static members
sockpp::tcp_connector* ClientNetworkManager::_connection = nullptr;

bool ClientNetworkManager::_connectionSuccess = false;
bool ClientNetworkManager::_failedToConnect = false;

SignalHandler* _signalHandler = nullptr;


void ClientNetworkManager::init(const std::string& host, const uint16_t port) {

    // initialize sockpp framework
    sockpp::socket_initializer sockInit;

    // reset connection status
    ClientNetworkManager::_connectionSuccess = false;
    ClientNetworkManager::_failedToConnect = false;

    // delete exiting connection and create new one
    if(ClientNetworkManager::_connection != nullptr) {
        ClientNetworkManager::_connection->shutdown();
        delete ClientNetworkManager::_connection;
    }
    ClientNetworkManager::_connection = new sockpp::tcp_connector();

    // try to connect to server
    if(ClientNetworkManager::connect(host, port)) {
        GameController::showStatus("Connected to " + host + ":" + std::to_string(port));
        ClientNetworkManager::_connectionSuccess = true;

        qDebug() << "ClientNetworkManager running on Thread:" << QThread::currentThreadId();

        // start network thread
        ResponseListenerThread* responseListenerThread = new ResponseListenerThread(ClientNetworkManager::_connection);
        responseListenerThread->start();

        qDebug() << "ClientNetworkManager running on Thread:" << QThread::currentThreadId();

        SignalHandler* _signalHandler = new SignalHandler();

        //Connect Signals Of ResponseListenerThread to functions of ClientNetworkManager
        QObject::connect(responseListenerThread, &ResponseListenerThread::ouputServerMessage, _signalHandler, &SignalHandler::parseResponse, Qt::QueuedConnection);
        QObject::connect(responseListenerThread, &ResponseListenerThread::outputError, _signalHandler, &SignalHandler::outputError, Qt::QueuedConnection);

        qDebug() << "ClientNetworkManager running on Thread:" << QThread::currentThreadId();

        //GameController::showError("Connection error", "Could not create client network thread");


    } else {
        ClientNetworkManager::_failedToConnect = true;
        GameController::showStatus("Not connected");
    }
}


bool ClientNetworkManager::connect(const std::string& host, const uint16_t port) {

    // create sockpp address and catch any errors
    sockpp::inet_address address;
    try {
        address = sockpp::inet_address(host, port);
    } catch (const sockpp::getaddrinfo_error& e) {
        GameController::showError("Connection error", "Failed to resolve address " + e.hostname());
        return false;
    }

    // establish connection to given address
    if (!ClientNetworkManager::_connection->connect(address)) {
        GameController::showError("Connection error", "Failed to connect to server " + address.to_string());
        return false;
    }

    return true;
}


void ClientNetworkManager::sendRequest(const client_request &request) {

    // wait until network is connected (max. 5 seconds)
    int connectionCheckCounter = 0;
    while(!ClientNetworkManager::_connectionSuccess
           && !ClientNetworkManager::_failedToConnect
           && connectionCheckCounter < 200) {
        //wxMilliSleep(25);
        connectionCheckCounter++;
    }

    // do not continue if failed to connect to server
    if(ClientNetworkManager::_failedToConnect) {
        return;
    }

    if(ClientNetworkManager::_connectionSuccess && ClientNetworkManager::_connection->is_connected()) {

        // serialize request into JSON string
        rapidjson::Document* jsonDocument = request.to_json();
        std::string message = json_utils::to_string(jsonDocument);
        delete jsonDocument;

        // turn message into stream and prepend message length
        std::stringstream messageStream;
        messageStream << std::to_string(message.size()) << ':' << message;
        message = messageStream.str();

// output message for debugging purposes
#ifdef PRINT_NETWORK_MESSAGES
        std::cout << "Sending request : " << message << std::endl;
#endif

        // send message to server
        ssize_t bytesSent = ClientNetworkManager::_connection->write(message);

        // if the number of bytes sent does not match the length of the message, probably something went wrong
        if (bytesSent != ssize_t(message.length())) {
            GameController::showError("Network error", "Error writing to the TCP stream: " + ClientNetworkManager::_connection->last_error_str());
        }

    } else {
        GameController::showError("Network error", "Lost connection to server");
    }
}

