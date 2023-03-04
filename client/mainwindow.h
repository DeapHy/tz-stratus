#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkRequest>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QCryptographicHash>


#include "./ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    private:
        QString salt = "";
        Ui::MainWindow *ui;
        QNetworkAccessManager *manager;
    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void handleLogin();
        void replyFinished(QNetworkReply* reply);



};

#endif // MAINWINDOW_H
