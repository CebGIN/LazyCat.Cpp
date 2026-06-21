#include "ModelManager.hpp"
#include "ExecutionEngine.hpp"
#include "LazyTUI.hpp"
#include <iostream>
#include <string>
#include <vector>

void printHelp() {
    std::cout << "LazyCat: Gestor Híbrido para Llama.cpp\n\n";
    std::cout << "Usage:\n";
    std::cout << "  lazycat list                  List all .gguf models in the configured directory\n";
    std::cout << "  lazycat download <repo_id>    Download a model from Hugging Face\n";
    std::cout << "  lazycat delete <filename>     Delete a specific .gguf model\n";
    std::cout << "  lazycat chat <filename>       Start an interactive chat with a model\n";
    std::cout << "  lazycat serve <filename>      Serve the model via systemd (llama-server.service)\n";
    std::cout << "  lazycat config | ui           Open the Guayaba Engine visual configuration\n";
    std::cout << "  lazycat help                  Show this help message\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printHelp();
        return 0;
    }

    std::string command = argv[1];

    if (command == "list") {
        ModelManager::listModels();
    } else if (command == "download") {
        if (argc < 3) {
            std::cerr << "Usage: lazycat download <repo_id>\n";
            return 1;
        }
        ModelManager::downloadModel(argv[2]);
    } else if (command == "delete") {
        if (argc < 3) {
            std::cerr << "Usage: lazycat delete <filename>\n";
            return 1;
        }
        ModelManager::deleteModel(argv[2]);
    } else if (command == "chat") {
        if (argc < 3) {
            std::cerr << "Usage: lazycat chat <filename>\n";
            return 1;
        }
        ExecutionEngine::chat(argv[2]);
    } else if (command == "serve") {
        if (argc < 3) {
            std::cerr << "Usage: lazycat serve <filename>\n";
            return 1;
        }
        ExecutionEngine::serve(argv[2]);
    } else if (command == "config" || command == "ui") {
        LazyTUI::run();
    } else if (command == "help" || command == "--help" || command == "-h") {
        printHelp();
    } else {
        std::cerr << "Unknown command: " << command << "\n";
        printHelp();
        return 1;
    }

    return 0;
}
