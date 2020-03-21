#pragma once
#include <string>
#include <vector>

namespace McEngine
{
namespace Files
{
class FileManager
{
public:
    void start();

    void shutdown();

    void loadFile(std::string p_fileName);

    static FileManager& getInstance();

private:
    FileManager() = default;
    ~FileManager() = default;
    std::vector<std::string> m_filePaths;
    
};
}//Files
}//McEngine

