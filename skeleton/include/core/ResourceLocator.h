#pragma once

#include <filesystem>
#include <string>
#include <iostream>
#include <cerrno>


class ResourceLocator {
public:
    // Sets the project root (once, on startup)
    static void setProjectRoot(const std::filesystem::path& root);
    // Returns absolute path to project root
    static std::filesystem::path getProjectRoot();

    // Returns absolute path to shader file
    static std::filesystem::path getShaderPath(const std::string& name, const std::string& ext);
    // Returns absolute path to shaders folder
    static std::filesystem::path getShadersFolderPath();

    // Optionally add support for other asset types later
    static std::filesystem::path getTexturePath(const std::string& name);
    static std::filesystem::path getMeshPath(const std::string& name);

private:
    static std::filesystem::path s_projectRoot;
};

inline std::filesystem::path findProjectRoot() {
    auto path = std::filesystem::current_path();

    std::cout << "Searching for root containing 'build' folder: " << path << std::endl;

    for (int i = 0; i < 10; ++i) { // limit search depth
        if (std::filesystem::exists(path / "build")) {
            std::cout << "Found 'build' folder at: " << path << std::endl;
            return path;
        }
        if (path.has_parent_path())
            path = path.parent_path();
        else
            break;
    }

    std::cerr << "Warning: Could not find project root automatically. Using current directory.\n";
    return std::filesystem::current_path();
}