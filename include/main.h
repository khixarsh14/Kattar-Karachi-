#pragma once

// screen
const int   SCREEN_W          = 1152;
const int   SCREEN_H          = 528;
const int   SCALE             = 3;

// world
const int   WORLD_W           = 6912;

// layer Y positions (display px)
const int   SKY_Y             = 0;
const int   SKY_H             = 186;
const int   BUILDING_Y        = 186;
const int   BUILDING_H        = 222;
const int   FOOTPATH_Y        = 408;
const int   FOOTPATH_H        = 72;
const int   ROAD_Y            = 480;
const int   ROAD_H            = 48;

// player
const float GROUND_Y          = 408.0f;
const float PLAYER_SPEED      = 200.0f;
const float PLAYER_JUMP       = -550.0f;
const float GRAVITY           = 1200.0f;
const float INVINCIBLE_TIME   = 1.5f;
const int   MAX_HEARTS        = 5;

// camera
const float CAM_OFFSET_RIGHT  = SCREEN_W / 4.0f;
const float CAM_OFFSET_LEFT   = SCREEN_W * 3.0f / 4.0f;
const float CAM_LERP          = 0.1f;

// world boundary walls
const float WORLD_LEFT_WALL   = SCREEN_W / 4.0f;
const float WORLD_RIGHT_WALL  = WORLD_W - (SCREEN_W / 4.0f);

// score
const int   SCORE_PER_TASK    = 1000;
const float SCORE_TIME_MULT   = 10.0f;