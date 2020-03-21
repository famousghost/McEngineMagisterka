#include "FileManager.h"

namespace McEngine
{
namespace Files
{
void FileManager::start()
{
}

void FileManager::shutdown()
{
}

void FileManager::loadFile(std::string p_fileName)
{
    m_filePaths.push_back(p_fileName);
}

FileManager & FileManager::getInstance()
{
    static FileManager fileManager;
    return fileManager;
}

}//Files
}//McEngine
