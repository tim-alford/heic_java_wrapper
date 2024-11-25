// vim: ts=2
#include <jni.h>
#include <com_ocka_HeifWrapper.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "heif_env.h"

// library and plguin paths
static const char * LIB_HEIF = "/usr/local/lib/libheif.so";
static const char * LIB_265_DECODER_PLUGIN = "/usr/local/lib/heif/libheif-libde265.so";

// function pointer table
// loaded with heif_env_init
static HeifEnv* e = NULL;
const static struct heif_plugin_info* info;

// JNI class fields
static jclass jcImageData = NULL;
static jfieldID fidMessage = NULL;
static jfieldID fidWidth = NULL;
static jfieldID fidHeight = NULL;
static jfieldID fidStride = NULL;
static jfieldID fidLength = NULL;
static jfieldID fidError = NULL;
static jfieldID fidBytes = NULL;
static jmethodID midConstructor = NULL;

int init(){
	if(e == NULL){
		e = (HeifEnv*) malloc(sizeof(HeifEnv));
		heif_env_init(LIB_HEIF, e);
		struct heif_error error = (*e->heif_load_plugin)(LIB_265_DECODER_PLUGIN, &info);
		if(error.code != 0){
			printf("Error, failed to load plugin \"%s\", message was %s\n", LIB_265_DECODER_PLUGIN, error.message);
			return 1;
		}
		return 0;
	}
	return 0;
}

int loadClass(JNIEnv* env){
	jcImageData = (*env)->FindClass(env, "com/ocka/ImageData");
 	if (jcImageData == NULL) 
		return 1;
	midConstructor = (*env)->GetMethodID(env, jcImageData, "<init>", "()V");
	fidMessage = (*env)->GetFieldID(env, jcImageData, "message", "Ljava/lang/String;");
	fidWidth = (*env)->GetFieldID(env, jcImageData, "width", "I");
	fidHeight = (*env)->GetFieldID(env, jcImageData, "height", "I");
	fidStride = (*env)->GetFieldID(env, jcImageData, "stride", "I");
	fidLength = (*env)->GetFieldID(env, jcImageData, "length", "I");
	fidError = (*env)->GetFieldID(env, jcImageData, "error", "Z");
	fidBytes = (*env)->GetFieldID(env, jcImageData, "bytes", "[I");
	if(fidWidth == NULL || fidHeight == NULL || 
			fidStride == NULL || fidLength == NULL || 
			fidError == NULL || fidBytes == NULL){
		return 1;
	}
	return 0;
}

jobject getImageDataObject(JNIEnv *env){
	return (*env)->NewObject(env, jcImageData, midConstructor, NULL);
}

JNIEXPORT jstring JNICALL Java_com_ocka_HeifWrapper_getVersion
  	(JNIEnv * env, jobject obj){
	// make sure that environment is loaded
	init();
	const char * version = (*e->heif_get_version)();
	return (*env)->NewStringUTF(env, version);
}

JNIEXPORT jobject JNICALL Java_com_ocka_HeifWrapper_getImageData
  (JNIEnv * env, jobject this_, jstring jFileName) {
	// make sure that environment is loaded
	// load meta data fields from class
	init();
	int result = loadClass(env);
	if(result != 0){
		printf("Error, failed to load class details\n");
		return NULL;
	}
	// allocate image data object
	// which will become the return result of the function
	jobject jImageData = getImageDataObject(env);
	// follow procedure from main test file ...
	jboolean jbCopy;
	const char* inputFile = (const char*) (*env)->GetStringUTFChars(env, jFileName, &jbCopy);
	jsize nLength = (*env)->GetStringUTFLength(env, jFileName);
	struct heif_context* ctx = (*e->heif_context_alloc)();
	struct heif_error error = (*e->heif_context_read_from_file)(ctx, inputFile, NULL);
	if(error.code != 0){
		const char * msg = error.message;
		jstring jsMsg = (*env)->NewStringUTF(env, msg);
		jboolean jbError = JNI_TRUE;
		(*env)->SetObjectField(env, jImageData, fidMessage, jsMsg);
		(*env)->SetBooleanField(env, jImageData, fidError, jbError);
		(*env)->ReleaseStringUTFChars(env, jFileName, inputFile);
		return jImageData;
	}
	(*env)->ReleaseStringUTFChars(env, jFileName, inputFile);
	struct heif_image_handle* image;
	error = (*e->heif_context_get_primary_image_handle)(ctx, &image);
	if(error.code != 0){
		const char * msg = error.message;
		jstring jsMsg = (*env)->NewStringUTF(env, msg);
		jboolean jbError = JNI_TRUE;
		(*env)->SetObjectField(env, jImageData, fidMessage, jsMsg);
		(*env)->SetBooleanField(env, jImageData, fidError, jbError);
		return jImageData;
	}
	struct heif_image * i;
	error = (*e->heif_decode_image)(image, &i, heif_colorspace_RGB, heif_chroma_interleaved_RGBA, NULL);
	if(error.code != 0){
		const char * msg = error.message;
		jstring jsMsg = (*env)->NewStringUTF(env, msg);
		jboolean jbError = JNI_TRUE;
		(*env)->SetObjectField(env, jImageData, fidMessage, jsMsg);
		(*env)->SetBooleanField(env, jImageData, fidError, jbError);
		return jImageData;
	}
	int stride;
	const uint8_t* bytes = (*e->heif_image_get_plane_readonly)(i, heif_channel_interleaved, &stride);
	int width = (*e->heif_image_get_width)(i, heif_channel_interleaved);
	int height = (*e->heif_image_get_height)(i, heif_channel_interleaved);
	int bpp = (*e->heif_image_get_bits_per_pixel_range)(i, heif_channel_interleaved);
	if(bytes == NULL){
		const char * msg = "Failed to read bytes from image";
		jstring jsMsg = (*env)->NewStringUTF(env, msg);
		jboolean jbError = JNI_TRUE;
		(*env)->SetObjectField(env, jImageData, fidMessage, jsMsg);
		(*env)->SetBooleanField(env, jImageData, fidError, jbError);
		return jImageData;
	}
	jboolean jbError = JNI_FALSE;
	const char * msg = "Success";
	nLength = stride * height;
	int * nBuffer = malloc(sizeof(int)*nLength);
	int j = 0;
	for(j = 0; j < nLength; j++){
		nBuffer[j] = bytes[j];
	}
	jstring jsMsg = (*env)->NewStringUTF(env, msg);
	(*env)->SetObjectField(env, jImageData, fidMessage, jsMsg);
	(*env)->SetBooleanField(env, jImageData, fidError, jbError);
	(*env)->SetIntField(env, jImageData, fidWidth, width);
	(*env)->SetIntField(env, jImageData, fidHeight, height);
	(*env)->SetIntField(env, jImageData, fidStride, stride);
	(*env)->SetIntField(env, jImageData, fidLength, (int)nLength);
	jintArray jBuffer = (*env)->NewIntArray(env, nLength);
	(*env)->SetIntArrayRegion(env, jBuffer, 0, nLength, nBuffer);
	(*env)->SetObjectField(env, jImageData, fidBytes, jBuffer);
	free(nBuffer);
	return jImageData;
}

