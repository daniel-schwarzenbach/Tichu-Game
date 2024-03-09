#include <QApplication>
#include <QDebug>
#include <QThread>
#include "GameController.h"

// Macro which will generate a main function which serves as entry point into the program
int main(int argc, char *argv[])
{
    QApplication Tichu(argc, argv);

    qDebug() << "Main Thread started with" << QThread::currentThreadId() << ".";
    if(argc == 2){
        std::string playerName = argv[1];
        GameController::init(playerName);
    } else {
        GameController::init();
    }
    
    return Tichu.exec();
}
