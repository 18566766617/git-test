package com.example.myapplication;

import android.util.Log;

/**
 * Created by Ace on 2017/8/5.
 */

//专门调用jni C++接口的类
public class OBOJNI {
    //加载cpp给提供的 动态库
    static {
        try {
            System.loadLibrary("testjni");//libtestjni.so
        } catch (Exception e) {
            e.printStackTrace();
            Log.e("tag", e.getMessage());
        }

    }

    public static OBOJNI getInstance() {
        if (instance == null) {
            instance = new OBOJNI();
        }

        return instance;
    }

    private static OBOJNI instance = null;


    //提供一个调用JNI接口的成员方法
    public native void hello_jni();

    public native void hello_jni2();

    public native int test_jni_api(int a, int b);

    public native boolean test_jni_api2(boolean a);

    public native String test_jni_api3(String str1, String str2);

    public native void test_jni_api4_array(int[] array);


    //登陆的jni接口login
    public native boolean login(String username, String passwd, boolean isDriver);


    public native boolean mylogin(String username, String passwd, boolean isDriver);

}
