#include "dbmanager.h"

const QString DBManager::DATABASE_PATH = "/database/hearwave.db";

/*
 * Creates a new DBManager for the database specified with DATABASE_PATH
 *
 *
 */

DBManager::DBManager()
{
    heartWaveDB = QSqlDatabase::addDatabase("QSQLITE");
    heartWaveDB.setDatabaseName("heartwave.db");

    if (!heartWaveDB.open()) {
        throw "Error: Database could not be opened";
    }
    if (!DBInit()) {
        throw "Error: Database could not be initialized";
    }

}

//initalizes the DB
bool DBManager::DBInit(){
    heartWaveDB.transaction();

    QSqlQuery query;
//    query.exec("DROP TABLE sessions");
    query.exec("CREATE TABLE IF NOT EXISTS sessions(sid INTEGER NOT NULL UNIQUE PRIMARY KEY AUTOINCREMENT,date TEXT NOT NULL, duration INTEGER NOT NULL, challenge_level INTEGER NOT NULL, achievement_score NUMERIC(3,2) NOT NULL, coherence_average NUMERIC(3,2) NOT NULL, coherence_summary_low REAL, coherence_summary_mid REAL,coherence_summary_high REAL, x_data BLOB, y_data BLOB);");

    //FOR TESTING PURPOSES
//    query.exec("INSERT OR REPLACE INTO sessions (date, duration, challenge_level, achievement_score, coherence_average,coherence_summary_low,coherence_summary_mid,coherence_summary_high) VALUES ('2023-04-05 12:37',300,1,2.10,0.12,0.33,0.33,0.33);");
//    query.exec("INSERT OR REPLACE INTO sessions (date, duration, challenge_level, achievement_score, coherence_average,coherence_summary_low,coherence_summary_mid,coherence_summary_high) VALUES ('2023-04-05 14:37',300,1,2.10,0.12,0.33,0.33,0.33);");
    return heartWaveDB.commit();
}



//gets a specific session and will return the data associated with that session
//may change so it gets based off of date and time instead
//returns data via pass by refernce and return the y data aswell
QVector<int> DBManager::getSession(QString dateTime,int &dur,float &achS,float &coAvg,float &low,float& med, float&high){
    heartWaveDB.transaction();
    QSqlQuery query;


    // select the session with the correct ID
    query.prepare("SELECT * FROM sessions WHERE date=:date ");
    query.bindValue(":date", dateTime);
    query.exec();



    QVector<int> xSeries, ySeries;
    //gets the row within the query that was jsut executed
    while(query.next()){
        dur = query.value(2).toInt();
        achS = query.value(3).toFloat();
        coAvg = query.value(4).toFloat();

        low = query.value(6).toFloat();
        med = query.value(7).toFloat();
        high = query.value(8).toFloat();
       //takes the blob byte array and converts it to a new qvector
       QByteArray xByteArray = query.value(9).toByteArray() ;
       QDataStream xStream (xByteArray);
       xStream.setByteOrder(QDataStream::LittleEndian);
       //iterates through the Byte array within the data stream
       while(!xStream.atEnd()){
           int d;
           xStream >> d;
           xSeries << d;
       }
       //does the same for the Y data
       QByteArray yByteArray = query.value(10).toByteArray();
       QDataStream yStream (yByteArray);
       //iterates through the Byte array within the data stream
       while(!yStream.atEnd()){
           int d;
           yStream >> d;
           ySeries << d;
       }


    }

    return ySeries;

}

//deletes the session based off of the date and time
bool DBManager::deleteSession(QString myDate){
    QSqlQuery query;
    query.prepare("DELETE FROM sessions WHERE date=:date");
    query.bindValue(":date", myDate);

    return query.exec();

}
//deletes all sessions in the database
bool DBManager::deleteAllSessions(){
    QSqlQuery query;
    query.prepare("DELETE FROM sessions WHERE date=date");
    qDebug()<<query.exec();
    return true;
}



//Adds a new session to the data base using the passed in parameters
bool DBManager::addSession(int challengeLevel, int duration, float achievementScore,  float coherenceAverage, float coherenceSummary[3], QVector<double>xSeries, QVector<double>ySeries){
    heartWaveDB.transaction();

    //ran into issues putting doubles into the database need to convert to int
    QVector<int> newX, newY;
    QVectorIterator<double> xNumbers(xSeries);
    QVectorIterator<double> yNumbers(ySeries);
    while(yNumbers.hasNext()){
        newX.append((int)xNumbers.next());
        newY.append((int)yNumbers.next());
    }


    //used to serialize the Qvector into a blob to be put into the database
    QByteArray xdata;
     QDataStream xStream(&xdata, QIODevice::ReadWrite);
     for (auto val :newX){

         xStream << val;
     }

     QByteArray ydata;
     QDataStream yStream(&ydata, QIODevice::ReadWrite);
     for (auto val : newY){

         yStream << val;
     }



    QSqlQuery query;
    QDateTime qtd;
    //NEEDS TO BE UPDATED TO ADD COHERANCE SUMMARY
    query.prepare("INSERT INTO sessions (date, duration, challenge_level, achievement_score, coherence_average,coherence_summary_low,coherence_summary_mid,coherence_summary_high,x_data,y_data) VALUES (:date,:duration,:challenge_level,:achievement_score,:coherence_average,:coherence_low,:coherence_mid,:coherence_high,:x_data,:y_data);");


    query.bindValue(":date",qtd.currentDateTime().toString(DATE_FORMAT));
    query.bindValue(":duration",duration);
    query.bindValue(":challenge_level",challengeLevel);
    query.bindValue(":achievement_score",achievementScore);
    query.bindValue(":coherence_average",coherenceAverage);

    query.bindValue(":coherence_low",coherenceSummary[0]);
    query.bindValue(":coherence_mid",coherenceSummary[1]);
    query.bindValue(":coherence_high",coherenceSummary[2]);
    query.bindValue(":x_data",xdata);
    query.bindValue(":y_data",ydata);
    query.exec();


    return heartWaveDB.commit();

}

//returnts all of ithe dates and times of the sessions
QVector<QString> DBManager::getHistory(){


    heartWaveDB.transaction();
    QSqlQuery query;
    query.prepare("SELECT * FROM sessions");
    query.exec();
    QVector<QString> completedSessions;

    while(query.next()){

        completedSessions.append(query.value(1).toString());
    }
    return completedSessions;

}
