#include "menu.h"
#include <string>

static char inputBuffer[256] = "";
static bool showKeyboard = false;
static std::string currentText = "";

void BeginDraw() {
    ShowDemoWindow();
}

void ShowDemoWindow() {
    ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("Soft Keyboard Demo", nullptr);
    
    ImGui::Text("Soft Keyboard Demo for ImGui Android");
    ImGui::Separator();
    
    // Text input field
    ImGui::Text("Text Input:");
    ImGui::PushItemWidth(-1);
    if (ImGui::InputText("##TextInput", inputBuffer, sizeof(inputBuffer))) {
        currentText = inputBuffer;
    }
    ImGui::PopItemWidth();
    
    ImGui::Spacing();
    
    // Current text display
    ImGui::Text("Current Text: %s", currentText.c_str());
    ImGui::Text("Keyboard Height: %d", SoftKeyboard::getInstance().getHeight());
    ImGui::Text("Keyboard Visible: %s", SoftKeyboard::getInstance().isVisible() ? "Yes" : "No");
    
    ImGui::Spacing();
    
    // Control buttons
    if (ImGui::Button("Show Keyboard", ImVec2(120, 30))) {
        showKeyboard = true;
        SoftKeyboard::getInstance().setVisible(true);
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Hide Keyboard", ImVec2(120, 30))) {
        showKeyboard = false;
        SoftKeyboard::getInstance().setVisible(false);
    }
    
    ImGui::Spacing();
    
    if (ImGui::Button("Clear Text", ImVec2(120, 30))) {
        memset(inputBuffer, 0, sizeof(inputBuffer));
        currentText.clear();
        SoftKeyboard::getInstance().clearTextInput();
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Test Input", ImVec2(120, 30))) {
        SoftKeyboard::getInstance().addTextInput("Test Input!");
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    
    // Instructions
    ImGui::TextWrapped("Instructions:");
    ImGui::BulletText("Click 'Show Keyboard' to display the soft keyboard");
    ImGui::BulletText("Use the keyboard to input text");
    ImGui::BulletText("Text will appear in the input field above");
    ImGui::BulletText("Click 'Hide Keyboard' to hide the keyboard");
    
    ImGui::End();
}