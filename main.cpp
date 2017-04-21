#include "zhsoft.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ZhSoft w;
    w.show();

    return a.exec();
}
