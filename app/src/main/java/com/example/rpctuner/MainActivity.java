package com.example.rpctuner;

import android.os.Bundle;
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


/**
 * @author Valpha
 */
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
    private TextView tv;

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
        // tv = findViewById(R.id.sample_text);
        // etFrequency = findViewById(R.id.et_freq);
    }

    public void setFrequency(View view) {
        String frequency = etFrequency.getText().toString().trim();
        singleThreadPool.execute(() -> {
            TunerJniMessage result = TunerManager.getInstance().setFrequency(frequency);
            runOnUiThread(() -> tv.setText(result.frequencyString));
            sendToast("收到命令！当前频率为" + result.frequencyString);
        });
        sendToast("设置命令已发送");
    }

    private void sendToast(String text) {
        runOnUiThread(() -> Toast.makeText(MainActivity.this, text, Toast.LENGTH_SHORT).show()
        );
    }

    @Override
    protected void onDestroy() {
        singleThreadPool.shutdown();
        super.onDestroy();
    }
}

