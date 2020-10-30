package com.example.myapplication;

import android.Manifest;
import android.annotation.TargetApi;
import android.content.pm.PackageManager;
import android.graphics.BitmapFactory;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;

import com.amap.api.location.AMapLocation;
import com.amap.api.location.AMapLocationClient;
import com.amap.api.location.AMapLocationClientOption;
import com.amap.api.location.AMapLocationListener;
import com.amap.api.maps.AMap;
import com.amap.api.maps.AMapOptions;
import com.amap.api.maps.CameraUpdateFactory;
import com.amap.api.maps.MapView;
import com.amap.api.maps.UiSettings;
import com.amap.api.maps.model.BitmapDescriptorFactory;
import com.amap.api.maps.model.LatLng;
import com.amap.api.maps.model.Marker;
import com.amap.api.maps.model.MarkerOptions;

import java.util.ArrayList;
import java.util.List;


public class DriverActivity extends AppCompatActivity {
    String[] permissions = {
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.READ_PHONE_STATE,
            Manifest.permission.ACCESS_COARSE_LOCATION,
            Manifest.permission.ACCESS_FINE_LOCATION};

    private int requestCode = 1;


    //创建一个地图容器MapView对象
    private MapView _mapView = null;
    //地图的UISetting对象 给amap设置地图内嵌控件
    private UiSettings _uiSettings = null;
    //地图对象
    private AMap _amap = null;

    //定位服务器客户端句柄
    private AMapLocationClient _amapLocationClient = null;
    //定位服务器客户端句柄属性
    private AMapLocationClientOption _amapLocationOption = null;

    //显示自我位置的图标
    private Marker _selfMarker = null;


    boolean isAddSelfMarker = false;
    int flag = 0;
    int traffic_flag = 0;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_driver);
        getPermissions();

        initUI();
        createMap(savedInstanceState);
        doLocation();

    }

    @TargetApi(Build.VERSION_CODES.M)
    private void getPermissions() {

        List<String> deniedPermissions = new ArrayList<>();
        for (String permission : permissions) {
            if (ContextCompat.checkSelfPermission(this, permission) == PackageManager.PERMISSION_DENIED) {
                deniedPermissions.add(permission);
            }
        }


        if (deniedPermissions.size() > 0) {
            //没有授权过，去申请一下
            requestPermissions(deniedPermissions.toArray(new String[deniedPermissions.size()]), requestCode);
        } else {
            Log.e("tag", "权限都已申请2");
        }


    }

    protected void initUI() {
        //将地图容器跟MapView控件相关联
        _mapView = (MapView) findViewById(R.id.DriverMap);
    }

    protected void createMap(Bundle savedInstanceState) {
        //展示地图容器
        _mapView.onCreate(savedInstanceState);


        //得到amap对象
        _amap = _mapView.getMap();

        //默认显示实时交通信息
        _amap.setTrafficEnabled(true);


        //得到UISettings
        _uiSettings = _amap.getUiSettings();

        //添加一个指南针控件
        _uiSettings.setCompassEnabled(true);

        //添加一个缩放比例尺
        _uiSettings.setScaleControlsEnabled(true);

        //修改logo位置
        _uiSettings.setLogoPosition(AMapOptions.LOGO_POSITION_BOTTOM_RIGHT);

    }

    //启动定位服务器
    protected void doLocation() {
        //1 创建一个客户端定位句柄
        _amapLocationClient = new AMapLocationClient(getApplicationContext());

        //1.5 给定位客户端设置一些属性
        _amapLocationOption = new AMapLocationClientOption();
        //每个5s定位一次
        _amapLocationOption.setInterval(3000);
        //_amapLocationOption.setOnceLocation(true);

        //将option设置给client对象
        _amapLocationClient.setLocationOption(_amapLocationOption);

        //2 给客户端句柄设置一个listenner来处理服务器返回的定位数据
        _amapLocationClient.setLocationListener(new AMapLocationListener() {
            @Override
            public void onLocationChanged(AMapLocation aMapLocation) {
                //onLocationChanged 就是如果服务器给客户端返回数据，调用的回调函数
                //aMapLocation 就是服务器给客户端返回的定位数据

                if (aMapLocation != null) {
                    //服务器是有响应的

                    if (aMapLocation.getErrorCode() == 0) {
                        //定位成功，aMapLocation获取数据
                        Log.e("Amap", "location succ address = " + aMapLocation.getAddress());
                        Log.e("Amap", "city = " + aMapLocation.getCity());
                        Log.e("Amap", "longtitude = " + aMapLocation.getLongitude());
                        Log.e("Amap", "latitude = " + aMapLocation.getLatitude());

                        if (isAddSelfMarker == false) {
                            //在此位置添加一个标记
                            addMarkerToMap(aMapLocation.getLatitude(), aMapLocation.getLongitude());
                            isAddSelfMarker = true;

                            //以自我为中心展示地图
                            moveMap(aMapLocation.getLatitude(), aMapLocation.getLongitude());
                        }


                    } else {
                        //定位失败，

                        Log.e("tag", "location error, code = " + aMapLocation.getErrorCode() +
                                ", info = " + aMapLocation.getErrorInfo());
                    }
                }
            }
        });

        //3 开启定位服务
        _amapLocationClient.startLocation();
    }


    //向固定的经纬度添加一个标记
    protected void addMarkerToMap(double latitude, double longitude) {
        _selfMarker = _amap.addMarker(new MarkerOptions().position(new LatLng(latitude, longitude))
                .icon(BitmapDescriptorFactory.fromBitmap(BitmapFactory.decodeResource(getResources(), R.drawable.location_marker))));
    }

    //以某个经纬度为中心来展示地图
    protected void moveMap(double latitude, double longtiude) {
        LatLng lagLng = new LatLng(latitude, longtiude);

        //移动amap地图 以之前的缩放比例展示
        _amap.animateCamera(CameraUpdateFactory.newLatLngZoom(lagLng, _amap.getCameraPosition().zoom));
    }


    @Override
    protected void onResume() {
        super.onResume();
        Log.e("tag", "onResume()...");
        _mapView.onResume();
    }


    @Override
    protected void onPause() {
        super.onPause();
        Log.e("tag", "onPause()...");
        _mapView.onPause();
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        Log.e("tag", "onSaveInstanceState()...");
        _mapView.onSaveInstanceState(outState);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.e("tag", "onDestroy()...");
        _mapView.onDestroy();
    }

    @TargetApi(Build.VERSION_CODES.M)
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

        if (requestCode != -1 && requestCode == requestCode) {

            boolean isVerifyPermission = false;//验证所有权限是否都已经授权
            for (String permission : permissions) {
                Log.e("tag", "permissions:" + permission);
            }
            for (int grantResult : grantResults) {
                if (grantResult != PackageManager.PERMISSION_GRANTED) {
                    isVerifyPermission = false;
                } else {
                    isVerifyPermission = true;
                }

            }
            if (isVerifyPermission) {
                //都授权了，执行初始化控件动作
                Log.e("tag", "权限都已申请1");
            } else {
                //有一个未授权或者多个未授权
                Toast.makeText(this, "请同意权限以精准定位", Toast.LENGTH_LONG).show();
                boolean isbool=shouldShowRequestPermissionRationale(permissions[0]);

                Log.e("tag", "isbool:"+isbool);
            }
        }
    }
}
