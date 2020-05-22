package org.cocos2dx.cpp;

import android.content.Context;

import java.util.concurrent.TimeUnit;

import androidx.work.ExistingWorkPolicy;
import androidx.work.OneTimeWorkRequest;
import androidx.work.WorkManager;

public class NotificationController
{
    public static void InitNotification(Context context)
    {
        OneTimeWorkRequest request = new OneTimeWorkRequest.Builder(NotificationWorker.class)
                .setInitialDelay(9, TimeUnit.HOURS)
                .build();

        WorkManager.getInstance(context).enqueueUniqueWork(NotificationWorker.WORK_ID, ExistingWorkPolicy.REPLACE, request);
    }
}
