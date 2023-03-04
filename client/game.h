#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QRectF>
#include <QPainter>
#include <QPen>
#include <QKeyEvent>
#include <QGraphicsView>

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QElapsedTimer>
#include <QThread>

namespace Ui {
class game;
}

class game : public QWidget
{
    Q_OBJECT
    private:
        int x;
        int y;
        Ui::game *ui;
        QElapsedTimer timer;
        QNetworkAccessManager *manager = nullptr;
        QString user;

    public:
        game(QWidget *parent = nullptr, QString user = "default");
        ~game();
        void sendMeta(int x, int y, int timePassed);
        void logout();

    public slots:
        void replyFinished(QNetworkReply *reply);
        //void goBack();


    protected:
        void keyPressEvent(QKeyEvent *e) override;
        void closeEvent(QCloseEvent *e) override;
};

#endif // GAME_H
