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
            System.loadLibrary("OBOjni");//libtestjni.so
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




    //登陆 JNI模块
    public native boolean Login(String username, String passwd, boolean isDriver);

    //注册 JNI模块
    public native boolean Reg(String username,
                              String passwd,
                              String tel,
                              String email,
                              String idcard,
                              boolean isDriver);
    //开始下单 JNI模块
    public native boolean StartOrder(String src_longitude,
                                     String src_latitude,
                                     String src_address,
                                     String dst_longitude,
                                     String dst_latitude,
                                     String dst_address,
                                     String RMB);

    //司机端定位发生改变，上传地理位置信息 JNI模块
    public native  boolean DriverLocationChanged(String longitude,
                                                 String latitude,
                                                 String address,
                                                 String autoSend);

    //乘客端定位发生改变，上传地理位置信息 JNI模块
    public native  boolean PassengerLocationChanged(String longitude,
                                                    String latitude,
                                                    String address,
                                                    String dst_longitude,
                                                    String dst_latitude,
                                                    String dst_address);



    public native boolean FinishOrder();
    public native void setStatus(String status);
    public native String getOrderid();
    public native String getSessionid();
    public native String getStatus();
    public native String getIsDriver();
    public native String getPtempLongitude();
    public native String getPtempLatitude();
    public native String getDtempLongitude();
    public native String getDtempLatitude();
    public native void testLibcurl();








    public native void hello_jni();

    public native void hello_jni2();

    public native int test_jni_api(int a, int b);

    public native boolean test_jni_api2(boolean a);

    public native String test_jni_api3(String str1, String str2);

    public native void test_jni_api4_array(int[] array);

    public native boolean mylogin(String username, String passwd, boolean isDriver);



}
