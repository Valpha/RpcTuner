//
// Created by Valpha on 2019/8/20.
//

#include <jni.h>
#include <CTunerApp.h>
#include <RPC_GLOBAL.h>

extern "C"
JNIEXPORT void JNICALL
Java_com_example_rpctuner_TunerApp_init(JNIEnv *env, jobject instance) {

    CRpcApp::getInstance();


}


extern "C"
JNIEXPORT jobject JNICALL
Java_com_example_rpctuner_TunerManager_setTunerFrequency(JNIEnv *env, jobject instance, jint freq_,
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
    fmlistener->rpcDataOkFlag = false;


    CTunerApp::getInstance()->registeTunerRPCLisiner(fmlistener);
    CTunerApp::getInstance()->setFrequency(freq);

    while (!fmlistener->rpcDataOkFlag) {

    }
    auto flag = static_cast<jboolean>(fmlistener->rpcDataOkFlag);
    jbyteArray data = env->NewByteArray(RPC_CMD_LENGTH);
    env->SetByteArrayRegion(data, 0, RPC_CMD_LENGTH,
                            reinterpret_cast<const jbyte *>(fmlistener->rpcData));

    env->ReleaseStringUTFChars(signature_, signature);
//    jobject jnimessage = env->NewObject(jniMessage, contructor, flag, data);
    jobject jnimessage = env->NewObject(jniMessage, contructor, flag, data);
    CTunerApp::getInstance()->unregisteTunerRPCLisiner(fmlistener);
    fmlistener->rpcDataOkFlag = false;
    return jnimessage;
}