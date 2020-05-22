 Local Notifications:
 
When making mobile games, you probably want to notify the player that something is happening in your game, to keep the player engaged and build player retention. There are a lot of Push notification libs out there, but none really support offline local notifications. Since not every game or feature in the game requires internt access, letting your game schedule notificaitons might be great to keep the player playing.

For my current games, I was using an in game currency, which the player is able to collected every now and then besides playing and winning. To let the user know new credits are available I'm triggering notificaitons some times, for both, iOS and Android.
Also I want to reschedule notifications, for example if the player is playing all the time anyway, I will simply reschedule the Notification to a future point again, so it's not too annyoing.

For iOS it was suprisingly more straightforward than Android (be carefully my Obj-C code might be not the best ._.)
On android you need to be aware that after restarting the phone, you need to schedule a worker a gain.
On iOS mainly their Notification system. 

(More comments will follow soon, let me know if you want to more info)
(Current code is mainly an initial draft, including hardcoded timing numbers)
