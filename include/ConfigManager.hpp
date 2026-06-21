#ifndef CONFIG_MANAGER_HPP
#define CONFIG_MANAGER_HPP

#include <string>

class ConfigManager {
public:
    static ConfigManager& getInstance();

    // Configuration properties
    std::string getModelDirectory() const;
    void setModelDirectory(const std::string& dir);

    int getContextLimit() const;
    void setContextLimit(int limit);

    int getNumThreads() const;
    void setNumThreads(int threads);

    bool getNoWebui() const;
    void setNoWebui(bool no_webui);

    // Save and load
    void loadConfig();
    void saveConfig() const;

private:
    ConfigManager();
    ~ConfigManager() = default;

    // Delete copy/move constructors and assignment operators
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    ConfigManager(ConfigManager&&) = delete;
    ConfigManager& operator=(ConfigManager&&) = delete;

    std::string model_directory;
    int context_limit;
    int num_threads;
    bool no_webui;
    
    const std::string config_file = "lazycat.conf";
};

#endif // CONFIG_MANAGER_HPP
