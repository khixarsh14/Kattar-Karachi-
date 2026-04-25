#pragma once
#include <string>

class Task {
private:
    std::string description;   
    int  targetShopID;  
    bool isCompleted;   

public:
    Task();
   
    Task(std::string desc, int shopID);

    void Complete();

    bool        IsCompleted()    const;
    std::string GetDescription() const;
    int         GetTargetShopID()const;

    bool operator==(const Task& other) const;
};