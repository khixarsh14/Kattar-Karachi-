#include "task.h"
using namespace std;

Task::Task()
    : description(""), targetX(0), isCompleted(false)
{}

Task::Task(string desc, float x)
    : description(desc), targetX(x), isCompleted(false)
{}