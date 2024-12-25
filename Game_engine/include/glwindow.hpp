#ifndef GLWindow_hpp
#define GLWindow_hpp

#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include "functiontypes.hpp"

namespace graf
{
    class GLWindow
    {
    public:
        int create(unsigned int width,unsigned int height);
        void render();
        void setImGuiRenderFunction (ImGuiRenderFunction function);
        void setRenderFunction (RenderFunction function);
        void setKeyboardFunction(KeyboardFunction keyboardFunction);
        void setMouseFunction(MouseFunction mouseFunction);
        
        ~GLWindow();
    private:
        void deleteWindow();
        static void staticMouseFunction(GLFWwindow* window, double xpos, double ypos);
        static void staticMouseFcousFunction(GLFWwindow* window, int focused);
        static void statickeyboardFunction(GLFWwindow* window, int key, int scancode, int action, int mods);
        GLFWwindow*         m_window;
        ImGuiRenderFunction      m_imguiRenderFunction;
        RenderFunction      m_renderFunction;
        KeyboardFunction    m_keyboardFunction;
        MouseFunction    m_mouseFunction;
    };
}


#endif