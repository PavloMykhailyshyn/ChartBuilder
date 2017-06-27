#include "chartbuilder.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChartBuilder w;
    w.show();

    return a.exec();
}
