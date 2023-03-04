#include "game.h"
#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->passwInput->hide();
    ui->passwLabel->hide();

    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::replyFinished);
    connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::handleLogin);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleLogin() {
    if (ui->loginInput->text() == ""){
        ui->errorLabel->setText("Логин не может быть пустым");
        return;
    }
    QJsonObject jobject;
    if (ui->passwInput->text() == ""){
        jobject["login"] = ui->loginInput->text();
        QUrl url("http://localhost:8000/getOTT");
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QJsonDocument doc(jobject);
        QByteArray data = doc.toJson();

        manager->post(request, data);
    }
    else{
        jobject["login"] = ui->loginInput->text();

        QString hashedLogin = QString(QCryptographicHash::hash((ui->loginInput->text() + ":" + ui->passwInput->text()).toUtf8(), QCryptographicHash::Md5).toHex());
        QString hash = QString(QCryptographicHash::hash((salt + ">>" + hashedLogin).toUtf8(), QCryptographicHash::Md5).toHex());
        jobject["hash"] = hash;

        QUrl url("http://localhost:8000/login");
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QJsonDocument doc(jobject);
        QByteArray data = doc.toJson();

        manager->post(request, data);

    }
}

void MainWindow::replyFinished(QNetworkReply *reply){
    QByteArray rep = reply->readAll();
    QString str = QString(rep);

    QJsonDocument jsonResponse = QJsonDocument::fromJson(str.toUtf8());

    QJsonObject jsonObject = jsonResponse.object();
    if (jsonObject["Success"].toBool()){
        if (jsonObject["hash"].toString().compare("") != 0){
            ui->passwInput->show();
            ui->passwLabel->show();
            this->salt = jsonObject["hash"].toString();
        }
        else{
            game *a = new game(nullptr, ui->loginInput->text());
            a->show();
            this->hide();
        }

    }
    else{
        switch (jsonObject["error_code"].toInt()){
            case 1:
                ui->errorLabel->setText("Неверный пароль");
                break;
            case 2:
                ui->errorLabel->setText("Неверный логин");
                break;
            default:
                ui->errorLabel->setText("Что-то пошло не так");
                break;
        }
    }
}
