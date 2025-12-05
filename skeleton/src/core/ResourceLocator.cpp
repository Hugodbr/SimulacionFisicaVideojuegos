#include "ResourceLocator.h"

#include <iostream>


std::filesystem::path ResourceLocator::s_projectRoot;

void ResourceLocator::setProjectRoot(const std::filesystem::path& root) 
{
    if (!std::filesystem::exists(root)) {
        std::cerr << "Error: project root path not found: " << root << std::endl;
        std::cerr << "Make sure the path is correct and contains the 'build' folder." << std::endl;
        return;
    }
    s_projectRoot = root;
}

std::filesystem::path ResourceLocator::getProjectRoot()
{
    if(s_projectRoot.empty()) {
        std::cerr << "Error: project root not set! Couldn't get project root path.\n";
        return {};
    }
    return s_projectRoot;
}

std::filesystem::path ResourceLocator::getShaderPath(const std::string& name, const std::string& ext) 
{
    if (s_projectRoot.empty()) {
        std::cerr << "Error: project root not set!\n";
        return {};
    }

    auto path = s_projectRoot / "resources" / "shaders" / (name + ext);
    if (!std::filesystem::exists(path)) {
        std::cerr << "Warning: shader not found: " << path << std::endl;
        std::cout << "Folder structure should be: <project_root>/resources/shaders/" << name + ext << std::endl;
    }
    return path;
}

std::filesystem::path ResourceLocator::getShadersFolderPath() 
{
    if (s_projectRoot.empty()) {
        std::cerr << "Error: project root not set!\n";
        return {};
    }

    auto path = s_projectRoot / "resources" / "shaders";
    if (!std::filesystem::exists(path)) {
        std::cerr << "Warning: shaders folder not found: " << path << std::endl;
        std::cout << "Folder structure should be: <project_root>/resources/shaders/" << std::endl;
    }
    return path;
}

std::filesystem::path ResourceLocator::getTexturePath(const std::string& name) 
{
    if (s_projectRoot.empty()) {
        std::cerr << "Error: project root not set!\n";
        return {};
    }

    auto path = s_projectRoot / "resources" / "textures" / name;
    if (!std::filesystem::exists(path)) {
        std::cerr << "Warning: texture not found: " << path << std::endl;
        std::cout << "Folder structure should be: <project_root>/resources/textures/" << name << std::endl;
    }
    return path;
}

std::filesystem::path ResourceLocator::getMeshPath(const std::string& name) 
{
    if (s_projectRoot.empty()) {
        std::cerr << "Error: project root not set!\n";
        return {};
    }

    auto path = s_projectRoot / "resources" / "meshes" / name;
    if (!std::filesystem::exists(path)) {
        std::cerr << "Warning: mesh not found: " << path << std::endl;
        std::cout << "Folder structure should be: <project_root>/resources/meshes/" << name << std::endl;
    }
    return path;
}