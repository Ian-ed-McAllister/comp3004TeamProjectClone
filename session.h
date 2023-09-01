#ifndef SESSION_H
#define SESSION_H

#include <QVector>
#include <QDebug>
class Session {

public:
    Session();

    void detectCoherenceChange();
    void addCoherenceScore(float coherence);
    void updateCoherenceAverage();
    void updateCoherenceSummary();
    void updateCoherenceLevel();
    void updateAchievementScore();
    void reset();


    // getters
    float getAchievementScore();
    float getCoherenceAverage();
    float* getCoherenceSummary();
    QVector<float> getCoherenceScores();
    QString getCoherenceLevel();
    int getBreathPace();

    // setters
    void setBreathPace(int breathPace);

private:
    int breathPace;
    QString coherenceLevel;
    float achievementScore;
    float coherenceAverage;
    float coherenceSummary [3];
    QVector<float> coherenceScores;
};

#endif // SESSION_H
