#pragma once

// screen
const int   SCREEN_W          = 1152;
const int   SCREEN_H          = 528;
const int   SCALE             = 3;

// world (for now = one background)
const int   WORLD_W           = 3072;

// ----- LAYERS (BASE → *3 = DISPLAY) -----
const int   SKY_Y             = 0;
const int   BUILDING_Y        = 45 * 3;    // 135

// FOOTPATH SYSTEM (your new design)
const int   FOOTPATH_TOP      = 141 * 3;   // 423
const int   FOOTPATH_BOTTOM   = 150 * 3;   // 450
const int   FOOTPATH_WALL_TOP = 151 * 3;   // 453
const int   ROAD_Y            = 159 * 3;   // 477

// ----- PLAYER -----
const float GROUND_Y          = 146 * 3.0f;   // ✅ 438 (your confirmed value)

const float PLAYER_SPEED      = 200.0f;
const float PLAYER_JUMP       = -550.0f;
const float GRAVITY           = 1200.0f;

const float INVINCIBLE_TIME   = 1.5f;
const int   MAX_HEARTS        = 5;

// ----- CAMERA -----
const float CAM_LERP          = 0.1f;

// ----- WORLD WALLS (match camera system) -----
const float WORLD_LEFT_WALL   = SCREEN_W / 2.0f;
const float WORLD_RIGHT_WALL  = WORLD_W - (SCREEN_W / 2.0f);

// ----- SCORE -----
const int   SCORE_PER_TASK    = 1000;
const float SCORE_TIME_MULT   = 10.0f;