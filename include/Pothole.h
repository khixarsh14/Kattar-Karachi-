#pragma once
#include "obstacle.h"


//  A gutter/pothole on the Karachi footpath. If the player
//  touches it, it is instant death regardless of hearts.
//  It is completely static it never moves.

class Pothole : public Obstacle {
public:
    
    Pothole(Vector2 pos);

    
    ~Pothole();

    
    void Update(float dt) override;   
    void Draw()           override;   

    // Returns 999 this is the "instant kill" sentinel value.
    // Game checks IsLethal() first, but the damage value is also set high so even if IsLethal is somehow bypassed, the player still dies.
    int  GetDamage()      override;

    
    // Returns true  tells Game to trigger HandleGameOver()
    // immediately without deducting individual hearts.
    bool IsLethal()       override;
};