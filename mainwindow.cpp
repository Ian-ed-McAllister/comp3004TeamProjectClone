#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QComboBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , powerOn(false)
    , isSetting(false)
    , hasPulse(false)

{
    ui->setupUi(this);

    //interface buttons

    connect(ui->backButton, SIGNAL(released()), this, SLOT(navigateBack()));

    connect(ui->menuButton, SIGNAL(released()), this, SLOT(navigateToMenu()));
    connect(ui->upButton, SIGNAL(released()), this, SLOT(upButton()));
    connect(ui->downButton, SIGNAL(released()), this, SLOT(downButton()));

    connect(ui->selectButton, SIGNAL(released()), this, SLOT(handleSelection()));
    connect(ui->powerButton, SIGNAL(clicked()), this, SLOT(powerButton()));
    connect(ui->chargeBattery, SIGNAL(released()), this, SLOT(batteryCharge()));



    ui->menuButton->setIcon(QIcon(":/icons/menu"));
        ui->backButton->setIcon(QIcon(":/icons/backButton"));
        ui->upButton->setIcon(QIcon(":/icons/upButton"));
        ui->downButton->setIcon(QIcon(":/icons/downButton"));
    //    ui->leftButton->setIcon(QIcon(""));
    //    ui->rightButton->setIcon(QIcon(""));
        ui->chargeBattery->setIcon(QIcon(":/icons/chargeBattery"));
        ui->chargeBattery->setFlat(true);


        connect(ui->pulseCheck, SIGNAL(clicked(bool)), this, SLOT(applyToSkin(bool)));

    //Menu view
    //menuList = ui->displayScreen;
    //menuList->addItems(displayScreen());
    //menuList->setCurrentRow(0);
        ui->menuList->addItem("Start new session");
        ui->menuList->addItem("History");
        ui->menuList->addItem("Settings");

        //Apply to skin settup
//        ui->applyToSkinDropDown->addItem("True");
//        ui->applyToSkinDropDown->addItem("False");
//        ui->applyToSkinDropDown->setCurrentText(hasPulse ? "True" : "False");

        QPixmap pixmap(hasPulse ? ":/icons/heart.png" : ":/icons/blackHeart.png");
        QPixmap scaledPixmap = pixmap.scaled(QSize(15,15), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->pulseIcon->setPixmap(scaledPixmap);
        ui->pulseIcon->setFixedSize(scaledPixmap.size());

    //Placeholder variables, uncomment to see sample data graph
    //QVector<double> x, y;
    //MainWindow::generateGraph(x,y);


    //Needs to be reset each time a new session is started
    timer = new QTimer(this);
    currentTimerCount = 0;
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));

    //removes text form breath bar
    ui->breathPacer->setTextVisible(false);
    pacerDir = true;


    //hides the session window

    ui->lowLevel->setVisible(true);
    ui->midLevel->setVisible(false);
    ui->highLevel->setVisible(false);
    ui->deleteSingleSession->setVisible(false);

    ui->sessionWindow->setVisible(false);
    ui->summaryWindow->setVisible(false);
    ui->pulseIcon->setVisible(false);
    ui->historyScreen->setVisible(false);
    ui->settingsWindow->setVisible(false);

    ui->sessionWindow->move(30,70);
    ui->summaryWindow->move(30,70);
    ui->historyScreen->move(30,70);
    ui->settingsWindow->move(30,70);



    //adding the screns to a QVector, cur window refers to an index of the vector
    curWindow = 0;
    prevWindow =-1;
    screens.append(ui->widget_2);
    screens.append(ui->sessionWindow);
    screens.append(ui->summaryWindow);
    screens.append(ui->historyScreen);
    screens.append(ui->settingsWindow);

    connect(ui->deleteAllHistoryButton,SIGNAL(released()), this, SLOT(deleteAllHistory()));
    connect(ui->deleteSingleSession,SIGNAL(released()), this, SLOT(deleteSingleSession()));
    connect(ui->lowButton,SIGNAL(clicked()),this,SLOT(swapFile()));
    connect(ui->medButton,SIGNAL(clicked()),this,SLOT(swapFile()));
    connect(ui->highButton,SIGNAL(clicked()),this,SLOT(swapFile()));
    connect(ui->mixButton,SIGNAL(clicked()),this,SLOT(swapFile()));

    lowScores = {0.1,0.2,0.3,0.4,0.3,0.2,0.1,0.2,0.3,0.4,0.3,0.2,0.1,0.2,0.3,0.4,0.3,0.2,0.1,0.2,0.3,0.4,0.3,0.2,0.1,0.2,0.3,0.4,0.3,0.2,0.1,0.1,0.2,0.3,0.4,0.3,0.2,0.1,0.2,0.3,0.4,0.3,0.2,0.1,0.2,0.3,0.4,0.3,0.2,0.1,0.2,0.3,0.4,0.3,0.2,0.1,0.2,0.3,0.4,0.3,0.2,0.1};
    medScores = {0.5,0.6,0.7,0.8,0.9,0.8,0.7,0.6,0.5,0.6,0.7,0.8,0.9,0.8,0.7,0.6,0.5,0.6,0.7,0.8,0.9,0.8,0.7,0.6,0.5,0.6,0.7,0.8,0.9,0.8,0.7,0.6,0.5,0.6,0.7,0.8,0.9,0.8,0.7,0.6,0.5};
    highScores = {4.0,8.1,8.2,5.3,4.4,4.3,4.2,4.1,4.0,4.1,4.2,2.3,2.4,2.3,2.2,1.1,1.0,1.1,1.2,1.3,1.4,1.3,1.2,1.1,1.0,1.1,1.2,1.3,1.4,1.3,1.2,1.1,1.0,1.1,1.2,1.3,1.4,1.3,1.2,1.1,1.0};
    mixScores = {1.3,0.6,0.2,0.3,0.5,0.7,1.1,1.2,1.3,0.1,0.7,0.8,1.3,0.6,0.2,0.3,0.5,0.7,1.1,1.2,1.3,0.1,0.7,0.8,1.3,0.6,0.2,0.3,0.5,0.7,1.1,1.2,1.3,0.1,0.7,0.8,1.3,0.6,0.2,0.3,0.5,0.7,1.1,1.2,1.3,0.1,0.7,0.8};
}

