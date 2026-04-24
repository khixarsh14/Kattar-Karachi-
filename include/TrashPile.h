#pragma once
#include "obstacle.h"


class TrashPile : public Obstacle {
public:
    
    TrashPile(Vector2 pos);

    ~TrashPile();
   
    void Update(float dt) override;   
    void Draw()           override;   

    int  GetDamage()      override;
};