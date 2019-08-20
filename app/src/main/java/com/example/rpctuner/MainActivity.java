package com.example.rpctuner;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.google.common.util.concurrent.ThreadFactoryBuilder;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;


public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";

    // Used to load the 'native-lib' library on application startup.
    static {

        System.loadLibrary("tuner-lib");
        System.loadLibrary("RPCJNI");
        System.loadLibrary("rpc-lib");
        // System.loadLibrary("native-lib");
    }

    private EditText etFrequency;
    private ExecutorService singleThreadPool;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ThreadFactory namedThreadFactory = new ThreadFactoryBuilder()
                .setNameFormat("demo-pool-%d").build();
        singleThreadPool = new ThreadPoolExecutor(1, 1,
                0L, TimeUnit.MILLISECONDS,
                new LinkedBlockingQueue<Runnable>(1024),
                namedThreadFactory, new ThreadPoolExecutor.AbortPolicy());
        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        etFrequency = findViewById(R.id.et_freq);
        tv.setText(stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native JniMessage setTunerFrequency(int freq, String signature);

    public void setFrequency(View view) {
        int frequency = Integer.valueOf(etFrequency.getText().toString().trim());
        singleThreadPool.execute(() -> {
            Log.d(TAG, "setFrequency: ---" + Thread.currentThread().getName());
            String signature = SignatureGen.getSignature(Void.class, Boolean.class, Byte[].class);


            Log.d(TAG, "setFrequency: ---JNI:setTunerFrequency Called!");
            JniMessage result = setTunerFrequency(frequency, signature);
            if (result.complete) {
                Log.d(TAG, "setFrequency: ---result is " + result.complete);
                int freq = 0;
                freq <<= 8;
                freq |= (result.data[0 + 3] & 0xff);
                freq <<= 8;
                freq |= (result.data[1 + 3] & 0xff);
                freq <<= 8;
                freq |= (result.data[2 + 3] & 0xff);
                sendToast("设置成功！当前频率为" + freq);
            } else {
                Log.d(TAG, "setFrequency: ---result is " + result.complete);
                sendToast("设置失败！");

            }
        });


        sendToast("设置命令已发送");
    }

    public void sendToast(String text) {
        runOnUiThread(() -> Toast.makeText(MainActivity.this, text, Toast.LENGTH_SHORT).show()
        );
    }

    @Override
    protected void onDestroy() {
        singleThreadPool.shutdown();
        super.onDestroy();
    }
}

