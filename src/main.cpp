#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <cmath>
#include <vector>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

int main() {
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui + ImPlot Demo", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
    // Generate sample data for the plot
    static std::vector<float> x_data, y_data;
    if (x_data.empty()) {
        for (int i = 0; i < 100; ++i) {
            float x = i * 0.1f;
            x_data.push_back(x);
            y_data.push_back(sin(x) * cos(x * 0.5f));
        }
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Simple Tooltip Example");
        
        ImGui::Text("Hover over the button below to see a tooltip:");
        
        if (ImGui::Button("Button with Tooltip")) {
            // Button was clicked
        }
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("This is a simple tooltip!\nIt appears when you hover over the button.");
        }
        
        ImGui::Separator();
        
        ImGui::Text("Hover over this text for an advanced tooltip:");
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("Advanced Tooltip");
            ImGui::Separator();
            ImGui::Text("Mouse Position: (%.1f, %.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
            ImGui::Text("Frame Rate: %.1f FPS", ImGui::GetIO().Framerate);
            ImGui::EndTooltip();
        }
        
        ImGui::Separator();
        
        ImGui::Text("Button with automatic item tooltip:");
        if (ImGui::Button("Auto Tooltip Button")) {
            // Button was clicked
        }
        ImGui::SetItemTooltip("This tooltip is set automatically\nfor the previous item using SetItemTooltip()");
        
        ImGui::End();

        // ImPlot window with tooltip
        ImGui::Begin("Plot with Tooltip");
        
        if (ImPlot::BeginPlot("Sin Wave with Tooltip")) {
            ImPlot::PlotLine("sin(x) * cos(0.5x)", x_data.data(), y_data.data(), x_data.size());
            
            // Check if mouse is hovering over the axes only
            bool x_axis_hovered = ImPlot::IsAxisHovered(ImAxis_X1);
            bool y_axis_hovered = ImPlot::IsAxisHovered(ImAxis_Y1);
            
            if (x_axis_hovered || y_axis_hovered) {
                ImPlotPoint mouse_pos = ImPlot::GetPlotMousePos();
                
                ImGui::BeginTooltip();
                if (x_axis_hovered && y_axis_hovered) {
                    ImGui::Text("Hovering over both axes");
                } else if (x_axis_hovered) {
                    ImGui::Text("X-Axis Hovered");
                    ImGui::Separator();
                    ImGui::Text("X Value: %.2f", mouse_pos.x);
                } else if (y_axis_hovered) {
                    ImGui::Text("Y-Axis Hovered");
                    ImGui::Separator();
                    ImGui::Text("Y Value: %.2f", mouse_pos.y);
                }
                ImGui::EndTooltip();
            }
            
            ImPlot::EndPlot();
        }
        
        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}