MainWindow::~MainWindow()
{
    delete ui;
}






//code to implement the back button on the UI, some cases where you cannot move back is when you were just in a session
void MainWindow::navigateBack(){
    if (ui->batteryDisplay->value() == 0){
        qDebug()<<"battery low";
        return powerButton();
    }
    if(prevWindow == 3){
        ui->deleteSingleSession->setVisible(false);
    }
    if((curWindow != 1) && (prevWindow != -1) && (prevWindow != 1)){
        int temp = curWindow;
        curWindow =prevWindow;
        prevWindow = temp;
        swapScreen();
    }

}

//code to implement the up button to cycle through the menus
void MainWindow::upButton(){
    if (ui->batteryDisplay->value() == 0){
        qDebug()<<"battery low";
        return powerButton();
    }
    
    //if in the main menu
    if(curWindow == 0){
        int currRow = ui->menuList->currentRow();
        ui->menuList->setCurrentRow(((currRow - 1 + ui->menuList->count()) % ui->menuList->count()));
    }
    //if in the history menu
    else if(curWindow == 3){
        int currRow = ui->historyMenu->currentRow();
        ui->historyMenu->setCurrentRow(((currRow - 1 + ui->historyMenu->count()) % ui->historyMenu->count()));
    }


}

//code to implement the down button to cycle through the menus (same as upButton but moves downwards through the menu)
void MainWindow::downButton(){
    if (ui->batteryDisplay->value() == 0){
        qDebug()<<"battery low";
        return powerButton();
    }
    if(curWindow == 0){
        int currRow = ui->menuList->currentRow();
        ui->menuList->setCurrentRow(((currRow + 1 + ui->menuList->count()) % ui->menuList->count()));
    }else if(curWindow == 3){
        int currRow = ui->historyMenu->currentRow();
        ui->historyMenu->setCurrentRow(((currRow + 1 + ui->historyMenu->count()) % ui->historyMenu->count()));
    }

}


