package com.example.rpctuner;

import android.app.Application;

public class TunerApp extends Application {
    static {
        System.loadLibrary("tuner-lib");
        System.loadLibrary("RPCJNI");
        System.loadLibrary("rpc-lib");
    }

    @Override
    public void onCreate() {
        init();
        super.onCreate();
    }

    public native void init();
}
