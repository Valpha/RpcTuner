package com.example.rpctuner;

import android.util.Log;

public class TunerManager {
    private static final TunerManager ourInstance = new TunerManager();
    private static final String TAG = "TunerManager";

    private TunerManager() {
        System.loadLibrary("RPCJNI");
    }

    public static TunerManager getInstance() {
        return ourInstance;
    }

    private native JniMessage setTunerFrequency(int freq, String signature);

    public TunerJniMessage setFrequency(int frequency) {

        String signature = SignatureGen.getSignature(Void.class, Boolean.class, Byte[].class);
        Log.d(TAG, "setFrequency: ---JNI:setTunerFrequency Called!");
        JniMessage result = setTunerFrequency(frequency, signature);
        Log.d(TAG, "setFrequency: ---JNI:Got result!");
        return new TunerJniMessage(result);
    }

    public TunerJniMessage setFrequency(String frequencyString) {
        int frequency = Integer.valueOf(frequencyString);
        String signature = SignatureGen.getSignature(Void.class, Boolean.class, Byte[].class);
        Log.d(TAG, "setFrequency: ---JNI:setTunerFrequency Called!");
        JniMessage result = setTunerFrequency(frequency, signature);
        Log.d(TAG, "setFrequency: ---JNI:Got result!");
        return new TunerJniMessage(result);
    }

}

class TunerJniMessage {
    public int frequency;
    public String frequencyString;
    public JniMessage baseMessage;

    TunerJniMessage(JniMessage baseMessage) {
        this.baseMessage = baseMessage;
        int freq = 0;
        freq <<= 8;
        freq |= (baseMessage.data[0 + 3] & 0xff);
        freq <<= 8;
        freq |= (baseMessage.data[1 + 3] & 0xff);
        freq <<= 8;
        freq |= (baseMessage.data[2 + 3] & 0xff);
        this.frequency = freq;
        this.frequencyString = String.valueOf(freq);
    }

}
