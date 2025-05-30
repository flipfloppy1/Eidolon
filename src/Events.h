#pragma once
#include "raylib.h"

template<typename T = bool>
// Primitive for measuring changes in boolean events that are polled, typically per-frame
class EventStream
{
  private:
    T prev = false;
    T curr = false;

  public:
    // Set the previous to the current. Should be executed in the update loop before anything else
    void UpdatePrev() { prev = curr; }

    // Set the current value
    void SetCurr(T newCurr) { curr = newCurr; }
    // Getter for previous value
    bool Prev() { return prev; }
    // Getter for current value
    bool Curr() { return curr; }
    // Whether the event has just begun
    bool OnStart() { return curr && !prev; }
    // Whether the event has just ended
    bool OnEnd() { return prev && !curr; }
};

// Used to detect mouse events in a given region of the window. Aggregates EventStreams that are bound by a region
class MouseHandler
{
  public:
    MouseHandler(): MouseHandler(0,0,0,0) {}
    MouseHandler(int x, int y, int width, int height)
    {
        region = Rectangle{(float)x, (float)y, (float)width, (float)height};
    }
    MouseHandler(Rectangle region) { MouseHandler::region = region; }

    // Should be run per-frame
    void Update()
    {
        hover.UpdatePrev();
        leftClick.UpdatePrev();
        midClick.UpdatePrev();
        rightClick.UpdatePrev();

        hover.SetCurr(GetMouseX() < region.x + region.width && GetMouseX() > region.x &&
                      GetMouseY() < region.y + region.height && GetMouseY() > region.y);
        leftClick.SetCurr(IsMouseButtonDown(MOUSE_BUTTON_LEFT));
        midClick.SetCurr(IsMouseButtonDown(MOUSE_BUTTON_MIDDLE));
        rightClick.SetCurr(IsMouseButtonDown(MOUSE_BUTTON_RIGHT));

    }

    // Whether the mouse is hovering in the region
    bool Hover() { return hover.Curr(); }

    // Whether the given mouse button has been clicked in the region
    bool OnClick(MouseButton buttonType)
    {
        switch (buttonType)
        {
        default:
            return false;
        case MOUSE_BUTTON_LEFT:
            return leftClick.OnStart() && hover.Curr();
        case MOUSE_BUTTON_MIDDLE:
            return midClick.OnStart() && hover.Curr();
        case MOUSE_BUTTON_RIGHT:
            return rightClick.OnStart() && hover.Curr();
        }
    }

    // Whether the given mouse button is down in the region
    bool IsDown(MouseButton buttonType)
    {
        switch (buttonType)
        {
        default:
            return false;
        case MOUSE_BUTTON_LEFT:
            return leftClick.Curr() && hover.Curr();
        case MOUSE_BUTTON_MIDDLE:
            return midClick.Curr() && hover.Curr();
        case MOUSE_BUTTON_RIGHT:
            return rightClick.Curr() && hover.Curr();
        }
    }

    // Whether the given mouse button was just released in the region
    bool OnRelease(MouseButton buttonType)
    {
        switch (buttonType)
        {
        default:
            return false;
        case MOUSE_BUTTON_LEFT:
            return leftClick.OnEnd() && !hover.OnEnd() && hover.Curr();
        case MOUSE_BUTTON_MIDDLE:
            return midClick.OnEnd() && !hover.OnEnd() && hover.Curr();
        case MOUSE_BUTTON_RIGHT:
            return rightClick.OnEnd() && !hover.OnEnd() && hover.Curr();
        }
    }

    // Setter for region
    void SetRegion(Rectangle region) { MouseHandler::region = region; }
    // Setter for region
    void SetRegion(int x, int y, int width, int height)
    {
        MouseHandler::region = Rectangle{(float)x, (float)y, (float)width, (float)height};
    }
    Rectangle GetRegion() { return region; };

  private:
    Rectangle region;
    EventStream<> leftClick;
    EventStream<> midClick;
    EventStream<> rightClick;
    EventStream<> hover;
};

class KeyHandler
{
    public:
    KeyHandler(): KeyHandler(0) {}
    KeyHandler(int key): KeyHandler(key, 20, 4) {}
    KeyHandler(int key, int repeatWait, int repeatFrequency)
    {
        KeyHandler::key = key;
        KeyHandler::repeatWait = repeatWait;
        KeyHandler::repeatFrequency = repeatFrequency;
        eventCount = 0;
    }

    void Update()
    {
        eventHandler.UpdatePrev();

        bool keyDown = IsKeyDown(key);
        eventHandler.SetCurr(keyDown);
        if (keyDown)
            eventCount++;
        else
            eventCount = 0;
    }

    bool OnRepeat()
    {
        return eventCount >= repeatWait && eventCount % repeatFrequency == 0;
    }

    bool OnPress()
    {
        return eventHandler.OnStart();
    }

    bool IsDown()
    {
        return eventHandler.Curr();
    }

    bool OnRelease()
    {
        return eventHandler.OnEnd();
    }

    private:
    int key;
    int eventCount;
    int repeatWait;
    int repeatFrequency;
    EventStream<> eventHandler;
};
