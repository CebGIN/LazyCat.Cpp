#include "ExecutionEngine.hpp"
#include "ConfigManager.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

void ExecutionEngine::chat(const std::string& filename) {
    const auto& config = ConfigManager::getInstance();
    std::string model_path = fs::path(config.getModelDirectory()) / filename;

    if (!fs::exists(model_path)) {
        std::cerr << "Error: Model not found at " << model_path << "\n";
        return;
    }

    // Construct the command
    // e.g. llama-cli -m <path> -t <threads> -c <context_limit>
    std::string command = "llama-cli -m " + model_path + 
                          " -t " + std::to_string(config.getNumThreads()) + 
                          " -c " + std::to_string(config.getContextLimit());

    std::cout << "Starting chat with " << filename << "...\n";
    std::cout << "Command: " << command << "\n";

    // Use std::system to run in the foreground
    int result = std::system(command.c_str());
    if (result != 0) {
        std::cerr << "llama-cli exited with code " << result << "\n";
    }
}

void ExecutionEngine::serve(const std::string& filename) {
    const auto& config = ConfigManager::getInstance();
    std::string model_path = fs::path(config.getModelDirectory()) / filename;

    if (!fs::exists(model_path)) {
        std::cerr << "Error: Model not found at " << model_path << "\n";
        return;
    }

    std::string env_file_path = "/etc/lazycat/env";
    
    // Ensure directory exists for env file
    fs::path env_dir = fs::path(env_file_path).parent_path();
    if (!fs::exists(env_dir)) {
        // Need to create directory but we might need sudo for /etc.
        // We will attempt it, but standard system commands or sudo might be needed depending on context.
        // Assuming the app runs with sufficient privileges or the user will handle /etc permissions.
        try {
            fs::create_directories(env_dir);
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Warning: Could not create directory " << env_dir << ": " << e.what() << "\n";
            std::cerr << "You may need to run this command with sudo.\n";
        }
    }

    // Write to /etc/lazycat/env
    std::ofstream env_file(env_file_path);
    if (!env_file.is_open()) {
        std::cerr << "Error: Could not open " << env_file_path << " for writing. Try running with sudo.\n";
        return;
    }

    env_file << "MODEL_PATH=" << model_path << "\n";
    env_file << "THREADS=" << config.getNumThreads() << "\n";
    env_file << "CONTEXT=" << config.getContextLimit() << "\n";
    env_file.close();

    std::cout << "Environment configured in " << env_file_path << "\n";

    // Restart the systemd service
    std::string command = "systemctl restart llama-server.service";
    std::cout << "Executing: " << command << "\n";
    
    int result = std::system(command.c_str());
    if (result == 0) {
        std::cout << "🟢 Servicio iniciado exitosamente (Service restarted).\n";
    } else {
        std::cerr << "Error: Failed to restart systemd service (code " << result << ").\n";
    }
}