//code to make the menu button work
void MainWindow::navigateToMenu(){
    if (ui->batteryDisplay->value() == 0){
        qDebug()<<"battery low";
        return powerButton();
    }
    //dont want to swap if ina session
    if(curWindow != 1){
        prevWindow = curWindow;
        curWindow = 0;
        ui->deleteSingleSession->setVisible(false);
        swapScreen();

    }


}






//handels when user presses select, diffrent cases based on what screen you are on
void MainWindow::handleSelection(){
    if (ui->batteryDisplay->value() == 0){
        return powerButton();
    }


    //SELECT BUTTON IN MAIN MENU
    if(curWindow == 0){
        //Get the selected item in Menu
        QListWidgetItem* item = ui->menuList->currentItem();
        //Actions for selected item within main menu
        if(item) {
            QString selectedText = item->text();

            if (selectedText == "Start new session"){
                if(hasPulse){

                    prevWindow = curWindow;
                    curWindow = 1;
                    //handles the switching to session screen
                    if(ui->batteryDisplay->value() < 5){
                        qDebug()<<"To low to start new session";
                        return;
                    }
                    initializeTimer();
                }else{
                    qDebug()<<"PLEASE ATTACH THE MONITOR TO SKIN";
                }


            }else if (selectedText == "History") {
                prevWindow = curWindow;
                curWindow = 3;
                updateHistory();

                swapScreen();

            } else if (selectedText == "Settings") {
                prevWindow = curWindow;
                curWindow = 4;
                swapScreen();
            }
        }
    }
    //for when in a session
    else if(curWindow == 1){
        //sets to summary screen
        prevWindow = curWindow;
        curWindow = 2;
        //handles the rest of the the switching
        initializeTimer();
    }
    //for when in summary screen
    else if(curWindow == 2){
        if(prevWindow == 1){
            prevWindow = curWindow;
            curWindow = 0;
            swapScreen();
        }else if(prevWindow == 3){
            prevWindow = curWindow;
            curWindow = 3;
            swapScreen();
            ui->deleteSingleSession->setVisible(false);


        }
    }
    //for when in the history screen
    else if(curWindow ==3){
        QString text = ui->historyMenu->currentItem()->text();
        pickedHistory = text;


        prevWindow = curWindow;
        curWindow = 2;
        historyToSummary(text);

    }
}
//handels when the user presses the power button
void MainWindow::powerButton(){



        powerOn = !powerOn;
        if(powerOn){
            screens[curWindow]->setVisible(true);
        }else{
            screens[curWindow]->setVisible(false);
        }

        ui->upButton->setEnabled(powerOn);
        ui->downButton->setEnabled(powerOn);
        ui->leftButton->setEnabled(powerOn);
        ui->rightButton->setEnabled(powerOn);
        ui->menuButton->setEnabled(powerOn);
        ui->backButton->setEnabled(powerOn);
        ui->selectButton->setEnabled((powerOn));


}


//sets the icon and has pulse variable when the user has the sensor on their skin (side menu)
void MainWindow::applyToSkin(bool value){
        hasPulse = value;

        if(hasPulse){
            ui->pulseIcon->setVisible(true);

        }else{
            ui->pulseIcon->setVisible(false);

        }







}


//makes the graph for the session

void MainWindow::generateGraph(QVector<double> xData, QVector<double> yData){
    ui->qGraph->addGraph();
    ui->qGraph->graph(0)->setLineStyle(QCPGraph::lsLine);
    //Labels
    ui->qGraph->xAxis->setLabel("Time (Seconds)");
    ui->qGraph->yAxis->setLabel("Heart Rate");
    //Axes ranges
    ui->qGraph->yAxis->setRange(50,100);
    //Sample Data
    //xData = {0, 5, 10, 15, 20, 25, 30};
    //yData = {63, 75, 59, 84, 91, 52, 78};
    ui->qGraph->graph(0)->setData(xData,yData);
    ui->qGraph->rescaleAxes();
    ui->qGraph->replot();
}




