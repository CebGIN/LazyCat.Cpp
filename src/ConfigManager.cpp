#include "ConfigManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

ConfigManager::ConfigManager() {
    // Default values
    model_directory = "/root/models/";
    context_limit = 4096;
    num_threads = 4;
    no_webui = false;
    
    loadConfig();
}

std::string ConfigManager::getModelDirectory() const { return model_directory; }
void ConfigManager::setModelDirectory(const std::string& dir) { model_directory = dir; }

int ConfigManager::getContextLimit() const { return context_limit; }
void ConfigManager::setContextLimit(int limit) { context_limit = limit; }

int ConfigManager::getNumThreads() const { return num_threads; }
void ConfigManager::setNumThreads(int threads) { num_threads = threads; }

bool ConfigManager::getNoWebui() const { return no_webui; }
void ConfigManager::setNoWebui(bool no_webui) { this->no_webui = no_webui; }

void ConfigManager::loadConfig() {
    std::ifstream file(config_file);
    if (!file.is_open()) {
        // If it doesn't exist, create it with default values
        saveConfig();
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string key;
        if (std::getline(iss, key, '=')) {
            std::string value;
            if (std::getline(iss, value)) {
                if (key == "model_directory") model_directory = value;
                else if (key == "context_limit") context_limit = std::stoi(value);
                else if (key == "num_threads") num_threads = std::stoi(value);
                else if (key == "no_webui") no_webui = (value == "true" || value == "1");
            }
        }
    }
}

void ConfigManager::saveConfig() const {
    std::ofstream file(config_file);
    if (file.is_open()) {
        file << "model_directory=" << model_directory << "\n";
        file << "context_limit=" << context_limit << "\n";
        file << "num_threads=" << num_threads << "\n";
        file << "no_webui=" << (no_webui ? "true" : "false") << "\n";
    } else {
        std::cerr << "Error: Could not open " << config_file << " for writing.\n";
    }
}
