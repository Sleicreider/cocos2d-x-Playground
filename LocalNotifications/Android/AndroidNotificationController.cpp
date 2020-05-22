#include "FGeneral.h"

#include "AndroidNotificationController.h"
#include "platform/android/jni/JniHelper.h"

constexpr char* AndroidNotificationController_jni_class = "org/cocos2dx/cpp/NotificationWorker";

void AndroidNotificationController::RescheduleNotification()
{
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, AndroidNotificationController_jni_class, "RescheduleNotification", "()V"))
	{
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
	}
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
}
