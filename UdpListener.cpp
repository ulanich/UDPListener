#include "UdpListener.h"


UdpListener::UdpListener(QWidget *parent) : QMainWindow(parent)
{
    m_ui.setupUi(this);
    m_tablemodel = new QTableViewModel();

    socket = new QUdpSocket();
    
    m_ui.tableView->setModel(m_tablemodel);

    m_ui.tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_ui.tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_ui.tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    m_ui.tableView->setStyleSheet("QHeaderView::section { background-color:gray }");
    m_ui.pushStop->setEnabled(false);

    connect(m_ui.pushStop, SIGNAL(clicked()), this, SLOT(closesocket()));
    connect(m_ui.pushStart, SIGNAL(clicked()), this, SLOT(startsocket()));
}

UdpListener::~UdpListener()
{
    socket->close();
}

void UdpListener::startsocket()
{
    m_ui.pushStart->setEnabled(false);
    m_ui.pushStop->setEnabled(true);
    m_ui.lineEdit->setEnabled(false);

    QString port = m_ui.lineEdit->text().toInt();

    socket = new QUdpSocket();
    socket->bind(QHostAddress::LocalHost, m_ui.lineEdit->text().toInt());

    connect(socket, SIGNAL(readyRead()), SLOT(readPendingDiagrams()));
}

void UdpListener::closesocket()
{
    m_ui.pushStop->setEnabled(false);
    m_ui.pushStart->setEnabled(true);
    m_ui.lineEdit->setEnabled(true);
    m_tablemodel->disableDataFlag();
    socket->close();
}

void UdpListener::readPendingDiagrams()
{
    QHostAddress sender;
    quint16 port;
    unsigned long long count = 0;
    while (socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(), datagram.size(), &sender, &port);
        m_tablemodel->pushBackRow(datagram);
        if(m_ui.checkBox->checkState()) m_ui.tableView->scrollToBottom();
    }
}