package org.cocos2dx.cpp;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import java.util.concurrent.TimeUnit;

import androidx.work.ExistingWorkPolicy;
import androidx.work.OneTimeWorkRequest;
import androidx.work.WorkManager;

/**
 * Solution to the notification request not being processed anymore
 * after the phone got restarted.
 * This broadcast receiver simply listens to BOOT_COMPLETED actions
 * and runs our notification request again.
 */
public class NotificationBroadcastReceiver extends BroadcastReceiver
{
    @Override
    public void onReceive(Context context, Intent intent)
    {
        OneTimeWorkRequest request = new OneTimeWorkRequest.Builder(NotificationWorker.class)
                .setInitialDelay(20, TimeUnit.MINUTES)
                .build();

        WorkManager.getInstance(context).enqueue(request);
    }
}
