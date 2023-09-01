#include "session.h"

Session::Session() {
    breathPace = 10;
    achievementScore = 0;
    coherenceAverage = 0;
}

// check if coherence level has changed; if so, "beep"
// coherence levels
// low: (< 0.5), medium: (<= 0.9), high: (> 0.9)
void Session::detectCoherenceChange() {
    float current = 0, previous = 0;

    if (coherenceScores.size() >= 2) {
        current = coherenceScores[coherenceScores.size()-1];
        previous = coherenceScores[coherenceScores.size()-2];

        if ((previous < 0.5 && current >= 0.5) || (previous <= 0.9 && current > 0.9)) {
            qDebug()<<"BEEP! Coherence level has increased!";
        } else if ((previous >= 0.5 && current < 0.5) || (previous > 0.9 && current <= 0.9)) {
            qDebug()<<"BEEP! Coherence level has decreased!";
        }
    }
}

// finds the average coherence level from the 64 most recent seconds
// 1 score every 5 seconds across 64 seconds = 12 scores
void Session::updateCoherenceLevel() {
    float average = 0;

    if (coherenceScores.size() < 13) {
        average = getCoherenceAverage();
    } else {
        float sum = 0;
        for (int i = coherenceScores.size()-1; i > coherenceScores.size()-13; i--) { // find the 12 most recent scores
            sum += coherenceScores[i];
        }
        average = sum/12;
    } if (average < 0.5) {
        coherenceLevel = "low";
    } else if (average <= 1) {
        coherenceLevel = "medium";
    } else {
        coherenceLevel = "high";
    }
}

// add a new coherence score to the session's data and update its metrics as a result
void Session::addCoherenceScore(float coherence) {
    coherenceScores.append(coherence);

    detectCoherenceChange();
    updateAchievementScore();
    updateCoherenceAverage();
    updateCoherenceLevel();
    updateCoherenceSummary();
}

// calculate the average coherence score
void Session::updateCoherenceAverage() {
    coherenceAverage = achievementScore/coherenceScores.size();
}

// calculate the percentage of coherence scores found in each coherence level
// coherence levels--low: (< 0.5), medium: (<= 0.9), high: (> 0.9)
void Session::updateCoherenceSummary() {
    float low = 0, medium = 0, high = 0; // coherence levels
    for (int i = 0; i < coherenceScores.size(); i++) {
        if (coherenceScores[i] < 0.5) {
            low++;
        } else if (coherenceScores[i] <= 0.9) {
            medium++;
        } else {
            high++;
        }
    }

    // percentage of time spent in each level
    coherenceSummary[0] = (low/coherenceScores.size())*100;
    coherenceSummary[1] = (medium/coherenceScores.size())*100;
    coherenceSummary[2] = (high/coherenceScores.size())*100;
}



void Session::reset(){
    QString coherenceLevel;
    achievementScore = 0;
    coherenceAverage=0;
    coherenceScores.clear();
}



// calculate the sum of all coherence scores
void Session::updateAchievementScore() {
    achievementScore += coherenceScores.back();
}

// getters
float Session::getAchievementScore() { return achievementScore; }
float Session::getCoherenceAverage() { return coherenceAverage; }
float* Session::getCoherenceSummary() { return coherenceSummary; }
QVector<float> Session::getCoherenceScores() { return coherenceScores; }
QString Session::getCoherenceLevel() { return coherenceLevel; }
int Session::getBreathPace() { return breathPace; }

// setters
void Session::setBreathPace(int breathPace) { this->breathPace = breathPace; }
