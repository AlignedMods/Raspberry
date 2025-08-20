#include "menu.hpp"
#include "log.hpp"
#include "types.hpp"
#include "game.hpp"
#include "registry/registry.hpp"
#include "renderer/renderer.hpp"

#include "raylib.h"

static Menu* current = nullptr;
static bool pressing = false;

// Callbacks
static void M_StartGame();
static void M_Options();
static void M_QuitGame();

static void Q_Quit();
static void Q_Return();

static void Q_Update();
static void Q_Render();

static void P_Continue();

static void MoveCursor(bool up);
static void MoveCursorToIndex(u16 index);
static void ExecuteCallback();

static MenuItem MainMenu[] = {
    {1, "Play",     M_StartGame, 'p'},
    {1, "Settings", M_Options, 's'},
    {1, "Quit",     M_QuitGame, 'q'},
};

static MenuItem QuitMenu[] {
    {0, "Are you sure you wish to quit?", nullptr, 0, 30},
    {1, "Yes",                            Q_Quit, 0, 40},
    {1, "No",                             Q_Return, 0, 40}
};

static MenuItem PauseMenu[] {
    {0, "--PAUSED--", nullptr, 50},
    {1, "Continue",   P_Continue, 40},
    {1, "Quit",       M_QuitGame, 40}
};

static Menu MainMenuDef = {
    3,
    MainMenu,
    nullptr,
    0,
};

static Menu QuitMenuDef {
    3,
    QuitMenu,
    nullptr,
    1
};

static Menu PauseMenuDef {
    3,
    PauseMenu,
    nullptr,
    1
};

void M_StartGame() {
    Log(Log::Info, "Pressed Play!");
    Game.StartGameplay();
    SwitchMenu(nullptr);
}

void M_StartEditor() {}
void M_Options() {}

void M_QuitGame() {
    SwitchMenu(&QuitMenuDef);
}

void Q_Quit() {
    Game.Quit();
}

void Q_Return() {
    SwitchMenu(current->prevMenu);
}

void P_Continue() {
    Game.SetPause(false);
    SwitchMenu(nullptr);
}

void MoveCursor(bool up) {
    if (up) {
        if (current->selection > 0) {
            MoveCursorToIndex(current->selection - 1);
        }
    } else {
        if (current->selection < current->numItems) {
            MoveCursorToIndex(current->selection + 1);
        }
    }
}

void MoveCursorToIndex(u16 index) {
    u16 last = current->selection;

    if (current->items[index].type > 0) {
        current->selection = index;
        current->items[index].timeSinceSelect = 0.0f;
    }

    if (last != current->selection) {
        Registry.PlaySound("menu_cycle", 0.9f, 1.1f);
    }
}

void ExecuteCallback() {
    current->items[current->selection].callback();
    Registry.PlaySound("menu_select", 0.8f, 1.3f);
}

void InitMenu() {
    SwitchMenu(&MainMenuDef);
}

void UpdateCurrentMenu() {
    if (current) {
        u32 key = GetKeyPressed();
        char c = GetCharPressed();

        current->items[current->selection].timeSinceSelect += Game.deltaTime;

        if (key == KEY_DOWN) {
            MoveCursor(false);
        }

        if (key == KEY_UP) {
            MoveCursor(true);
        }

        if (key == KEY_ENTER) { 
            ExecuteCallback();
        }

        if (c) {
            for (u16 i = 0; i < current->numItems; i++) {
                if (current->items[i].shortcut == c) {
                    MoveCursorToIndex(i);
                }
            }
        }

        // optional gamepad (controller) navigation
        // i actually prefer it for some reason
        // now i have a controller next to my setup always D:
        if (IsGamepadAvailable(0)) {
            f32 axis = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);

            if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
                MoveCursor(false);
            }

            if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
                MoveCursor(true);
            }

            if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
                ExecuteCallback();
            }
        }
    }

    if (Game.m_GameRunning) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            SwitchMenu(&PauseMenuDef);
            Game.SetPause(true);
        }

        if (IsGamepadAvailable(0)) {
            if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) {
                SwitchMenu(&PauseMenuDef);
                Game.SetPause(true);
            }
        }
    }

    if (current) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            if (current->prevMenu) {
                SwitchMenu(current->prevMenu);
            }
        }

        if (IsGamepadAvailable(0)) {
            Log(Log::Info, "gmapad available1");
            if (IsGamepadButtonPressed(0, 6)) {
                if (current->prevMenu) {
                    SwitchMenu(current->prevMenu);
                }
            }
        }
    } // holy
}

void RenderCurrentMenu() {
    if (current) {
        f32 height = 0.0f;
        f32 y = 0.0f;

        for (int i = 0; i < current->numItems; i++) {
            MenuItem* item = &current->items[i];

            height += item->fontSize - 10;
        }

        // centering the menu
        y = (GetScreenHeight() - height) / 2.0f;

        for (u16 i = 0; i < current->numItems; i++) {
            u64 ticks = Game.ticks;

            MenuItem* item = &current->items[i];
            const char* text = item->text;

            if (current->selection == i) {
                // make sure that if you switch the cursor is visible no matter what for some time
                if (ticks % 40 > 20 || item->timeSinceSelect < 0.5f) {
                    text = TextFormat("> %s <", text);
                }
            }

            Renderer.RenderText(text, {GetScreenWidth() / 2.0f, y}, item->fontSize);
            y += item->fontSize + 10.0f;
        }
    }
}

void SwitchMenu(Menu* menu) {
    if (current != nullptr && menu != nullptr) {
        menu->prevMenu = current;
    }

    current = menu;
}
