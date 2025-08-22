# ImGui Android Soft Keyboard Integration

This project provides a complete soft keyboard implementation for ImGui on Android, with both Java and C++ support.

## Features

- **Custom Soft Keyboard**: Built-in virtual keyboard with QWERTY layout
- **Text Input Handling**: Seamless text input integration with ImGui
- **Keyboard State Management**: Automatic detection of keyboard show/hide
- **Cross-Platform**: Works with both Java Android and C++ ImGui
- **Touch Support**: Full touch input support for the virtual keyboard

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
1. **SoftKeyboardManager**: Manages the Android soft keyboard state
2. **Layout Detection**: Automatically detects when the system keyboard appears/disappears
3. **JNI Bridge**: Communicates keyboard state to C++ code

### C++ Side (ImGui)
1. **SoftKeyboard Class**: Implements the virtual keyboard UI
2. **ImGui Integration**: Renders keyboard using ImGui widgets
3. **Text Input**: Handles text input and forwards to ImGui input fields

## Usage

### 1. Show/Hide Keyboard

```java
// Java side
SoftKeyboardManager keyboardManager = SoftKeyboardManager.getInstance(this);
keyboardManager.showKeyboard();  // Show system keyboard
keyboardManager.hideKeyboard();  // Hide system keyboard
```

```cpp
// C++ side
SoftKeyboard& keyboard = SoftKeyboard::getInstance();
keyboard.setVisible(true);   // Show virtual keyboard
keyboard.setVisible(false);  // Hide virtual keyboard
```

### 2. Handle Text Input

```cpp
// Set callback for text input
keyboard.setTextInputCallback([](const std::string& text) {
    // Handle incoming text here
    printf("Received text: %s\n", text.c_str());
});
```

### 3. Customize Keyboard Layout

Edit `SoftKeyboard.cpp` to modify the keyboard layout:

```cpp
// Modify key rows in renderKeyboard() method
const char* keys[] = {
    "1234567890",
    "qwertyuiop",
    "asdfghjkl",
    "zxcvbnm"
};
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
- Text input field
- Keyboard visibility controls
- Real-time keyboard state display
- Test input functionality

## Troubleshooting

### Common Issues

1. **Keyboard not showing**
   - Check if `SoftKeyboardManager` is properly initialized
   - Verify JNI methods are correctly linked

2. **Text input not working**
   - Ensure `handleTextInput` JNI method is called
   - Check if text input callback is set

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