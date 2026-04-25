#pragma once
#include "raylib.h"


class TaskManager;

class ScoreManager {
private:
    float startTime;      
    float endTime;        
    float elapsedTime;    
    int   finalScore;     
    bool  timerRunning;   

public:
    ScoreManager();

    void StartTimer();                          
    void StopTimer();                           
    float GetElapsedTime() const;               // live time while running, fixed after stop
    void  CalculateScore(const TaskManager& tm);// uses friend access to read completedCount
    int   GetScore() const;               // returns finalScore

    bool operator>(const ScoreManager& other) const;
};