#include "ConnectionView.h"
#include "./ui_ConnectionView.h"

#include "../../common/network/default.conf"
#include "../GameController.h"
#include <QIntValidator>

ConnectionView::ConnectionView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionView)
{

    ui->setupUi(this);

    //Set Logo
    QPixmap logo("assets/tichu_logo.png");
    ui->logo->setPixmap(logo);

    //Set default text of Textfields
    ui->serverText->setText(QString::fromStdString(default_server_host));
    ui->portText->setText(QString::number(default_port));

    //Placeholder for Playername
    ui->nameText->setPlaceholderText("Max");

    //Comment the following line when finished with testing
    ui->nameText->setText("Max");

    //Set max length of playername and server ip adress
    ui->nameText->setMaxLength(16);
    ui->serverText->setMaxLength(15);

    //Only accept int input for port text input
    ui->portText->setValidator( new QIntValidator(0, 99999, this) );

}

ConnectionView::ConnectionView(std::string const& playerName, QWidget *parent):
    QWidget(parent),
    ui(new Ui::ConnectionView)
{

    ui->setupUi(this);

    //Set Logo
    QPixmap logo("assets/tichu_logo.png");
    ui->logo->setPixmap(logo);

    //Set default text of Textfields
    ui->serverText->setText(QString::fromStdString(default_server_host));
    ui->portText->setText(QString::number(default_port));

    //Placeholder for Playername
    ui->nameText->setPlaceholderText("Max Muster");

    //Comment the following line when finished with testing
    ui->nameText->setText(playerName.c_str());

    //Set max length of playername and server ip adress
    ui->nameText->setMaxLength(16);
    ui->serverText->setMaxLength(15);

    //Only accept int input for port text input
    ui->portText->setValidator( new QIntValidator(0, 99999, this) );

}

ConnectionView::~ConnectionView()
{
    delete ui;
}

QString ConnectionView::getServerAddress(){
    return ui->serverText->text();
}

QString ConnectionView::getServerPort(){
    return ui->portText->text();
}

QString ConnectionView::getPlayerName(){
    return ui->nameText->text();
}

void ConnectionView::on_connectButton_clicked()
{
    GameController::connectToServer();
}

