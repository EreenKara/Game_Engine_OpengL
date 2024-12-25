#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "glm/glm.hpp"
namespace graf{
    void printMatrix(const glm::mat4& mtx)
    {
        if (ImGui::BeginTable("Matrix", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
        {  
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("%-.3f",mtx[0][0]);
            ImGui::TableNextColumn(); ImGui::Text("%-.3f",mtx[1][0]);
            ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[2][0]);
            ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[3][0]);
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[0][1]);
            ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[1][1]);
            ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[2][1]);
            ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[3][1]);
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[0][2]);
            ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[1][2]);
            ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[2][2]);
            ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[3][2]);
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[0][3]);
            ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[1][3]);
            ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[2][3]);
            ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[3][3]);
            ImGui::EndTable();
        }
    }
}
