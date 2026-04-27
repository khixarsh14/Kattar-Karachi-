#pragma once
#include "raylib.h"

class Obstacle {
protected:
    Vector2    position;   
    float      width;      
    float      height;     
    bool       isActive;   // If false: skip update, draw, and collision
    Texture2D  texture;    

public:
    bool isPlayerTouching = false;
    
    Obstacle(Vector2 pos, float w, float h);   // Called by every subclass ctor
    virtual ~Obstacle();                        // MUST be virtual â€” safe delete via base ptr

    
    
    
    virtual void Update(float dt) = 0;         // Called every frame
    virtual void Draw()           = 0;         // Render the obstacle
    virtual int  GetDamage()      = 0;         // Hearts to remove on collision

    
    virtual bool      IsLethal();              // Returns false. Pothole overrides to true.
    virtual Rectangle GetBounds();             // Returns {position.x, position.y, width, height}

    
    bool    IsActive()            const;       // Returns isActive
    void    SetActive(bool active);            // Enable / disable this obstacle
    Vector2 GetPosition()         const;       // Returns position
};