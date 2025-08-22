#include "SoftKeyboard.h"
#include <algorithm>
#include <cstring>

SoftKeyboard& SoftKeyboard::getInstance() {
    static SoftKeyboard instance;
    return instance;
}

bool SoftKeyboard::isVisible() const {
    return m_isVisible;
}

int SoftKeyboard::getHeight() const {
    return m_height;
}

void SoftKeyboard::setVisible(bool visible) {
    m_isVisible = visible;
    if (m_keyboardStateCallback) {
        m_keyboardStateCallback(visible, m_height);
    }
}

void SoftKeyboard::setHeight(int height) {
    m_height = height;
}

void SoftKeyboard::addTextInput(const std::string& text) {
    m_textBuffer.push_back(text);
    m_currentText += text;
    
    if (m_textInputCallback) {
        m_textInputCallback(text);
    }
}

void SoftKeyboard::clearTextInput() {
    m_currentText.clear();
    m_textBuffer.clear();
    memset(m_inputBuffer, 0, sizeof(m_inputBuffer));
}

std::string SoftKeyboard::getCurrentText() const {
    return m_currentText;
}

void SoftKeyboard::renderKeyboard() {
    if (!m_isVisible) return;
    
    ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetIO().DisplaySize.y - m_height), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, m_height), ImGuiCond_Always);
    
    ImGui::Begin("Soft Keyboard", nullptr, 
                 ImGuiWindowFlags_NoTitleBar | 
                 ImGuiWindowFlags_NoResize | 
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoScrollbar);
    
    // Text input field
    ImGui::PushItemWidth(ImGui::GetWindowWidth() - 20);
    if (ImGui::InputText("##Input", m_inputBuffer, sizeof(m_inputBuffer), 
                         ImGuiInputTextFlags_EnterReturnsTrue)) {
        if (strlen(m_inputBuffer) > 0) {
            addTextInput(m_inputBuffer);
            memset(m_inputBuffer, 0, sizeof(m_inputBuffer));
        }
    }
    ImGui::PopItemWidth();
    
    // Keyboard buttons
    const char* keys[] = {
        "1234567890",
        "qwertyuiop",
        "asdfghjkl",
        "zxcvbnm"
    };
    
    float buttonSize = (ImGui::GetWindowWidth() - 40) / 10.0f;
    float spacing = 5.0f;
    
    for (int row = 0; row < 4; ++row) {
        ImGui::BeginGroup();
        for (int col = 0; col < strlen(keys[row]); ++col) {
            if (col > 0) ImGui::SameLine(spacing);
            
            char key[2] = {keys[row][col], '\0'};
            if (ImGui::Button(key, ImVec2(buttonSize, buttonSize))) {
                addTextInput(key);
            }
        }
        ImGui::EndGroup();
        
        if (row < 3) ImGui::Spacing();
    }
    
    // Special keys
    ImGui::Spacing();
    ImGui::BeginGroup();
    
    if (ImGui::Button("Space", ImVec2(buttonSize * 3, buttonSize))) {
        addTextInput(" ");
    }
    ImGui::SameLine();
    if (ImGui::Button("Back", ImVec2(buttonSize * 2, buttonSize))) {
        if (!m_currentText.empty()) {
            m_currentText.pop_back();
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear", ImVec2(buttonSize * 2, buttonSize))) {
        clearTextInput();
    }
    ImGui::SameLine();
    if (ImGui::Button("Hide", ImVec2(buttonSize * 2, buttonSize))) {
        setVisible(false);
    }
    
    ImGui::EndGroup();
    
    ImGui::End();
}

void SoftKeyboard::handleInput() {
    // Handle any additional input processing here
}

void SoftKeyboard::setTextInputCallback(std::function<void(const std::string&)> callback) {
    m_textInputCallback = callback;
}

void SoftKeyboard::setKeyboardStateCallback(std::function<void(bool, int)> callback) {
    m_keyboardStateCallback = callback;
}

// Static methods called from Java
void SoftKeyboard::handleTextInputFromJava(const std::string& text) {
    getInstance().addTextInput(text);
}

void SoftKeyboard::setKeyboardVisibleFromJava(bool visible) {
    getInstance().setVisible(visible);
}

bool SoftKeyboard::isKeyboardVisibleFromJava() {
    return getInstance().isVisible();
}

int SoftKeyboard::getKeyboardHeightFromJava() {
    return getInstance().getHeight();
}