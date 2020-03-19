#pragma once
namespace McEngine
{
namespace Editor
{
class EditorManager
{
public:
    void start();

    void shutdown();

    static EditorManager& getInstance();

    void editorLoop();
private:
    EditorManager() = default;
    ~EditorManager() = default;

    void initGlfw();
};

}//Editor
}//McEngine

