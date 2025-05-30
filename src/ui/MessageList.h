#pragma once

#include "Events.h"
#include "Interp.h"
#include "raylib.h"
#include <string>
#include <vector>

struct MessageListStyling
{
    Font msgFont;
    bool fadeOut = true;
    bool scrollable = true;
    bool hideBorder = true;
    int scrollWidth = 10;
    int textSize = 16;
    int borderWidth = 1;
    int leftMargin = 0;
    float textSpacing = 1.0f;
    Color backgroundColor = Color{0,0,0,0};
    Color scrollbarColor = Color{255,255,255,50};
    Color textColor = Color{255,255,255,255};
    Color borderColor = Color{255,255,255,255};
};

class MessageList
{
  public:
    MessageList(Rectangle bounds, MessageListStyling styles) : bounds(bounds), styles(styles)
    {
        handler.SetRegion(bounds);
        containerSize = bounds.y / styles.textSize;
        lastMsgSize = 0;
        scrollPos = 0;
    }
    void Draw();
    void Update(float deltaTime);
    void AddMessage(std::string message);
    void SetRegion(Rectangle bounds);

  private:
    struct Message
    {
        std::string text;
        EaseIn fade;
        int msgCount = 1;
    };
    void RenderMessage(int index, float fadeVal, Vector2 renderSize);
    std::vector<Message> messages;
    int lastMsgSize;
    int containerSize;
    float scrollPos;
    Rectangle bounds;
    EaseIn opacity;
    MessageListStyling styles;
    MouseHandler handler;
};
