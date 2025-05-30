#include "MessageList.h"

void MessageList::SetRegion(Rectangle bounds)
{
    MessageList::bounds = bounds;
    containerSize = bounds.y / styles.textSize;
    handler.SetRegion(bounds);
}

void MessageList::Draw()
{
    Vector2 screenDims = Vector2{(float)GetRenderWidth(), (float)GetRenderHeight()};
    DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, styles.backgroundColor);
    for (int i = 0; i < messages.size(); i++)
    {
        if (styles.fadeOut)
        {
            if (messages[i].fade.Val() < 0.99f)
                RenderMessage(i, messages[i].fade.Val(), screenDims);
        }
        else
        {
            RenderMessage(i, 0, screenDims);
        }
    }
    DrawRectangleLinesEx(bounds, styles.borderWidth, styles.borderColor);
}

void MessageList::AddMessage(std::string message)
{
    if (messages.size() > 0)
    {
        if (messages[messages.size() - 1].text == message)
            messages[messages.size() - 1].msgCount++;
        else
            messages.push_back({message, EaseIn(0.0f, 1.0f, 1.0f)});
    }
    else
    {
        messages.push_back({message, EaseIn(0.0f, 1.0f, 1.0f), 1});
    }
    messages[messages.size() - 1].fade.Val(0.0f);
    messages[messages.size() - 1].fade.On();
}

void MessageList::RenderMessage(int index, float fadeVal, Vector2 renderSize)
{
    Message msg = messages[index];
    std::string count = msg.msgCount > 1 ? " x" + std::to_string(msg.msgCount) : "";
    Vector2 textSize = MeasureTextEx(styles.msgFont, (msg.text + count).c_str(), styles.textSize, styles.textSpacing);
    DrawTextEx(styles.msgFont, (msg.text + count).c_str(), {bounds.x + styles.leftMargin, bounds.y + styles.textSize * (index + 0.5f) - textSize.y / 2.0f - (styles.scrollable ? scrollPos : 0)}, styles.textSize, styles.textSpacing, {styles.textColor.r, styles.textColor.g, styles.textColor.b, (unsigned char)(styles.textColor.a * (1.0f - fadeVal))});
}

void MessageList::Update(float deltaTime)
{
    if (styles.fadeOut)
        for (Message& msg : messages)
            msg.fade.Update(deltaTime);

    opacity.Update(deltaTime, handler.Hover());
}
