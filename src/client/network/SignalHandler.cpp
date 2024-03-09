#include "SignalHandler.h"

#include "../GameController.h"
#include "../../common/network/responses/server_response.h"

SignalHandler::SignalHandler(QObject *parent)
    : QObject{parent}
{

}


void SignalHandler::parseResponse(const std::string message) {

    qDebug() << "ClientNetworkManager running on Thread:" << QThread::currentThreadId();

    // output message for debugging purposes
#ifdef PRINT_NETWORK_MESSAGES
    std::cout << "Received response : " << message << std::endl;
#endif

    rapidjson::Document json = rapidjson::Document(rapidjson::kObjectType);
    json.Parse(message.c_str());

    try {
        server_response* res = server_response::from_json(json);
        res->Process();

    } catch (std::exception e) {
        GameController::showError("JSON parsing error", "Failed to parse message from server:\n" + message + "\n" + (std::string) e.what());
    }
}

void SignalHandler::outputError(const std::string title, std::string message){
    GameController::showError(title, message);
}
