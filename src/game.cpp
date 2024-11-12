#include "game.hpp"

#include "collectables/collectable.hpp"
#include "core/core.hpp"
#include "core/random.hpp"
#include "graphics/renderer.hpp"
#include "menu/menu.hpp"
#include "player/player.hpp"
#include "raylib.h"
#include "tile/tile.hpp"

#include <pch.hpp>

using std::make_shared;
using std::shared_ptr;

static Game* s_Instance;
static shared_ptr<Renderer> s_Renderer;

Game::Game()
{
    if (!s_Instance) {
        s_Renderer = make_shared<Renderer>();
        s_Instance = this;
    } else {
        Error(
            "Game class already exists!\nThere cannot be multiple game classes "
            "at the same time!",
            1);
    }
    SetCurrentLevel(make_shared<Level>());
}

Game::~Game() { }

void Game::Run()
{
    const int windowWidth = 21 * 64;
    const int windowHeight = 12 * 64;

    InitWindow(windowWidth, windowHeight, "Raspberry");

    Random::Init();

    SetTargetFPS(60);

    Menu menu;
    menu.InitFonts();

    // for some reason i have to explictly show the menu
    menu.Show();

    Camera2D camera = { 0 };
    camera.zoom = 1.0f;
    camera.offset = { windowWidth / 2.0f, windowHeight / 2.0f };
    camera.target = { 0.0f, 0.0f };
    camera.rotation = 0.0f;

    while (!WindowShouldClose()) {
        if (m_IsGamePlayRunning) {
            menu.Hide();

            m_CurrentLevel->GetPlayer()->Update();

            camera.target = { m_CurrentLevel->GetPlayer()->GetX(),
                m_CurrentLevel->GetPlayer()->GetY() };

            m_CurrentLevel->GetCollectable().Update();
        }

        menu.Update();

        BeginDrawing();

        ClearBackground(WHITE);

        if (m_IsGamePlayRunning) {
            BeginMode2D(camera);

            m_CurrentLevel->GetPlayer()->Draw();

            for (Tile tile : m_CurrentLevel->GetAllTiles()) {
                tile.Draw();
            }

            if (m_CurrentLevel->IsCollectableFound()) {
                DrawText("Found raspberry", 400, 600, 20, GREEN);
            }

            m_CurrentLevel->GetCollectable().Draw();

            EndMode2D();
        }

        menu.Draw();

        EndDrawing();
    }
}

void Game::SetCurrentLevel(shared_ptr<Level> level)
{
    m_CurrentLevel = level;
}

shared_ptr<Level> Game::GetCurrentLevel()
{
    return Game::Get()->m_CurrentLevel;
}

void Game::StartGameplay()
{
    m_IsGamePlayRunning = true;

    m_CurrentLevel->AddPlayer();

    m_CurrentLevel->AddCollectable();
    m_CurrentLevel->GetCollectable().InitTextures();

    m_CurrentLevel->LoadLevelFromFile("hi.lvl");
}

Game* Game::Get()
{
    return s_Instance;
}

shared_ptr<Renderer> Game::GetRenderer()
{
    return s_Renderer;
}
