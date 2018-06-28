#include "personal_chencs_practice_jni_KeyNativeUtils.h"
#include "KeyNativeUtilsAux.h"

JNIEXPORT jint JNICALL Java_personal_chencs_practice_jni_KeyNativeUtils_getKeyElements
(JNIEnv *env, jclass thiz, jbyteArray keyElement1, jintArray keyElement1Len, jbyteArray keyElement2, jintArray keyElement2Len)
{
	unsigned char*	pu1KeyElement1 = (unsigned char*)env->GetByteArrayElements(keyElement1, 0);
	unsigned int*	pu4KeyElement1 = (unsigned int*)env->GetIntArrayElements(keyElement1Len, 0);
	unsigned char*  pu1KeyElement2 = (unsigned char*)env->GetByteArrayElements(keyElement2, 0);
	unsigned int*	pu4KeyElement2 = (unsigned int*)env->GetIntArrayElements(keyElement2Len, 0);

	unsigned int	u4Result = getKeyElements(pu1KeyElement1, pu4KeyElement1, pu1KeyElement2, pu4KeyElement2);

	env->ReleaseByteArrayElements(keyElement1, (jbyte*)pu1KeyElement1, 0);
	env->ReleaseIntArrayElements(keyElement1Len, (jint*)pu4KeyElement1, 0);
	env->ReleaseByteArrayElements(keyElement2, (jbyte*)pu1KeyElement2, 0);
	env->ReleaseIntArrayElements(keyElement2Len, (jint*)pu4KeyElement2, 0);

	return (jint)u4Result;
}

JNIEXPORT jbyteArray JNICALL Java_personal_chencs_practice_jni_KeyNativeUtils_getKey
(JNIEnv *env, jclass thiz, jbyteArray keyElement1, jbyteArray keyElement2)
{	
	/************************************************************************/
	/* 
	利用GetByteArrayElements来获取Java的本地数组类型转换C/C++的数组，有两种处理方式：
	0：拷贝一份数据传回本地代码；1：把指向Java数组的指针直接传回到本地代码
	利用GetArrayLength来获取Java本地数组类型的长度 
	*/
	/************************************************************************/
	unsigned char*	pu1KeyElement1 = (unsigned char*)env->GetByteArrayElements(keyElement1, 0);
	unsigned int	u4KeyElement1Len = (unsigned int)env->GetArrayLength(keyElement1);
	unsigned char*  pu1KeyElement2 = (unsigned char*)env->GetByteArrayElements(keyElement2, 0);
	unsigned int	u4KeyElement2Len = (unsigned int)env->GetArrayLength(keyElement2);

	unsigned int	u4KeyLen = 0x100;
	unsigned char*	pu1Key = new unsigned char[u4KeyLen];

	jbyteArray		key = NULL;

	unsigned int	u4Result = getKey(pu1KeyElement1, u4KeyElement1Len, pu1KeyElement2, u4KeyElement2Len, pu1Key, &u4KeyLen);
	if (0 != u4Result)
	{
		goto END;
	}

	key = env->NewByteArray(u4KeyLen);
	env->SetByteArrayRegion(key, 0, u4KeyLen, (jbyte*)pu1Key);

END:
	/************************************************************************/
	/* 
	处理完本地化数组后，利用ReleaseByteArrayElements释放数组，有三种处理方式：
	0：对Java进行更新并释放C++数组
	JNI_ABORT：不更新Java数组，释放C++数组
	JNI_COMMIT：更新Java数组，不释放C++数组
	*/
	/************************************************************************/
	env->ReleaseByteArrayElements(keyElement1, (jbyte*)pu1KeyElement1, JNI_ABORT);
	env->ReleaseByteArrayElements(keyElement2, (jbyte*)pu1KeyElement2, JNI_ABORT);
	if (NULL != pu1Key)
	{
		delete[] pu1Key;
	}

	return key;
}