//Ian's new functions
//makes the graph for the summary view
void MainWindow::generateSummaryGraph(QVector<double> xData, QVector<double> yData){
    ui->qGraph_2->addGraph();
    ui->qGraph_2->graph(0)->setLineStyle(QCPGraph::lsLine);
    //Labels
    ui->qGraph_2->xAxis->setLabel("Time (Seconds)");
    ui->qGraph_2->yAxis->setLabel("Heart Rate");
    //Axes ranges
    ui->qGraph_2->yAxis->setRange(50,100);
    //Sample Data
    //xData = {0, 5, 10, 15, 20, 25, 30};
    //yData = {63, 75, 59, 84, 91, 52, 78};
    ui->qGraph_2->graph(0)->setData(xData,yData);
    ui->qGraph_2->rescaleAxes();
    ui->qGraph_2->replot();
}

//timer for the session if it gets called when in a session it will stop the timer (therefore the session) and show the session screen and save the data to the database
//normal use is that it starts the session and starts the timer to run another function every second
void MainWindow::initializeTimer() {

    //used to create the summary screen
    if (currentTimerCount > 0){
        prevWindow = 1;
        curWindow =2;

        ui->sessionWindow->setVisible(false);
        timer->stop();
        //to be replaced with actual data


        float* vals = ses.getCoherenceSummary();
        float newVals[3] = {vals[0],vals[1],vals[2]};
        qDebug()<<myDb.addSession(1,currentTimerCount,ui->archNum->text().toFloat(),ses.getCoherenceAverage(),newVals,xData,yData);
        //show summary screen
        createSummary(currentTimerCount,ui->archNum->text().toFloat(),ses.getCoherenceAverage(),newVals,xData,yData);

        //reset the variables used in the session
        currentTimerCount =0;
        xData.clear();
        yData.clear();
        ui->breathPacer->setValue(0);

    }
    //used to start the session
    else{
        curIndex = 0;
        if (hasPulse == true) {
                ses.reset();

                swapScreen();

                timer->start(1000);
            }


            qDebug()<<"Put the sensor on!!!!";
    }
}

//function that runs each time the timer times out (1 second)
//runs various updates each time
void MainWindow::updateTimer() {
    currentTimerCount++;
    if(hasPulse == true){
       if (ui->batteryDisplay->value() <= 5){
            qDebug()<<"BATTERY IS TO LOW STOPING SESSION";
            initializeTimer();
       }


        //upadte the time shown on screen
        QString s = QString::number(currentTimerCount);
        ui->lengthNum->setText(s);


        //updates the graph happens every second
        yData.append(qrand() % ((100+1)-60)+60);
        xData.append(currentTimerCount);
        generateGraph(xData,yData);


        //move breath bar
        if (pacerDir){
            ui->breathPacer->setValue(ui->breathPacer->value() + (100/10));
            if (ui->breathPacer->value() == 100){
                pacerDir=false;
            }
        }else{
            ui->breathPacer->setValue(ui->breathPacer->value() - (100/10));
            if (ui->breathPacer->value() == 0){
                pacerDir=true;
            }
        }


        //update battery level
        if(currentTimerCount%3 ==0){
            ui->batteryDisplay->setValue(ui->batteryDisplay->value()-1);
        }

        //update session things
        if(currentTimerCount%5 == 0){
            //COHERENCE LEVEL CHANGER (RED< YELLOW, GREEN) WILL GET COHERENCE LEVEL FROM SESSION ONCE COMPLETED
            if(selectedScores == 0){
                ses.addCoherenceScore(lowScores.value(curIndex));
            }else if(selectedScores == 1){
                ses.addCoherenceScore(medScores.value(curIndex));
            }else if (selectedScores ==3){
                ses.addCoherenceScore(mixScores.value(curIndex));
            }else{
                ses.addCoherenceScore(highScores.value(curIndex));
            }

            curIndex++;


            if(ses.getCoherenceLevel() =="low"){
                ui->lowLevel->setVisible(true);
                ui->midLevel->setVisible(false);
                ui->highLevel->setVisible(false);
            }else if(ses.getCoherenceLevel() =="medium"){
                ui->lowLevel->setVisible(false);
                ui->midLevel->setVisible(true);
                ui->highLevel->setVisible(false);
            }else{
                ui->lowLevel->setVisible(false);
                ui->midLevel->setVisible(false);
                ui->highLevel->setVisible(true);
            }
            QString s = QString::number(ses.getCoherenceScores().last());
            ui->coScore->setText(s);
            s = QString::number(ses.getAchievementScore()+(currentTimerCount*1));

            ui->archNum->setText(s);

        }

    }else{
    qDebug()<<"Sensor removed ending session";
    initializeTimer();
    }



}

