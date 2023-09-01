#ifndef DBMANAGER_H
#define BDMANAGER_H
#include <QString>
#include <QSqlDatabase>
#include <QDateTime>
#include <QDebug>
#include <QSqlQuery>
#include <QList>
#include <QApplication>
#include <QVector>
#include <QBuffer>
#include <QByteArray>
#include <QIODevice>
#include <QDataStream>
#include <QIODevice>
#include <tuple>




class DBManager
{
public:
    const QString DATE_FORMAT = "yyyy-MM-dd hh:mm";
    static const QString DATABASE_PATH;
    DBManager();
    bool addSession(int challengeLevel, int duration, float achievementScore,  float coherenceAverage, float coherenceSummary[3], QVector<double>xSeries, QVector<double>ySeries);
    bool deleteSession(QString myDate);
    bool deleteAllSessions();

    //boolean value for testing once Session class is created it will return the values of that session
    QVector<int> getSession(QString dateTime,int &dur,float &achS,float &coAvg,float &low,float& med, float&high);

    //returns a QVector of string showing the date of each of the sessions will be used to populate the history menu
    QVector<QString> getHistory();

private:
    QSqlDatabase heartWaveDB;
    bool DBInit();
};

#endif // DBMANAGER_H
