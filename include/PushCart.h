#pragma once
#include "obstacle.h"



//  Touching it costs the player 1 heart.

//  IsLethal is NOT overridden â€” the base class default of
//  false is exactly what we want here.

class PushCart : public Obstacle {
public:
    
    PushCart(Vector2 pos);

    
    ~PushCart();

    void Update(float dt) override;   
    void Draw()           override;   
   
    int  GetDamage()      override;

    
};