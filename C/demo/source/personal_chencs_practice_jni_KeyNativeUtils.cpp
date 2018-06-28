#include "personal_chencs_practice_jni_KeyNativeUtils.h"
#include "KeyNativeUtilsAux.h"

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
	����GetByteArrayElements����ȡJava�ı�����������ת��C/C++�����飬�����ִ���ʽ��
	0������һ�����ݴ��ر��ش��룻1����ָ��Java�����ָ��ֱ�Ӵ��ص����ش���
	����GetArrayLength����ȡJava�����������͵ĳ��� 
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
	�����걾�ػ����������ReleaseByteArrayElements�ͷ����飬�����ִ���ʽ��
	0����Java���и��²��ͷ�C++����
	JNI_ABORT��������Java���飬�ͷ�C++����
	JNI_COMMIT������Java���飬���ͷ�C++����
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