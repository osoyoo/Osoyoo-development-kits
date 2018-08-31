
package com.bluetooth.admin.balencecar;

import android.os.Bundle;
import android.os.Looper;
import android.support.v7.app.AppCompatActivity;
import android.view.MenuItem;
import java.util.ArrayList;
import java.util.List;

public abstract class BaseActivity extends AppCompatActivity {
    private boolean canUpdateUI;
    private List<Runnable> pendingTasks;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        canUpdateUI = true;
        super.onCreate(savedInstanceState);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            finish();
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onUserLeaveHint() {
        super.onUserLeaveHint();
        canUpdateUI = false;
    }

    @Override
    protected void onPause() {
        super.onPause();
        canUpdateUI = false;
    }

    @Override
    protected void onPostResume() {
        super.onPostResume();
        canUpdateUI = true;
        if (pendingTasks != null && pendingTasks.size() > 0) {
            for (Runnable task : pendingTasks) {
                task.run();
            }
            pendingTasks.clear();
        }
    }

    @Override
    protected void onResumeFragments() {
        super.onResumeFragments();
        canUpdateUI = true;
    }

    protected boolean canUpdateUI() {
        return canUpdateUI;
    }

    protected void handleTaskInForeground(Runnable task) {
        if (canUpdateUI()) {
            if (Looper.myLooper() != Looper.getMainLooper()) {
                runOnUiThread(task);
            } else {
                task.run();
            }
        } else {
            if (pendingTasks == null) {
                pendingTasks = new ArrayList<>();
            }

            if (!pendingTasks.contains(task)) {
                pendingTasks.add(task);
            }
        }
    }
}
