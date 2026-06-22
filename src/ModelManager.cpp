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

void ModelManager::downloadModel(const std::string& repo_id, const std::string& quant) {
    std::string model_dir = ConfigManager::getInstance().getModelDirectory();
    
    if (!fs::exists(model_dir)) {
        std::cout << "Creating model directory: " << model_dir << "\n";
        fs::create_directories(model_dir);
    }

    // Determinar qué cuantización usar (Parámetro o por defecto)
    std::string chosen_quant = quant.empty() ? "Q4_K_M" : quant;

    std::cout << "=======================================================\n";
    std::cout << " Iniciando descarga con cliente local 'hf'\n";
    std::cout << " Repositorio:   " << repo_id << "\n";
    std::cout << " Cuantización:  " << chosen_quant << "\n";
    std::cout << " Destino:       " << model_dir << "\n";
    std::cout << "=======================================================\n";
    
    // Construcción dinámica del comando hf inyectando la variable chosen_quant
    std::string command = "hf download " + repo_id + " --include \"*" + chosen_quant + ".gguf\" --local-dir " + model_dir;
    
    std::cout << "Ejecutando: " << command << "\n";
    
    int result = std::system(command.c_str());
    if (result == 0) {
        std::cout << "\n🟢 Descarga finalizada exitosamente.\n";
    } else {
        std::cerr << "\n🔴 Error: El comando 'hf' falló con código de salida " << result << "\n";
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
