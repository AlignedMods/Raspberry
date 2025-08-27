#pragma once

#include "application.hpp"

int main(int argc, char **argv) {
    Application *app = Application::CreateApplication(CommandLineArgs(argc, argv));
    app->Run();
    delete app;
}