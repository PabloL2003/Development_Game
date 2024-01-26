#include "GUIControlValueBox.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"

// Constructor
GUIControlValueBox::GUIControlValueBox(uint32 id, SDL_Rect bounds, const char* text):GUIControl(GUIControlType::VALUEBOX, id), value(text)
{
	this->bounds = bounds;
}

// Destructor
GUIControlValueBox::~GUIControlValueBox()
{

}

// Update method
bool GUIControlValueBox::Update(float dt)
{
    if (state != GUIControlState::DISABLED)
    {
        int mouseX, mouseY;
        app->input->GetMousePosition(mouseX, mouseY);

        // Check if the mouse is over the value box
        if (mouseX > bounds.x && mouseX < bounds.x + bounds.w &&
            mouseY > bounds.y && mouseY < bounds.y + bounds.h)
        {
            state = GUIControlState::FOCUSED;

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
            {
                // Here you can handle the click event, perhaps to start editing the value
            }
        }
        else
        {
            state = GUIControlState::NORMAL;
        }

       /* switch (state)
        {
        case GUIControlState::DISABLED:
            app->render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);
            break;
        case GUIControlState::NORMAL:
            app->render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
            break;
        case GUIControlState::FOCUSED:
            app->render->DrawRectangle(bounds, 0, 0, 20, 255, true, false);
            break;
        case GUIControlState::PRESSED:
            app->render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
            break;
        }*/
        // app->render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
        app->render->DrawText(value.c_str(), bounds.x, bounds.y, bounds.w / 2, bounds.h);

    }

    return false;
}

void GUIControlValueBox::SetValue(const std::string& value)
{
    this->value = value;
}

const std::string& GUIControlValueBox::GetValue() const
{
    return value;
}