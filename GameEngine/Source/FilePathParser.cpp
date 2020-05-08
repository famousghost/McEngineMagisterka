#include "FilePathParser.h"
namespace McEngine
{
namespace Utility
{

std::string FilePathParser::fetchObjectName(const std::string & p_filePath)
{
    auto l_fromLastSlash = p_filePath.find_last_of("/");
    std::string l_objectName = p_filePath.substr(l_fromLastSlash + 1);
    auto l_findElem = l_objectName.find_first_of(".");
    return l_objectName.substr(0, l_findElem);
}

}//Utility
}//McEngine
