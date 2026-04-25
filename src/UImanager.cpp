
// PRIVATE attribute (add with your other private stuff):
//
//   float introTimer;
//   // counts down from 3 to 0 at game start. shows the urdu intro message.


// PUBLIC methods (add with your other public methods):
//
//   void DrawIntroMessage();
//   // draws "Traffic bohot hai, sambhal ke!" at top-centre for 3 seconds
//
//   void DrawSpecialMessage(const TaskManager& tm);
//   // draws the bun kebab warning while task 2 is active


// Also update DrawHUD signature to accept dt:
//   void DrawHUD(const Player& p, const TaskManager& tm, const ScoreManager& sm, float dt);

// In your constructor UIManager::UIManager(), add:
//   introTimer = 3.0f;


void UIManager::DrawIntroMessage()
{
    // draws the urdu flavour text at the top centre of the screen
    const char* msg = "Traffic bohot hai, sambhal ke!";
    int textWidth = MeasureText(msg, 24);
    DrawText(msg, (1152 / 2) - (textWidth / 2), 20, 24, YELLOW);
}



void UIManager::DrawSpecialMessage(const TaskManager& tm)
{
    // gets the special warning message from task manager
    // for task 2 (bun kebab) this returns "Don't drop the bun kebab (avoid obstacles!)"
    // for all other tasks it returns "" so nothing is drawn
    std::string msg = tm.GetSpecialMessage();
    if (!msg.empty())
    {
        DrawText(msg.c_str(), 20, 50, 20, ORANGE);
    }
}



void UIManager::DrawHUD(const Player& p, const TaskManager& tm, const ScoreManager& sm, float dt)
{
    // if intro timer is still running, show the urdu message instead of task
    if (introTimer > 0.0f)
    {
        introTimer -= dt;
        DrawIntroMessage();
    }
    else
    {
        DrawCurrentTask(tm);       // your existing function, no change
        DrawSpecialMessage(tm);    // new shows bun kebab warning if task 2 is active
    }

    // these stay exactly the same as before
    DrawHearts(p);
    DrawElapsedTime(sm);
}

//  SUMMARY  what to add in total:
//
//  UIManager.h:
//    private:  float introTimer;
//    public:   void DrawIntroMessage();
//    public:   void DrawSpecialMessage(const TaskManager& tm);
//    update:   DrawHUD signature to include float dt
//
//  UIManager.cpp:
//    constructor:    introTimer = 3.0f;
//    DrawIntroMessage():  draw "Traffic bohot hai, sambhal ke!" top-centre
//    DrawSpecialMessage(): get tm.GetSpecialMessage(), draw if not empty
//    DrawHUD():       add intro countdown logic + call DrawSpecialMessage