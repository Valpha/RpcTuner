//
// Created by Valpha on 2019/8/20.
//

#include <jni.h>
#include <string>

#include <RPC_GLOBAL.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_rpctuner_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_rpctuner_MainActivity_rpcJNI(JNIEnv *env, jobject instance) {

    // TODO


}