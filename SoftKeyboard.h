#ifndef SOFT_KEYBOARD_H
#define SOFT_KEYBOARD_H

#include <string>
#include <vector>
#include <functional>
#include "ImGui/imgui.h"

class SoftKeyboard {
public:
    static SoftKeyboard& getInstance();
    
    // Keyboard state
    bool isVisible() const;
    int getHeight() const;
    void setVisible(bool visible);
    void setHeight(int height);
    
    // Text input handling
    void addTextInput(const std::string& text);
    void clearTextInput();
    std::string getCurrentText() const;
    
    // ImGui integration
    void renderKeyboard();
    void handleInput();
    
    // Callbacks
    void setTextInputCallback(std::function<void(const std::string&)> callback);
    void setKeyboardStateCallback(std::function<void(bool, int)> callback);
    
    // Native methods called from Java
    static void handleTextInputFromJava(const std::string& text);
    static void setKeyboardVisibleFromJava(bool visible);
    static bool isKeyboardVisibleFromJava();
    static int getKeyboardHeightFromJava();
    
private:
    SoftKeyboard() = default;
    ~SoftKeyboard() = default;
    SoftKeyboard(const SoftKeyboard&) = delete;
    SoftKeyboard& operator=(const SoftKeyboard&) = delete;
    
    bool m_isVisible = false;
    int m_height = 0;
    std::string m_currentText;
    std::vector<std::string> m_textBuffer;
    
    std::function<void(const std::string&)> m_textInputCallback;
    std::function<void(bool, int)> m_keyboardStateCallback;
    
    // ImGui state
    char m_inputBuffer[1024];
    bool m_inputActive = false;
    ImGuiID m_inputFieldID;
};

#endif // SOFT_KEYBOARD_H