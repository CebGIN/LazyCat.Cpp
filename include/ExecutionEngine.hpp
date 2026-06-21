#ifndef EXECUTION_ENGINE_HPP
#define EXECUTION_ENGINE_HPP

#include <string>

class ExecutionEngine {
public:
    static void chat(const std::string& filename);
    static void serve(const std::string& filename);
};

#endif // EXECUTION_ENGINE_HPP
