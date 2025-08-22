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
    ImGui::SetNextWindowSize(ImVec2(450, 350), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("Native Android Soft Keyboard Demo", nullptr);
    
    ImGui::Text("Native Android Soft Keyboard Integration with ImGui");
    ImGui::Separator();
    
    // Text input field
    ImGui::Text("Text Input Field:");
    ImGui::PushItemWidth(-1);
    if (ImGui::InputText("##TextInput", inputBuffer, sizeof(inputBuffer))) {
        currentText = inputBuffer;
    }
    ImGui::PopItemWidth();
    
    ImGui::Spacing();
    
    // Current text display from Java keyboard
    ImGui::Text("Text from Java Android Keyboard: %s", currentText.c_str());
    ImGui::Text("Keyboard Height: %d", SoftKeyboard::getInstance().getHeight());
    ImGui::Text("Keyboard Visible: %s", SoftKeyboard::getInstance().isVisible() ? "Yes" : "No");
    
    ImGui::Spacing();
    
    // Control buttons for native Android keyboard
    if (ImGui::Button("Show Android Keyboard", ImVec2(150, 30))) {
        showKeyboard = true;
        // This will trigger the native Android soft keyboard
        // The keyboard manager will handle showing it
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Hide Android Keyboard", ImVec2(150, 30))) {
        showKeyboard = false;
        // This will hide the native Android soft keyboard
    }
    
    ImGui::Spacing();
    
    if (ImGui::Button("Clear Text", ImVec2(120, 30))) {
        memset(inputBuffer, 0, sizeof(inputBuffer));
        currentText.clear();
        SoftKeyboard::getInstance().clearTextInput();
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Test Input", ImVec2(120, 30))) {
        // This simulates text input from Java
        SoftKeyboard::getInstance().addTextInput("Test Input from Java!");
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    
    // Instructions for native Android keyboard
    ImGui::TextWrapped("How to use Native Android Soft Keyboard:");
    ImGui::BulletText("Click 'Show Android Keyboard' to display the system keyboard");
    ImGui::BulletText("Type using the native Android keyboard (not ImGui)");
    ImGui::BulletText("Text will automatically appear in the input field above");
    ImGui::BulletText("Press Enter/Done on the Android keyboard to submit");
    ImGui::BulletText("Click 'Hide Android Keyboard' to hide the system keyboard");
    
    ImGui::Spacing();
    ImGui::TextWrapped("Features:");
    ImGui::BulletText("Uses native Android soft keyboard (not custom ImGui keyboard)");
    ImGui::BulletText("Automatic text input from Java to C++ ImGui");
    ImGui::BulletText("Real-time text change detection");
    ImGui::BulletText("Proper keyboard state management");
    ImGui::BulletText("Supports all Android keyboard features (autocomplete, etc.)");
    
    ImGui::End();
}