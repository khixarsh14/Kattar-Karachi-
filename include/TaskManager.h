#pragma once
#include <vector>
#include <string>
#include "Task.h"


// Friend Class (ScoreManager)


class ScoreManager;

class TaskManager {
private:
    std::vector<Task> tasks;         
    int currentIndex;   
    int completedCount; // How many tasks are done ScoreManager reads this via friend


    friend class ScoreManager;

public:
    TaskManager();

    void Init();

    // Returns a reference to the active task UIManager reads description from here
    Task& GetCurrentTask();
    
    void CompleteCurrentTask();
    
    bool AllTasksDone() const;

    int GetCompletedCount() const;

    int GetTotalTasks() const;

    int GetActiveShopID() const;

    // Returns the special warning message for a task, if any (e.g. bun kebab warning)
    // Returns "" if no special message for the active task
    std::string GetSpecialMessage() const;
};