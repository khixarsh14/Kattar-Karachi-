#include "TaskManager.h"

TaskManager::TaskManager()
    : currentIndex(0), completedCount(0)
{}

void TaskManager::Init()
{
    tasks.clear();

    // CORRECT ORDER: Medical (509) → Zainab (75) → Bun Kebab (880)
    tasks.push_back(Task("Get Panadol from Medical Store", 509));
    tasks.push_back(Task("Buy EID clothes from Zainab Market", 75));
    tasks.push_back(Task("Pick up Bun Kabab", 880));

    currentIndex = 0;
    completedCount = 0;
}

const Task& TaskManager::GetCurrentTask() const
{
    if (currentIndex < (int)tasks.size())
        return tasks[currentIndex];
    return tasks.back();  // safe fallback when all done
}

float TaskManager::GetCurrentTargetX() const
{
    if (currentIndex < (int)tasks.size())
        return tasks[currentIndex].targetX;
    return -1;  // no active task
}

void TaskManager::CompleteCurrentTask()
{
    if (currentIndex < (int)tasks.size())
    {
        tasks[currentIndex].isCompleted = true;
        currentIndex++;
        completedCount++;
    }
}

bool TaskManager::AllTasksDone() const
{
    return currentIndex >= (int)tasks.size();
}

int TaskManager::GetCompletedCount() const
{
    return completedCount;
}

int TaskManager::GetTotalTasks() const
{
    return (int)tasks.size();
}