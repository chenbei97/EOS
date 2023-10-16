#include <QCoreApplication>
#include <QDebug>
#include "public.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    qDebug() << "Hello World";
    return QCoreApplication::exec();
}
