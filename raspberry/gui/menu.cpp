#include "menu.hpp"
#include "application/application.hpp"
#include "event/key_events.hpp"
#include "game.hpp"
#include "input/keycodes.hpp"
#include "log.hpp"
#include "registry/registry.hpp"
#include "types.hpp"

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
    {1, "Play", M_StartGame, 'p'},
    {1, "Settings", M_Options, 's'},
    {1, "Quit", M_QuitGame, 'q'},
};

static MenuItem QuitMenu[]{
    {0, "Are you sure you wish to quit?", nullptr, 0, 30},
    {1, "Yes", Q_Quit, 0, 40},
    {1, "No", Q_Return, 0, 40}};

static MenuItem PauseMenu[]{
    {0, "--PAUSED--", nullptr, 50},
    {1, "Continue", P_Continue, 40},
    {1, "Quit", M_QuitGame, 40}};

static Menu MainMenuDef = {
    3,
    MainMenu,
    nullptr,
    0,
};

static Menu QuitMenuDef{
    3,
    QuitMenu,
    nullptr,
    1};

static Menu PauseMenuDef{
    3,
    PauseMenu,
    nullptr,
    1};

void M_StartGame() {
    Log(Log_Info, "Pressed Play!");
    Game* g = (Game*)Blackberry::Application::Get().GetLayerStack().GetLayer("game");
    // Log(Log_Info, "Layer ptr: %p", g);
    g->StartGameplay();
    SwitchMenu(nullptr);
}

void M_StartEditor() {}
void M_Options() {}

void M_QuitGame() {
    SwitchMenu(&QuitMenuDef);
}

void Q_Quit() {
    Blackberry::Application::Get().Close();
}

void Q_Return() {
    SwitchMenu(current->prevMenu);
}

void P_Continue() {
    Game* g = (Game*)Blackberry::Application::Get().GetLayerStack().GetLayer("game");
    g->SetPause(false);
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
        Registry& reg = Blackberry::Application::Get().GetRegistry();
        // PlaySound(reg.GetSound("menu_cycle"));
    }
}

void ExecuteCallback() {
    current->items[current->selection].callback();
    Registry& reg = Blackberry::Application::Get().GetRegistry();
    // PlaySound(reg.GetSound("menu_select"));
}

void InitMenu() {
    SwitchMenu(&MainMenuDef);
}

void UpdateCurrentMenu(const Blackberry::Event& event) {
    Game* g = (Game*)Blackberry::Application::Get().GetLayerStack().GetLayer("game");

    if (current) {
        if (event.GetEventType() == EventType::KeyPressed) {
            const Blackberry::KeyPressedEvent& pressed = EVENT_CAST(KeyPressedEvent);

            KeyCode key = pressed.GetKeyCode();

            if (key == KeyCode::Down || key == KeyCode::S) {
                MoveCursor(false);
            }

            if (key == KeyCode::Up || key == KeyCode::W) {
                MoveCursor(true);
            }

            if (key == KeyCode::Enter) {
                ExecuteCallback();
            }
        }
    }

    if (g->m_GameRunning) {
        if (event.GetEventType() == EventType::KeyPressed) {
            const Blackberry::KeyPressedEvent& pressed = EVENT_CAST(KeyPressedEvent);

            KeyCode key = pressed.GetKeyCode();

            if (key == KeyCode::Escape) {
                SwitchMenu(&PauseMenuDef);
                g->SetPause(true);
            }
        }
    }
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
        // y = (GetScreenHeight() - height) / 2.0f;
        y = 200.0f;

        for (u16 i = 0; i < current->numItems; i++) {
            // u64 ticks = Game.ticks;
            u64 ticks = 3;

            MenuItem* item = &current->items[i];
            const char* text = item->text;

            if (current->selection == i) {
                // make sure that if you switch the cursor is visible no matter what for some time
                if (ticks % 40 > 20 || item->timeSinceSelect < 0.5f) {
                    // text = TextFormat("> %s <", text);
                }
            }

            // Renderer.RenderText(text, {GetScreenWidth() / 2.0f, y}, item->fontSize);
            // DrawText(text, GetScreenWidth() / 2.0f, y, item->fontSize, WHITE);
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
