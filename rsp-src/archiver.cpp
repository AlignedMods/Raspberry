#include <ios>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>

static void PrintUsage();
static void PrintError(const char* message);
static void PrintHelp();

int main(int argc, char* argv[]) {
    if (argc < 2) {
        PrintUsage();
    }

    std::filesystem::path outputPath;
    std::filesystem::path inputDir(argv[1]);

    std::ofstream output;

    if (!std::filesystem::is_directory(inputDir)) {
        PrintError("Input provided is not a directory!");
    }

    if (inputDir.is_relative()) {
        inputDir.assign(std::filesystem::current_path() / inputDir);
    }

    if (argc > 2) {
        outputPath.assign(argv[2]);

        if (outputPath.is_relative()) {
            outputPath.assign(std::filesystem::current_path() / outputPath);
        }
    } else {
        outputPath.assign(std::filesystem::current_path() / "a.rsp");
    }

    output.open(outputPath, std::ios::binary);

    std::string directory;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(inputDir)) {
        if (std::filesystem::is_directory(entry)) {
            directory = entry.path().filename().string();
        } else {
            std::string extension = entry.path().extension().string();

            if (extension == ".png") {
                output << "texture\n";
            }

            if (extension == ".wav") {
                output << "sound\n";
            }

            if (extension == ".lvl") {
                output << "map\n";
            }

            if (extension == ".ttf") {
                output << "font\n";
            }

            if (extension == ".json") {
                output << "fuck\n";
            }

            if (extension == ".glsl") {
                output << "shader\n";
            }

            output << entry.path().stem().string() << '\n';
            output << entry.file_size() << '\n';

            std::stringstream t;
            std::ifstream file(entry.path());

            t << file.rdbuf();

            output << t.str() << '\n';
        }
    }

    output.close();

    std::cout << "Archived \"" << inputDir.string() << "\" -> \"" << outputPath.string() << "\"!\n";
}

void PrintUsage() {
    std::cout << "Usage: <INPUT> <OUTPUT>\n";
    exit(1);
}

void PrintError(const char* message) {
    std::cerr << "ERROR: " << message << '\n';
    exit(1);
}

void PrintHelp() {
    std::cout << "help\n";
}
