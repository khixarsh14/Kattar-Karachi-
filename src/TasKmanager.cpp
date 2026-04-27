#include "TaskManager.h"
//  Shop ID mapping  (must match shopBounds[] order in Map)
//   0 = Zainab Market     (Task 1  Buy Eid clothes)
//   1 = Bun Kebab stall   (Task 2  Pick up bun kebab)
//   2 = Medical store     (Task 3  Get Panadol)

TaskManager::TaskManager()
    : currentIndex(0), completedCount(0)
{
    // Tasks are populated in Init(), not the constructor,
    // because Init() is called by Game after everything is set up.
}


void TaskManager::Init()
{
    tasks.clear();

    // Task 1 destination: Zainab Market (shopID 0)
    tasks.push_back(Task("Buy Eid clothes from Zainab Market", 0));

    // Task 2  destination: Bun Kebab stall (shopID 1)
    // After pickup, UIManager will show the bun-kebab warning message
    tasks.push_back(Task("Pick up bun kebab from Shah Jee BUN KEBAB", 1));

    // Task 3  destination: Medical store (shopID 2)
    tasks.push_back(Task("Get Panadol from medical store", 2));
}


const Task& TaskManager::GetCurrentTask() const
{
    return tasks[currentIndex];
}

void TaskManager::CompleteCurrentTask()
{
    if (currentIndex < (int)tasks.size())
    {
        tasks[currentIndex].Complete();
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

int TaskManager::GetActiveShopID() const
{
    if (currentIndex < (int)tasks.size())
        return tasks[currentIndex].GetTargetShopID();
    return -1; 
}
//  UIManager draws this below the task text while the task is active.
std::string TaskManager::GetSpecialMessage() const
{
    if (currentIndex < (int)tasks.size())
    {
        
        if (tasks[currentIndex].GetTargetShopID() == 1)
            return "Don't drop the bun kebab (avoid obstacles!)";
    }
    return "";
}