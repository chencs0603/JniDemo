#include "personal_chencs_practice_jni_KeyNativeUtils.h"
#include "KeyNativeUtilsAux.h"
#include <memory.h>

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
	printf("JNI On Load...");

	JNIEnv *env = NULL;
	jint result;

	if (vm->GetEnv((void **) &env, JNI_VERSION_1_8) != JNI_OK) {
		result = -1;
	} else {
		result = JNI_VERSION_1_8;
	}

	return result;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {
	printf("JNI On UnLoad...");
}

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

JNIEXPORT jobjectArray JNICALL Java_personal_chencs_practice_jni_KeyNativeUtils_arraySplit
(JNIEnv *env, jclass thiz, jbyteArray bytes, jint blockLen)
{
	jbyte* pu1Bytes = env->GetByteArrayElements(bytes, 0);
	jsize u4BytesLen = env->GetArrayLength(bytes);

	// 计算二维数组的行和列
	int row = u4BytesLen/blockLen + 1;
	int col = blockLen;
	// 二维数组与String数组类似，可以看成Object数组
 	jclass byteArrCls = env->FindClass("[B");// 获取一维数组类的引用
 	jobjectArray objectArrayResult = env->NewObjectArray(row, byteArrCls, NULL);// 创建Object数组，数组元素类型为一维数组

	for (int i = 0; i < row - 0x01; i++)
	{
		// 创建一维数组
		jbyteArray tempArray = env->NewByteArray(col);
		// 给一维数组赋值
		env->SetByteArrayRegion(tempArray, 0, col, (const jbyte*)(pu1Bytes + i*col));  
		// 将一维数组赋值给Object数组的第i个元素
		env->SetObjectArrayElement(objectArrayResult, i, tempArray);
		// 释放一维数组
		env->DeleteLocalRef(tempArray); 
	}
	// 最后一个数组的长度不一定是col
	int lastArrayLen = u4BytesLen - (row - 0x01)*col;
	jbyteArray lastArray = env->NewByteArray(lastArrayLen);
	env->SetByteArrayRegion(lastArray, 0, lastArrayLen, (const jbyte*)(pu1Bytes + (row - 0x01)*col));
	env->SetObjectArrayElement(objectArrayResult, row - 0x01, lastArray);  
	env->DeleteLocalRef(lastArray);

END:
	env->ReleaseByteArrayElements(bytes, (jbyte*)pu1Bytes, JNI_ABORT);

	return objectArrayResult;
}