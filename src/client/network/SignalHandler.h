#ifndef SIGNALHANDLER_H
#define SIGNALHANDLER_H

#include <QObject>

class SignalHandler : public QObject
{
    Q_OBJECT
public:
    explicit SignalHandler(QObject *parent = nullptr);

public slots:
    void parseResponse(const std::string message);

    void outputError(const std::string title, std::string message);

};

#endif // SIGNALHANDLER_H
