#pragma once
#include <string>
using namespace std;


struct Task {
    string description;
    float targetX;
    bool isCompleted;

    Task();
    Task(string desc, float x);
};