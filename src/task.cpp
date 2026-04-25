#include "Task.h"

Task::Task()
    : description(""), targetShopID(-1), isCompleted(false)
{}

Task::Task(std::string desc, int shopID)
    : description(desc), targetShopID(shopID), isCompleted(false)
{}

void Task::Complete()
{
    isCompleted = true;
}

bool Task::IsCompleted() const
{
    return isCompleted;
}

std::string Task::GetDescription() const
{
    return description;
}

int Task::GetTargetShopID() const
{
    return targetShopID;
}

bool Task::operator==(const Task& other) const
{
    return (description == other.description) && (targetShopID == other.targetShopID);
}