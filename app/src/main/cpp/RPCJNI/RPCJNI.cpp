//
// Created by Valpha on 2019/8/20.
//

#include <jni.h>
#include <string>
#include <CTunerApp.h>
#include <RPC_GLOBAL.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_rpctuner_MainActivity_stringFromJNI(JNIEnv *env, jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}




extern "C"
JNIEXPORT jobject JNICALL
Java_com_example_rpctuner_MainActivity_setTunerFrequency(JNIEnv *env, jobject instance, jint freq_,
                                                         jstring signature_) {
    const char *signature = env->GetStringUTFChars(signature_, 0);
    auto freq = static_cast<uLong>(freq_);
//    jclass exception = env->FindClass("java/lang/Exception");
    jclass jniMessage = env->FindClass("com/example/rpctuner/JniMessage");
    /*if(jniMessage==NULL){
        env->ThrowNew(exception,"找不到java类:com.example.rpctuner.JNIMessage");
    }*/
    jmethodID contructor = env->GetMethodID(jniMessage, "<init>", signature);


    auto *fmlistener = new CFMRadioRpcListener();
    fmlistener->rpcdataokflag = false;
    CTunerApp::getInstance()->registeTunerRPCLisiner(fmlistener);
    CTunerApp::getInstance()->setFrequency(freq);

    while (!fmlistener->rpcdataokflag) {

    }
    jboolean flag = fmlistener->rpcdataokflag;
    jbyteArray data = env->NewByteArray(RPC_CMD_LENGTH);
    env->SetByteArrayRegion(data, 0, RPC_CMD_LENGTH,
                            reinterpret_cast<const jbyte *>(fmlistener->rpcdata));

    env->ReleaseStringUTFChars(signature_, signature);
//    jobject jnimessage = env->NewObject(jniMessage, contructor, flag, data);
    jobject jnimessage = env->NewObject(jniMessage, contructor, flag, data);
    fmlistener->rpcdataokflag = false;
    return jnimessage;
}