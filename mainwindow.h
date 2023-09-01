#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QTimer>
#include <QRandomGenerator>
#include <QVector>
#include <QString>
#include <tuple>
#include "session.h"

#include "dbmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //ian's new functions
    void initializeTimer();
    void createSummary(int length, float achievementScore,  float coherenceAverage, float coherenceSummary[3], QVector<double>xSeries, QVector<double>ySeries);
    void updateHistory();
    void swapScreen();

private slots:

    void generateSummaryGraph(QVector<double> xData, QVector<double> yData);
    void generateGraph(QVector<double> xData,QVector<double> yData);
    void navigateBack();
    void downButton();
    void navigateToMenu();
    void upButton();
    void handleSelection();
    void powerButton();
   //void changePulse();
    //void openSettings();

    void applyToSkin(bool value);


    void updateTimer();
    void historyToSummary(QString val);
    void deleteAllHistory();
    void deleteSingleSession();
    void swapFile();
    void batteryCharge();

private:
    Ui::MainWindow *ui;
    bool powerOn;
    bool isSetting;
    bool hasPulse;
    int breathPace;
    QTimer* timer;
    int currentTimerCount;
    QVector<double> xData,yData;
    bool pacerDir;
    QListWidget *menuList;
    DBManager myDb;
    Session ses;
    //vector of the diffrent main screens
    //0: main menu, 1: session 2: summmary 3: history 4:settings(not implemented yet)
    QVector<QWidget*> screens;
    int curWindow;
    int prevWindow;
    int selectedScores;
    int curIndex;
    QString pickedHistory;
    QVector<float> lowScores;
    QVector<float> medScores;
    QVector<float> highScores;
    QVector<float> mixScores;
};
#endif // MAINWINDOW_H
