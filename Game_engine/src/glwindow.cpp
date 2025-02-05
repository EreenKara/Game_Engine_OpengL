#include "glwindow.hpp"
#include <iostream>
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui.h"
#include "save.hpp"

namespace graf
{
    
    void GLWindow::staticMouseFunction(GLFWwindow* window, double xpos, double ypos)
    {
        GLWindow* pWindow = (GLWindow*)glfwGetWindowUserPointer(window);
        pWindow->m_mouseFunction(xpos,ypos);
    }
    void GLWindow::staticMouseFcousFunction(GLFWwindow* window, int focused)
    {
        // GLWindow* pWindow = (GLWindow*)glfwGetWindowUserPointer(window);
        // if (focused) {
        //     // Pencere odaklandığında imleci devre dışı bırak
        //     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        //     std::cout << "Pencere odaklandı, fare imleci devre dışı bırakıldı." << std::endl;
        // } else {
        //     // Pencere odak kaybettiğinde imleci normal hale getir
        //     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        //     std::cout << "Pencere odak kaybetti, fare imleci geri yüklendi." << std::endl;
        // }
    }

    void GLWindow::statickeyboardFunction(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        GLWindow* pWindow = (GLWindow*)glfwGetWindowUserPointer(window);
         pWindow->m_keyboardFunction(key,scancode,action);
        if(key==GLFW_KEY_C && action==GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        if(key==GLFW_KEY_V && action==GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    void GLWindow::setKeyboardFunction(KeyboardFunction keyboardFunction)
    {
        m_keyboardFunction = keyboardFunction;
    }
    int GLWindow::create(unsigned int width,unsigned int height)
    {
        if(!glfwInit())
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
       
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        

        m_window = glfwCreateWindow(width, height, "My Title", NULL, NULL);

        if (!m_window )
        {
            // Window or OpenGL context creation failed
        }
        glfwMakeContextCurrent(m_window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
        /*
        ImGui ayarlamaları. create context ve init opengl initglfw normalde yeterli.
        */
       ImGui::CreateContext(); 

        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
        
         glEnable(GL_DEPTH_TEST); 
        glfwSetWindowUserPointer(m_window,this);
        glfwSetKeyCallback(m_window,statickeyboardFunction);
        // glfwSetCursorPosCallback(m_window, staticMouseFunction);

        return 1;
    }
    void GLWindow::setRenderFunction(RenderFunction render)
    {
        std::cout<<"Render func"<<std::endl;
        m_renderFunction = render;
    }
    void GLWindow::setImGuiRenderFunction(ImGuiRenderFunction imguiRender)
    {
        std::cout<<"imgui Render func"<<std::endl;
        m_imguiRenderFunction = imguiRender;
    }
    void GLWindow::setMouseFunction(MouseFunction mouseFunction){
        m_mouseFunction = mouseFunction;
    }


    void GLWindow::render()
    {
        while (!glfwWindowShouldClose(m_window))
        {
            // ImGui başlangıcı
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Kullanıcı tanımlı ImGui arayüzü
            m_imguiRenderFunction();

            ImGui::Render();

            // Kamera ve sahne kontrolleri sadece ImGui girdi istemediğinde yapılır
            ImGuiIO& io = ImGui::GetIO();
            if (!io.WantCaptureMouse) {
                // Eğer ImGui aktif değilse, kullanıcının kontrol fonksiyonları çalışır
                m_mouseFunction(io.MousePos.x, io.MousePos.y);
            }

            // Kullanıcı tanımlı render fonksiyonu
            m_renderFunction();

            // ImGui sonlandırma
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Buffer takası ve olaylar
            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }
    }
    void GLWindow::deleteWindow(){
        glfwDestroyWindow(m_window);
    }
    GLWindow::~GLWindow(){
        deleteWindow();
    }   

}