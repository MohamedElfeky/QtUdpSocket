#include "QtUdpSocket.h"



QtUdpServer::QtUdpServer()
{
	udpServer = new QUdpSocket(this);
	//�󶨱��ض˿�
	udpServer->bind(QHostAddress::AnyIPv4, 8810, QUdpSocket::ShareAddress);
	//�����鲥��
	bool result = udpServer->joinMulticastGroup(QHostAddress("224.104.100.101"));
	if (result)
	{
		//���û�����
		udpServer->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 1024 * 1024 * 8);
		//���ӽ����źŲ�     
		connect(udpServer, SIGNAL(readyRead()), this, SLOT(dataReceived()));
	}
}

QtUdpServer::~QtUdpServer()
{
	delete udpServer;
}


void QtUdpServer::dataReceived()
{
	while (udpServer->hasPendingDatagrams())
	{
		QByteArray datagram;
		datagram.resize(udpServer->pendingDatagramSize());
		udpServer->readDatagram(datagram.data(), datagram.size());
		QString msg = QString::fromLocal8Bit(datagram);
		qDebug() << msg;
	}
}

QtUdpClient::QtUdpClient()
{
	udpClient = new QUdpSocket(this);
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(dataSend()));
	timer->setInterval(1000);
	timer->start();
}

QtUdpClient::~QtUdpClient()
{
	delete udpClient;
	delete timer;
}


void QtUdpClient::dataSend()
{
	QString  msg = QString::fromLocal8Bit("hello��QtSocket!");
	if (udpClient->writeDatagram(msg.toLocal8Bit(), msg.toLocal8Bit().length(),QHostAddress( "224.104.100.101"), 8810) != msg.length())
	{
		return;
	}
}