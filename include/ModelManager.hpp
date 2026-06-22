#ifndef MODEL_MANAGER_HPP
#define MODEL_MANAGER_HPP

#include <string>
#include <vector>

class ModelManager {
public:
    static void listModels();
    static void downloadModel(const std::string& repo_id, const std::string& quant = ""); 
    static void deleteModel(const std::string& filename);
};

#endif // MODEL_MANAGER_HPP
