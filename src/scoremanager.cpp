#include "scoremanager.h"
#include "TaskManager.h"
#include "raylib.h"
#include "main.h"

ScoreManager::ScoreManager()
    : startTime(0), endTime(0), elapsedTime(0),
      finalScore(0), timerRunning(false)
{}


void ScoreManager::StartTimer()
{
    startTime     = (float)GetTime();
    timerRunning  = true;
}

void ScoreManager::StopTimer()
{
    endTime       = (float)GetTime();
    elapsedTime   = endTime - startTime;
    timerRunning  = false;
}


//  Returns live time while game is running.
//  Returns the fixed final time after StopTimer().

float ScoreManager::GetElapsedTime() const
{
    if (timerRunning)
        return (float)GetTime() - startTime;
    return elapsedTime;
}
//  Uses FRIEND ACCESS to read tm.completedCount directly.
//  Formula:  (completedTasks x 1000) - (elapsedTime x 10)
//  Score never goes below 0.

void ScoreManager::CalculateScore(const TaskManager& tm)
{
    
    int completed = tm.completedCount;

    finalScore = (completed * SCORE_PER_TASK) - (int)(elapsedTime * SCORE_TIME_MULT);

    if (finalScore < 0)
        finalScore = 0;
}

int ScoreManager::GetScore() const
{
    return finalScore;
}

bool ScoreManager::operator>(const ScoreManager& other) const
{
    return finalScore > other.finalScore;
}