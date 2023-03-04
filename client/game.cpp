#include "game.h"
#include "ui_game.h"

game::game(QWidget *parent, QString user) :
    QWidget(parent),
    ui(new Ui::game)
{
    this->user = user;
    x = 270;
    y = 120;
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &game::replyFinished);
    timer.start();
}
game::~game()
{
    delete ui;
}

void game::keyPressEvent(QKeyEvent *e) {
    switch (e->key()){
        case 87:
        case 1062:
            if (y - 5 <= 0)
                break;
            y -= 5;
            break;
        case 83:
        case 1067:
            if (y + 5 >= 225)
                break;
            y += 5;
            break;
        case 65:
        case 1060:
            if (x - 5 <= 0)
                break;
            x -= 5;
            break;
        case 68:
        case 1042:
            if (x + 5 >= 545)
                break;
            x += 5;
            break;
        default:
            return;
    }
    game::sendMeta(x, y, timer.elapsed());
    ui->pushButton->setGeometry(x, y, 30, 30);
}

void game::sendMeta(int x, int y, int timeElapsed){
    QUrl url("http://localhost:8000/sendMeta");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jobject;
    jobject["x"] = x;
    jobject["y"] = y;
    jobject["time"] = timeElapsed;
    jobject["login"] = this->user;

    QJsonDocument doc(jobject);
    QByteArray data = doc.toJson();

    manager->post(request, data);
}

void game::replyFinished(QNetworkReply *reply)
{
    QByteArray rep = reply->readAll();
    QString str = QString(rep);

    QJsonDocument jsonResponse = QJsonDocument::fromJson(str.toUtf8());

    QJsonObject jsonObject = jsonResponse.object();
    QString success = jsonObject["Success"].toString();

    if (success.compare("True") == 0){
        reply->deleteLater();
    }
    else {
        qDebug() << "Ошибка";
    }
}

void game::logout(){
    QUrl url("http://localhost:8000/logout");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jobject;
    jobject["login"] = user;

    QJsonDocument doc(jobject);
    QByteArray data = doc.toJson();

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QEventLoop loop;
    QNetworkReply* reply = manager->post(request, data);
    connect(reply , SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    replyFinished(reply);
}

void game::closeEvent(QCloseEvent *e) {
    if (e->spontaneous()) {
        this->logout();
        e->accept();
    } else {
        e->ignore();
    }
}
