package com.forseken.froz;

import android.app.Activity;
import android.content.Context;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.util.Log;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.inputmethod.EditorInfo;
import android.widget.TextView;

public class SoftKeyboardManager {
    private static final String TAG = "SoftKeyboardManager";
    private static SoftKeyboardManager instance;
    private Context context;
    private InputMethodManager imm;
    private EditText hiddenEditText;
    private boolean isKeyboardVisible = false;
    private int keyboardHeight = 0;
    private OnKeyboardStateChangeListener keyboardListener;
    private OnTextInputListener textInputListener;
    
    public interface OnKeyboardStateChangeListener {
        void onKeyboardShown(int height);
        void onKeyboardHidden();
    }
    
    public interface OnTextInputListener {
        void onTextInput(String text);
        void onTextChanged(String text);
        void onTextSubmitted(String text);
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
        hiddenEditText.setInputType(EditorInfo.TYPE_CLASS_TEXT | EditorInfo.TYPE_TEXT_VARIATION_NORMAL);
        
        // Add text change listener
        hiddenEditText.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {}
            
            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                if (textInputListener != null) {
                    textInputListener.onTextChanged(s.toString());
                }
            }
            
            @Override
            public void afterTextChanged(Editable s) {
                if (textInputListener != null) {
                    textInputListener.onTextInput(s.toString());
                }
            }
        });
        
        // Add editor action listener for Enter key
        hiddenEditText.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView v, int actionId, android.view.KeyEvent event) {
                if (actionId == EditorInfo.IME_ACTION_DONE || actionId == EditorInfo.IME_ACTION_GO) {
                    if (textInputListener != null) {
                        textInputListener.onTextSubmitted(v.getText().toString());
                    }
                    // Clear text after submission
                    v.setText("");
                    return true;
                }
                return false;
            }
        });
        
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
    
    public void setOnTextInputListener(OnTextInputListener listener) {
        this.textInputListener = listener;
    }
    
    public void showKeyboard() {
        if (hiddenEditText != null) {
            hiddenEditText.requestFocus();
            imm.showSoftInput(hiddenEditText, InputMethodManager.SHOW_IMPLICIT);
            isKeyboardVisible = true;
            Log.d(TAG, "Native Android soft keyboard shown");
        }
    }
    
    public void hideKeyboard() {
        if (hiddenEditText != null) {
            imm.hideSoftInputFromWindow(hiddenEditText.getWindowToken(), 0);
            hiddenEditText.clearFocus();
            isKeyboardVisible = false;
            Log.d(TAG, "Native Android soft keyboard hidden");
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
    
    public void setInputType(int inputType) {
        if (hiddenEditText != null) {
            hiddenEditText.setInputType(inputType);
        }
    }
    
    public void setHint(String hint) {
        if (hiddenEditText != null) {
            hiddenEditText.setHint(hint);
        }
    }
    
    public void setText(String text) {
        if (hiddenEditText != null) {
            hiddenEditText.setText(text);
        }
    }
    
    public String getText() {
        return hiddenEditText != null ? hiddenEditText.getText().toString() : "";
    }
    
    public void clearText() {
        if (hiddenEditText != null) {
            hiddenEditText.setText("");
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