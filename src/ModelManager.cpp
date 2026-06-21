#include "ModelManager.hpp"
#include "ConfigManager.hpp"
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <cstdlib>

namespace fs = std::filesystem;

void ModelManager::listModels() {
    std::string model_dir = ConfigManager::getInstance().getModelDirectory();
    
    if (!fs::exists(model_dir)) {
        std::cerr << "Error: Model directory does not exist: " << model_dir << "\n";
        return;
    }

    std::cout << std::left << std::setw(40) << "Filename" << "Size (GB)" << "\n";
    std::cout << std::string(55, '-') << "\n";

    bool found = false;
    for (const auto& entry : fs::directory_iterator(model_dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".gguf") {
            found = true;
            double size_gb = static_cast<double>(fs::file_size(entry.path())) / (1024.0 * 1024.0 * 1024.0);
            std::cout << std::left << std::setw(40) << entry.path().filename().string() 
                      << std::fixed << std::setprecision(2) << size_gb << " GB\n";
        }
    }

    if (!found) {
        std::cout << "No .gguf models found in " << model_dir << "\n";
    }
}

void ModelManager::downloadModel(const std::string& repo_id) {
    std::string model_dir = ConfigManager::getInstance().getModelDirectory();
    
    // Ensure the model directory exists
    if (!fs::exists(model_dir)) {
        std::cout << "Creating model directory: " << model_dir << "\n";
        fs::create_directories(model_dir);
    }

    std::cout << "Downloading model from " << repo_id << " to " << model_dir << "\n";
    
    // Example command: huggingface-cli download <repo_id> --local-dir <model_dir> --include "*.gguf"
    std::string command = "huggingface-cli download " + repo_id + " --local-dir " + model_dir + " --include \"*.gguf\"";
    
    int result = std::system(command.c_str());
    if (result == 0) {
        std::cout << "Download completed successfully.\n";
    } else {
        std::cerr << "Error: Download failed with exit code " << result << "\n";
    }
}

void ModelManager::deleteModel(const std::string& filename) {
    std::string model_dir = ConfigManager::getInstance().getModelDirectory();
    fs::path file_path = fs::path(model_dir) / filename;

    if (fs::exists(file_path)) {
        if (fs::remove(file_path)) {
            std::cout << "Successfully deleted " << filename << "\n";
        } else {
            std::cerr << "Error: Could not delete " << filename << "\n";
        }
    } else {
        std::cerr << "Error: File not found: " << file_path.string() << "\n";
    }
}
