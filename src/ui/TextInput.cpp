#include "TextInput.h"
#include "raylib.h"
#include <string>

TextInput::TextInput(int width, TextInputStyling styles): TextInput(width, new std::string, styles) {}
TextInput::TextInput(int width, std::string* val, TextInputStyling styles)
{
    TextInput::width = width;
    TextInput::val = val;
    TextInput::styles = styles;
    showCaret = false;
}

void TextInput::Update()
{
    double time = GetTime();
    if (styles.caretSpeed != 0.0f)
    {
        if (time >= lastCaretChange + 1.0f / styles.caretSpeed)
        {
            ToggleCaret();
        }
    }
}
void TextInput::ToggleCaret()
{
    showCaret = !showCaret;
    lastCaretChange = GetTime();
}
