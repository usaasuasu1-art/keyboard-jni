package com.forseken.froz;

import android.app.Activity;
import android.content.Context;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.graphics.Rect;
import android.util.Log;

public class SoftKeyboardManager {
    private static final String TAG = "SoftKeyboardManager";
    private static SoftKeyboardManager instance;
    private Context context;
    private InputMethodManager imm;
    private EditText hiddenEditText;
    private boolean isKeyboardVisible = false;
    private int keyboardHeight = 0;
    private OnKeyboardStateChangeListener keyboardListener;
    
    public interface OnKeyboardStateChangeListener {
        void onKeyboardShown(int height);
        void onKeyboardHidden();
    }
    
    private SoftKeyboardManager(Context context) {
        this.context = context;
        this.imm = (InputMethodManager) context.getSystemService(Context.INPUT_METHOD_SERVICE);
        setupHiddenEditText();
    }
    
    public static synchronized SoftKeyboardManager getInstance(Context context) {
        if (instance == null) {
            instance = new SoftKeyboardManager(context);
        }
        return instance;
    }
    
    private void setupHiddenEditText() {
        hiddenEditText = new EditText(context);
        hiddenEditText.setVisibility(View.GONE);
        hiddenEditText.setFocusable(true);
        hiddenEditText.setFocusableInTouchMode(true);
        
        // Add to the main activity's root view
        if (context instanceof Activity) {
            Activity activity = (Activity) context;
            View rootView = activity.getWindow().getDecorView().findViewById(android.R.id.content);
            if (rootView instanceof ViewGroup) {
                ((ViewGroup) rootView).addView(hiddenEditText);
            }
        }
    }
    
    public void setOnKeyboardStateChangeListener(OnKeyboardStateChangeListener listener) {
        this.keyboardListener = listener;
    }
    
    public void showKeyboard() {
        if (hiddenEditText != null) {
            hiddenEditText.requestFocus();
            imm.showSoftInput(hiddenEditText, InputMethodManager.SHOW_IMPLICIT);
            isKeyboardVisible = true;
            Log.d(TAG, "Soft keyboard shown");
        }
    }
    
    public void hideKeyboard() {
        if (hiddenEditText != null) {
            imm.hideSoftInputFromWindow(hiddenEditText.getWindowToken(), 0);
            hiddenEditText.clearFocus();
            isKeyboardVisible = false;
            Log.d(TAG, "Soft keyboard hidden");
        }
    }
    
    public boolean isKeyboardVisible() {
        return isKeyboardVisible;
    }
    
    public int getKeyboardHeight() {
        return keyboardHeight;
    }
    
    public void updateKeyboardHeight(int height) {
        this.keyboardHeight = height;
        if (keyboardListener != null) {
            if (height > 0) {
                keyboardListener.onKeyboardShown(height);
            } else {
                keyboardListener.onKeyboardHidden();
            }
        }
    }
    
    public void handleTextInput(String text) {
        // Send text input to native C++ code
        if (GLES3JNIView.class != null) {
            try {
                // Call native method to handle text input
                GLES3JNIView.handleTextInput(text);
            } catch (Exception e) {
                Log.e(TAG, "Error sending text to native code: " + e.getMessage());
            }
        }
    }
    
    public void cleanup() {
        if (hiddenEditText != null && context instanceof Activity) {
            Activity activity = (Activity) context;
            View rootView = activity.getWindow().getDecorView().findViewById(android.R.id.content);
            if (rootView instanceof ViewGroup) {
                ((ViewGroup) rootView).removeView(hiddenEditText);
            }
        }
        instance = null;
    }
}