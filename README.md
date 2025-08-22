# ImGui Android Native Soft Keyboard Integration

This project provides native Android soft keyboard integration for ImGui on Android, with both Java and C++ support.

## Features

- **Native Android Soft Keyboard**: Uses the system's built-in soft keyboard
- **Text Input Handling**: Seamless text input from Java to C++ ImGui
- **Keyboard State Management**: Automatic detection of keyboard show/hide
- **Cross-Platform**: Works with both Java Android and C++ ImGui
- **Real-time Text Updates**: Live text input as you type on Android keyboard

## Project Structure

```
├── MainActivity.java          # Main Android activity with keyboard integration
├── GLES3JNIView.java         # OpenGL view with JNI methods
├── SoftKeyboardManager.java  # Java-side keyboard manager
├── main.cpp                  # Main C++ file with ImGui integration
├── SoftKeyboard.h            # C++ header for soft keyboard
├── SoftKeyboard.cpp          # C++ implementation of soft keyboard
├── menu.h                    # Demo menu header
├── menu.cpp                  # Demo menu implementation
├── CMakeLists.txt            # Build configuration
└── README.md                 # This file
```

## How It Works

### Java Side (Android)
1. **SoftKeyboardManager**: Manages the native Android soft keyboard state
2. **Layout Detection**: Automatically detects when the system keyboard appears/disappears
3. **Text Input Handling**: Captures text input from the native Android keyboard
4. **JNI Bridge**: Communicates text input and keyboard state to C++ code

### C++ Side (ImGui)
1. **SoftKeyboard Class**: Handles text input from Java and manages keyboard state
2. **ImGui Integration**: Receives and displays text from the native Android keyboard
3. **Text Input Callbacks**: Processes text input, changes, and submissions from Java

## Usage

### 1. Show/Hide Native Android Keyboard

```java
// Java side
SoftKeyboardManager keyboardManager = SoftKeyboardManager.getInstance(this);
keyboardManager.showKeyboard();  // Show native Android keyboard
keyboardManager.hideKeyboard();  // Hide native Android keyboard
```

```cpp
// C++ side
SoftKeyboard& keyboard = SoftKeyboard::getInstance();
keyboard.setVisible(true);   // Show keyboard (managed by Java)
keyboard.setVisible(false);  // Hide keyboard (managed by Java)
```

### 2. Handle Text Input

```cpp
// Set callback for text input
keyboard.setTextInputCallback([](const std::string& text) {
    // Handle incoming text here
    printf("Received text: %s\n", text.c_str());
});
```

### 3. Handle Different Text Input Types

```java
// Java side - Set different input types
keyboardManager.setInputType(EditorInfo.TYPE_CLASS_TEXT | EditorInfo.TYPE_TEXT_VARIATION_PASSWORD);
keyboardManager.setHint("Enter password");
```

```cpp
// C++ side - Handle different text events
keyboard.setTextInputCallback([](const std::string& text) {
    // Handle each character input
});
keyboard.setTextChangeCallback([](const std::string& text) {
    // Handle text changes
});
keyboard.setTextSubmitCallback([](const std::string& text) {
    // Handle text submission (Enter key)
});
```

## Building

### Prerequisites
- Android NDK
- CMake 3.10.2+
- ImGui library files

### Build Steps

1. **Set up Android NDK environment**
```bash
export ANDROID_NDK=/path/to/your/android-ndk
export PATH=$PATH:$ANDROID_NDK
```

2. **Create build directory**
```bash
mkdir build && cd build
```

3. **Configure with CMake**
```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
         -DANDROID_ABI=arm64-v8a \
         -DANDROID_PLATFORM=android-21
```

4. **Build the project**
```bash
make -j$(nproc)
```

The library will be generated as `libs/arm64-v8a/libBossUnknownX.so`

## Integration with Existing Projects

### 1. Add to AndroidManifest.xml
```xml
<uses-permission android:name="android.permission.SYSTEM_ALERT_WINDOW" />
<uses-permission android:name="android.permission.INTERNET" />
```

### 2. Initialize in MainActivity
```java
@Override
protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    
    // Initialize soft keyboard manager
    SoftKeyboardManager keyboardManager = SoftKeyboardManager.getInstance(this);
    setupKeyboardDetection();
    
    // ... rest of your initialization
}
```

### 3. Add to C++ Code
```cpp
#include "SoftKeyboard.h"

// In your init function
SoftKeyboard& keyboard = SoftKeyboard::getInstance();
keyboard.setTextInputCallback([](const std::string& text) {
    // Handle text input
});

// In your render loop
keyboard.renderKeyboard();
```

## Demo

The included demo menu shows:
- Text input field that receives text from native Android keyboard
- Keyboard visibility controls for system keyboard
- Real-time text input display from Java to C++
- Keyboard state monitoring
- Test input functionality

## Troubleshooting

### Common Issues

1. **Native Android keyboard not showing**
   - Check if `SoftKeyboardManager` is properly initialized
   - Verify JNI methods are correctly linked
   - Ensure EditText has proper focus

2. **Text input not reaching C++ ImGui**
   - Ensure `handleTextInput`, `handleTextChange`, `handleTextSubmit` JNI methods are called
   - Check if text input callbacks are set in C++
   - Verify EditText text change listeners are working

3. **Build errors**
   - Verify ImGui include paths are correct
   - Check Android NDK version compatibility

### Debug Tips

- Enable logging in `SoftKeyboardManager.java`
- Use `adb logcat` to monitor Android logs
- Check JNI method signatures match between Java and C++

## License

This project is provided as-is for educational and development purposes.

## Contributing

Feel free to submit issues and enhancement requests!