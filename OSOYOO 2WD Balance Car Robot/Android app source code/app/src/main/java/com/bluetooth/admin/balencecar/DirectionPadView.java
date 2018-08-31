package com.bluetooth.admin.balencecar;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

public final class DirectionPadView extends View {
    public DirectionPadView(Context context) {
        super(context);
        init(context);
    }

    public DirectionPadView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(context);
    }

    public DirectionPadView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context);
    }

    private final Rect dpadRect = new Rect();
    private final Rect centerRect = new Rect();
    private Drawable dpadNormal;
    private Drawable dpadActivated;
    private Drawable centerNormal;
    private Drawable centerActivated;
    private int dpadRotation = -1;

    private void init(Context context) {
        Resources res = context.getResources();
        dpadNormal = res.getDrawable(R.mipmap.ic_direction_pad);
        dpadActivated = res.getDrawable(R.mipmap.ic_direction_pad_activated);
        centerNormal = res.getDrawable(R.mipmap.ic_direction_center);
        centerActivated = res.getDrawable(R.mipmap.ic_direction_center_activated);
    }

    private OnKeyListener listener;

    public interface OnKeyListener {
        void onUpAction(DirectionPadView v);

        void onDownAction(DirectionPadView v);

        void onLeftAction(DirectionPadView v);

        void onRightAction(DirectionPadView v);

        void onCenterAction(DirectionPadView v);

        void onKeyUp(DirectionPadView v);
    }

    public void setOnKeyListener(OnKeyListener listener) {
        this.listener = listener;
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        if (w > 0 && h > 0) {
            int cx = w / 2;
            int cy = h / 2;
            int radius = Math.min(w, h) / 2;
            dpadRect.set(cx - radius, cy - radius, cx + radius, cy + radius);
            dpadNormal.setBounds(dpadRect);
            dpadActivated.setBounds(dpadRect);

            int cw = dpadRect.width() * centerNormal.getIntrinsicWidth() / dpadNormal.getIntrinsicWidth();
            int dx = radius - cw / 2;
            centerRect.set(dpadRect);
            centerRect.inset(dx, dx);
            centerNormal.setBounds(centerRect);
            centerActivated.setBounds(centerRect);
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        switch (event.getActionMasked()) {
            case MotionEvent.ACTION_DOWN: {
                float x = event.getX();
                float y = event.getY();
                testKeyDown(x, y);
                invalidate();
                break;
            }
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_CANCEL: {
                testKeyUp();
                invalidate();
                break;
            }
        }
        return isEnabled();
    }


    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        dpadNormal.draw(canvas);

        if (dpadRotation != -1) {
            canvas.save();
            canvas.rotate(dpadRotation, dpadRect.centerX(), dpadRect.centerY());
            dpadActivated.draw(canvas);
            canvas.restore();
        }

        if (keyState == KeyState.CENTER) {
            centerActivated.draw(canvas);
        } else {
            centerNormal.draw(canvas);
        }
    }

    private boolean isInCenter(float x, float y) {
        float radius = centerRect.width() / 2;
        double distance = Math.sqrt(
                (x - centerRect.centerX()) * (x - centerRect.centerX()) +
                        (y - centerRect.centerY()) * (y - centerRect.centerY())
        );
        return distance < radius;
    }

    private boolean isInDpad(float x, float y) {
        float radius = dpadRect.width() / 2;
        double distance = Math.sqrt(
                (x - dpadRect.centerX()) * (x - dpadRect.centerX()) +
                        (y - dpadRect.centerY()) * (y - dpadRect.centerY())
        );
        return distance < radius;
    }

    private enum KeyState {NONE, UP, DOWN, LEFT, RIGHT, CENTER}

    private KeyState keyState = KeyState.NONE;

    private void testKeyDown(float x, float y) {
        if (isInCenter(x, y)) {
            keyState = KeyState.CENTER;
            dpadRotation = -1;
            if (listener != null) {
                listener.onCenterAction(this);
            }
        } else if (isInDpad(x, y)) {
            int degree = getRotationBetweenLines(dpadRect.centerX(), dpadRect.centerY(), x, y);
            if (degree > 45 && degree < 135) {
                keyState = KeyState.RIGHT;
                dpadRotation = 270;
                if (listener != null) {
                    listener.onRightAction(this);
                }
            } else if (degree > 135 && degree < 225) {
                keyState = KeyState.DOWN;
                dpadRotation = 0;
                if (listener != null) {
                    listener.onDownAction(this);
                }
            } else if (degree > 225 && degree < 315) {
                keyState = KeyState.LEFT;
                dpadRotation = 90;
                if (listener != null) {
                    listener.onLeftAction(this);
                }
            } else if (degree > 315 || degree < 45) {
                keyState = KeyState.UP;
                dpadRotation = 180;
                if (listener != null) {
                    listener.onUpAction(this);
                }
            }
        }
    }

    private void testKeyUp() {
        keyState = KeyState.NONE;
        dpadRotation = -1;
        if (listener != null) {
            listener.onKeyUp(this);
        }
    }

    public boolean isLeftKey() {
        return keyState == KeyState.LEFT;
    }

    public boolean isRightKey() {
        return keyState == KeyState.RIGHT;
    }

    public boolean isUpKey() {
        return keyState == KeyState.UP;
    }

    public boolean isDownKey() {
        return keyState == KeyState.DOWN;
    }

    /**
     * 获取两条线的夹角
     */
    public static int getRotationBetweenLines(float centerX, float centerY, float xInView, float yInView) {
        double rotation = 0;
        double k1 = (double) (centerY - centerY) / (centerX * 2 - centerX);
        double k2 = (double) (yInView - centerY) / (xInView - centerX);
        double tmpDegree = Math.atan((Math.abs(k1 - k2)) / (1 + k1 * k2)) / Math.PI * 180;
        if (xInView > centerX && yInView < centerY) { //第一象限
            rotation = 90 - tmpDegree;
        } else if (xInView > centerX && yInView > centerY) { //第二象限
            rotation = 90 + tmpDegree;
        } else if (xInView < centerX && yInView > centerY) { //第三象限
            rotation = 270 - tmpDegree;
        } else if (xInView < centerX && yInView < centerY) { //第四象限
            rotation = 270 + tmpDegree;
        } else if (xInView == centerX && yInView < centerY) {
            rotation = 0;
        } else if (xInView == centerX && yInView > centerY) {
            rotation = 180;
        }
        return (int) rotation;
    }
}
