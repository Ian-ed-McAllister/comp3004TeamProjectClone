#include "mainwindow.h"

#include <QApplication>
#include <QVector>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //used for testing database
//    DBManager mydb;
//    mydb.getSession(0);
//    float vals[3] = {0.1,0.1,0.1};
//    QVector<double> xVals, yVals;
//    for (int i=0; i<101; ++i)
//    {
//      xVals.append(i);
//      yVals.append(i);
//    }
//    mydb.addSession(1, 400, 0.12,  0.12, vals,xVals,yVals);
//    mydb.getSession(2);

    MainWindow w;
    w.show();
    return a.exec();

}
