#ifndef CONNECTIONVIEW_H
#define CONNECTIONVIEW_H

#include <QWidget>

namespace Ui {
class ConnectionView;
}

class ConnectionView : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionView(QWidget *parent = nullptr);
    explicit ConnectionView(std::string const& playerName, QWidget *parent = nullptr);
    ~ConnectionView();

    QString getServerAddress();
    QString getServerPort();
    QString getPlayerName();

private slots:
    void on_connectButton_clicked();

private:
    Ui::ConnectionView *ui;
};

#endif // CONNECTIONVIEW_H
