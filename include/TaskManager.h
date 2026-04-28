#pragma once
#include <vector>
#include "Task.h"

class ScoreManager;

class TaskManager {
private:
    std::vector<Task> tasks;
    int currentIndex;
    int completedCount;

    friend class ScoreManager;

public:
    TaskManager();

    void Init();

    const Task& GetCurrentTask() const;
    float GetCurrentTargetX() const;

    void CompleteCurrentTask();

    bool AllTasksDone() const;
    int GetCompletedCount() const;
    int GetTotalTasks() const;
};