//updates the info in the summary window, then generates the summary graph
void MainWindow::createSummary(int length, float achievementScore,  float coherenceAverage, float coherenceSummary[3], QVector<double>xSeries, QVector<double>ySeries){
    swapScreen();

    //setting % ofd time in certain lvels
    QString s = QString::number(coherenceSummary[0]);
    ui->lowLevelSummary->setText(s);
    s = QString::number(coherenceSummary[1]);
    ui->midLevelSummary->setText(s);
    s = QString::number(coherenceSummary[2]);
    ui->highLevelSummary->setText(s);

    s = QString::number(achievementScore);
    ui->achNumSummary->setText(s);
    s = QString::number(length);
    ui->lengthNum->setText(s);
    s = QString::number(coherenceAverage);
    ui->coScoreSummary->setText(s);
    s = QString::number(length);
    ui->lengthNumSummary->setText(s);

    generateSummaryGraph(xSeries,ySeries);

}


//takes the selected session from the history gets the data from the database and creates the session menu
void MainWindow::historyToSummary(QString val){
    int dur;
    float achievementScore, coherenceAverage, low, med,high;

    ui->deleteSingleSession->setVisible(true);
    QVector<int> sumY;

//    float* vals = myDb.getSession(1,&dur,&achievementScore,&coherenceAverage,&sumX,&sumY);
    sumY = myDb.getSession(val,dur,achievementScore,coherenceAverage,low,med,high);

    float vals[3] = {low,med,high};
    QVector<double> newX, newY;

    QVectorIterator<int> yNumbers(sumY);
    double i = 1;
    while(yNumbers.hasNext()){
        newX.append(i);
        newY.append((double)yNumbers.next());
        i++;
    }

    createSummary(dur,achievementScore, coherenceAverage,vals,newX,newY);
}

//updates the history menu from the database
void MainWindow::updateHistory(){
    QVector<QString> items = myDb.getHistory();
    if(items.length() == 0){
        qDebug()<<"NO ITEMS IN DATABASE NOTHING TO SHOW";
    }
    qDebug()<<items;
    ui->historyMenu->clear();
    for(QString item : items)
    ui->historyMenu->addItem(item);

}


//function to swap the screen depending on the prevWindow and curWindow
void MainWindow::swapScreen(){


    (screens[curWindow])->setVisible(true);
    (screens[prevWindow])->setVisible(false);
//    (screens[curWindow])->activateWindow();
}

//function called when the delete all button is pressed in history
void MainWindow::deleteAllHistory(){
    myDb.deleteAllSessions();
    updateHistory();
}


//deletes the session selected (only available after selecting a session through the history menu)
void  MainWindow::deleteSingleSession(){

    myDb.deleteSession(pickedHistory);
    updateHistory();
}


void MainWindow::swapFile(){


    if(ui->lowButton->isChecked()){
        selectedScores = 0;
    }else if(ui->medButton->isChecked()){
        selectedScores = 1;
    }else if (ui->mixButton->isChecked()){
        selectedScores =3;
    }else{
        selectedScores = 2;
    }



}

void MainWindow::batteryCharge(){
    ui->batteryDisplay->setValue(100);
}
