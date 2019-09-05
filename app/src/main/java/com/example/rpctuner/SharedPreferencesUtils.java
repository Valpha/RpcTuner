package com.example.rpctuner;


import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;

import java.util.Set;

/**
 * SharedPreferences为单例模式，在加载时自动初始化单例
 * 需要在Activity中初始化一下
 * <p>
 * 读写字符串、读写整型、读写长整型、读写布尔型、读写字符串集合
 * 一共十种方法
 *
 * @author Valpha
 */
public class SharedPreferencesUtils {

    private static SharedPreferencesUtils sharedPreferencesUtils = new SharedPreferencesUtils();
    private SharedPreferences sp;
    private SharedPreferences.Editor editor;

    public SharedPreferencesUtils() {
    }

    public static SharedPreferencesUtils getInstance() {
        return sharedPreferencesUtils;
    }

    public void initSharePreferences(Activity activity, String spName) {
        sp = activity.getSharedPreferences(spName, Context.MODE_PRIVATE);
        editor = sp.edit();
    }

    public void writeString(String key, String value) {
        editor.putString(key, value);
        editor.commit();
    }

    public void writeStringSet(String key, Set<String> set) {
        editor.putStringSet(key, set);
        editor.commit();
    }

    public void writeInt(String key, Integer value) {
        editor.putInt(key, value);
        editor.commit();
    }

    public void writeLong(String key, Long value) {
        editor.putLong(key, value);
        editor.commit();
    }

    public void writeBoolean(String key, boolean value) {
        editor.putBoolean(key, value);
        editor.commit();
    }

    public String readString(String key) {
        return sp.getString(key, null);
    }

    public Set<String> readStringSet(String key) {
        return sp.getStringSet(key, null);
    }

    public Integer readInt(String key) {
        return sp.getInt(key, -1);
    }

    public Boolean readBoolean(String key) {
        return sp.getBoolean(key, false);
    }

    public Long readLong(String key) {
        return sp.getLong(key, 0);
    }
}
