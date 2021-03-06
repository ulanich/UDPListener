#pragma once

#include <QtWidgets/QMainWindow>
#include "x64/Debug/uic/ui_tlmqt.h"
#include "TableModell.h"
//#include "DataContainer.h"
#include <QUdpSocket>


class UdpListener : public QMainWindow
{
    Q_OBJECT

public:
    UdpListener(QWidget *parent = Q_NULLPTR);
    ~UdpListener();

private:
    Ui::tlmqtClass m_ui;
    QTableViewModel* m_tablemodel;
    QUdpSocket* socket;

private slots:
    void readPendingDiagrams();
    void closeSocket();
    void startSocket();
};
