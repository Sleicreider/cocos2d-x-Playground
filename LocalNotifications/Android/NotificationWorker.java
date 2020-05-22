package org.cocos2dx.cpp;

import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;

import com.SleiTec.MemoryProject.R;

import androidx.annotation.NonNull;
import androidx.core.app.NotificationCompat;
import androidx.work.ExistingWorkPolicy;
import androidx.work.OneTimeWorkRequest;
import androidx.work.WorkManager;
import androidx.work.Worker;
import androidx.work.WorkerParameters;

import java.util.concurrent.TimeUnit;
import android.graphics.BitmapFactory;


public class NotificationWorker extends Worker
{
    private static Context context_;

    public NotificationWorker(@NonNull Context context, @NonNull WorkerParameters params)
    {
        super(context, params);
        context_ = context;
    }

    public static String WORK_ID = "sleitec-notification-request";

    // used on app start or close
    public static void RescheduleNotification()
    {
        OneTimeWorkRequest request = new OneTimeWorkRequest.Builder(NotificationWorker.class)
                .setInitialDelay(9, TimeUnit.HOURS)
                .build();

        WorkManager.getInstance(context_).enqueueUniqueWork(WORK_ID, ExistingWorkPolicy.REPLACE, request);
    }

    @Override
    public Result doWork()
    {
        final String CHANNEL_ID = "sleitec_magical_pairs";

        NotificationManager mNotificationManager = (NotificationManager)context_.getSystemService(Context.NOTIFICATION_SERVICE);

        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O)
        {
            NotificationChannel channel = new NotificationChannel(CHANNEL_ID,
                    "SleiTec_MagicalPairs_Notifications",
                    NotificationManager.IMPORTANCE_DEFAULT);
            channel.setDescription("SleiTec Magical Pairs Notifications");
            mNotificationManager.createNotificationChannel(channel);
        }

        NotificationCompat.Builder mBuilder = new NotificationCompat.Builder(getApplicationContext(), CHANNEL_ID)
                .setSmallIcon(R.drawable.ic_vector_material) // notification icon
                .setLargeIcon(BitmapFactory.decodeResource(getApplicationContext().getResources(), R.mipmap.ic_launcher)) // icon to the right inside the notification
                .setContentTitle("Magical Pairs - Memory Card Game") // title for notification
                .setContentText("Collect free credits! Find the Pairs!")// message for notification
                .setAutoCancel(true)  // clear notification after click
                .setVisibility(NotificationCompat.VISIBILITY_PUBLIC) //show with public settings
                ;

        Intent intent = new Intent(getApplicationContext(), AppActivity.class);
        PendingIntent pi = PendingIntent.getActivity(context_, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT);
        mBuilder.setContentIntent(pi);
        mNotificationManager.notify(0, mBuilder.build());

        OneTimeWorkRequest request = new OneTimeWorkRequest.Builder(NotificationWorker.class)
                .setInitialDelay(9, TimeUnit.HOURS)
                .build();

        /*prev_request = request.getId();*/
        WorkManager.getInstance(context_).enqueueUniqueWork(WORK_ID, ExistingWorkPolicy.REPLACE, request);

        return Result.success();
    }
}
