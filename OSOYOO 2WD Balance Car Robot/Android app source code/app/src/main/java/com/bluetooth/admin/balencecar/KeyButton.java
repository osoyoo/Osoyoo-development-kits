package com.bluetooth.admin.balencecar;

import android.content.Context;
import android.support.v7.widget.AppCompatButton;
import android.util.AttributeSet;
import android.view.MotionEvent;

public class KeyButton extends AppCompatButton {
    public KeyButton(Context context) {
        super(context);
    }

    public KeyButton(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public KeyButton(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    public interface OnActionListener {
        void onKeyDown(KeyButton v);

        void onKeyUp(KeyButton v);
    }

    private OnActionListener listener;

    public void setOnActionListener(OnActionListener listener) {
        this.listener = listener;
    }

    private boolean down;

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        super.onTouchEvent(event);
        switch (event.getActionMasked()) {
            case MotionEvent.ACTION_DOWN: {
                down = true;
                if (listener != null) {
                    listener.onKeyDown(this);
                }
                break;
            }
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_CANCEL: {
                down = false;
                if (listener != null) {
                    listener.onKeyUp(this);
                }
                break;
            }
        }
        return isEnabled();
    }

    public boolean isKeyDown() {
        return down;
    }
}
