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

void SoftKeyboard::addTextChange(const std::string& text) {
    m_currentText = text;
    
    if (m_textChangeCallback) {
        m_textChangeCallback(text);
    }
}

void SoftKeyboard::addTextSubmit(const std::string& text) {
    if (m_textSubmitCallback) {
        m_textSubmitCallback(text);
    }
    
    // Clear text after submission
    m_currentText.clear();
    m_textBuffer.clear();
    memset(m_inputBuffer, 0, sizeof(m_inputBuffer));
}

void SoftKeyboard::clearTextInput() {
    m_currentText.clear();
    m_textBuffer.clear();
    memset(m_inputBuffer, 0, sizeof(m_inputBuffer));
}

std::string SoftKeyboard::getCurrentText() const {
    return m_currentText;
}

void SoftKeyboard::renderTextInput() {
    // This method now just renders a text input field that shows the current text
    // The actual input comes from Java via the native Android keyboard
    
    ImGui::Begin("Text Input Status", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    
    ImGui::Text("Current Text: %s", m_currentText.c_str());
    ImGui::Text("Keyboard Height: %d", m_height);
    ImGui::Text("Keyboard Visible: %s", m_isVisible ? "Yes" : "No");
    
    if (ImGui::Button("Clear Text")) {
        clearTextInput();
    }
    
    ImGui::End();
}

void SoftKeyboard::handleInput() {
    // Handle any additional input processing here
}

void SoftKeyboard::setTextInputCallback(std::function<void(const std::string&)> callback) {
    m_textInputCallback = callback;
}

void SoftKeyboard::setTextChangeCallback(std::function<void(const std::string&)> callback) {
    m_textChangeCallback = callback;
}

void SoftKeyboard::setTextSubmitCallback(std::function<void(const std::string&)> callback) {
    m_textSubmitCallback = callback;
}

void SoftKeyboard::setKeyboardStateCallback(std::function<void(bool, int)> callback) {
    m_keyboardStateCallback = callback;
}

// Static methods called from Java
void SoftKeyboard::handleTextInputFromJava(const std::string& text) {
    getInstance().addTextInput(text);
}

void SoftKeyboard::handleTextChangeFromJava(const std::string& text) {
    getInstance().addTextChange(text);
}

void SoftKeyboard::handleTextSubmitFromJava(const std::string& text) {
    getInstance().addTextSubmit(text);
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