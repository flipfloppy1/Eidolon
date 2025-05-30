#pragma once
#include "raylib.h"
#include "Events.h"
#include <string>

// Styling data for TextInputs, with reasonable defaults
struct TextInputStyling
{
    Font font;
    int fontSize = font.baseSize;
    int padding = 0;
    int lineCount = 1;
    bool elastic = false;
    Color backgroundColor = Color{0,0,0,255};
    Color borderColor = Color{255,255,255,255};
    Color caretColor = Color{255,255,255,255};
    float caretSpeed = 1.0f; // flashes per second
};

class TextInput
{
    public:
    TextInput(int width, TextInputStyling styles);
    TextInput(int width, std::string* val, TextInputStyling styles);

    void Update();

    private:
    int width;
    bool showCaret;
    bool hasFocus = false;
    EventStream<> mouse;
    double lastCaretChange;
    std::string* val;
    TextInputStyling styles;
    void ToggleCaret();
};
