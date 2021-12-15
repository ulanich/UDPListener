#include <QtWidgets/QApplication>
#include "UdpListener.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UdpListener w;
    w.show();

    return a.exec();
}
