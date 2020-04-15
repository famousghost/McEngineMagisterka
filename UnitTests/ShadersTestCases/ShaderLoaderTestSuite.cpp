#pragma once
#include "ShaderLoader.h"
#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include <iostream>

using testing::_;
using testing::Return;
using testing::AtLeast;
using testing::NiceMock;
using testing::StrictMock;

namespace McEngine
{
namespace Shaders
{

struct ShaderLoaderTestSuite : public testing::Test
{
    ShaderLoaderTestSuite(){}
    ShaderLoader m_shaderLoader;
};

TEST_F(ShaderLoaderTestSuite, verifyIfShaderLoaderReturnEmptyStringWhenFileIsNotExist)
{
    std::string l_shaderSource = "TestFiles/NotExsistingFilePath.txt";
    std::string l_result = m_shaderLoader.loadShaderFromFile(l_shaderSource);
    ASSERT_EQ(l_result, "");
}

TEST_F(ShaderLoaderTestSuite, verifyIfShaderLoaderReturnFileContentWhenFileExists)
{
    std::string l_shaderSource = "../../MasterEngine/Shaders/TestFile.vlsl";
    std::string l_result = m_shaderLoader.loadShaderFromFile(l_shaderSource);
    std::cout << l_result<<std::endl;
    ASSERT_TRUE(l_result.length() > 0);
    ASSERT_EQ(l_result, "SimpleShader");
}

}//Shaders
}//GameEngine