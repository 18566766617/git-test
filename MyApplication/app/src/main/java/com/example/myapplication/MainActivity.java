package com.example.myapplication;

import android.Manifest;
import android.annotation.TargetApi;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    String[] permissions = {
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.READ_PHONE_STATE,
            Manifest.permission.ACCESS_COARSE_LOCATION,
            Manifest.permission.ACCESS_FINE_LOCATION};

    private int requestCode = 1;

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
            //关联控件
            initUI();
            //给登陆按钮绑定一个事件
            setOnclick();
        }


    }

    @TargetApi(Build.VERSION_CODES.M)
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

        if (requestCode != -1 && requestCode == requestCode) {

            boolean isVerifyPermission = true;//验证所有权限是否都已经授权
            for (int grantResult : grantResults) {
                if (grantResult != PackageManager.PERMISSION_GRANTED) {
                    isVerifyPermission = false;
                    break;
                }
            }
            if (isVerifyPermission) {
                //都授权了，执行初始化控件动作
                Log.e("tag", "权限都已申请1");
                //关联控件
                initUI();
                //给登陆按钮绑定一个事件
                setOnclick();
            } else {
                //有一个未授权或者多个未授权

                for (String permission : permissions) {
                    Log.e("tag", "permissions:" + permission);
                    if (shouldShowRequestPermissionRationale(permission)) {
                        //点击了拒绝
                        Toast.makeText(this, "请同意权限以精准定位", Toast.LENGTH_LONG).show();
                        finish();
                    } else {
                        //点击了不在询问
                        Toast.makeText(this, "请到权限页面开启权限", Toast.LENGTH_LONG).show();
                        finish();
                    }
                }

            }
        }
    }

    public static String LogTag = "OBO-MainActivity";

    private Button bt_login = null;
    private Button bt_reg = null;
    private EditText et_username = null;
    private EditText et_passwd = null;
    private CheckBox cb_isDriver_login = null;
    private boolean isDriver = false;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        getPermissions();

    }

    private void initUI() {
        bt_login = (Button) findViewById(R.id.bt_login);
        bt_reg = (Button) findViewById(R.id.bt_reg);
        et_username = (EditText) findViewById(R.id.et_username);
        et_passwd = (EditText) findViewById(R.id.et_passwd);
        cb_isDriver_login = (CheckBox) findViewById(R.id.login_cb_isDriver);
    }

    private void setOnclick() {
        cb_isDriver_login.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    isDriver = true;
                } else {
                    isDriver = false;
                }
            }
        });


        //绑定登陆按钮的点击事件
        bt_login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String username = et_username.getText().toString();
                String passwd = et_passwd.getText().toString();
                Log.e(LogTag, "username:" + username);
                Log.e(LogTag, "passwd:" + passwd);

                if (username.length() == 0 || passwd.length() == 0) {
                    Toast.makeText(getApplicationContext(),
                            "用户名或密码为空", Toast.LENGTH_SHORT).show();
                    return;
                }

                boolean login_result = OBOJNI.getInstance().Login(username, passwd, isDriver);
                Log.e(LogTag, "Login result is " + login_result);

                if (login_result == true) {
                    if (OBOJNI.getInstance().getIsDriver().equals("no")) {
                        Intent intent = new Intent();
                        intent.setClass(MainActivity.this, PassengerActivity.class);
                        startActivity(intent);
                    } else {
                        Intent intent = new Intent();
                        intent.setClass(MainActivity.this, DriverActivity.class);
                        startActivity(intent);
                    }
                } else {
                    Toast.makeText(getApplicationContext(),
                            "登陆失败-无法连接服务器", Toast.LENGTH_SHORT).show();
                    Log.e(LogTag, "Login error！");
                }

            }
        });


        bt_reg.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent();
                intent.setClass(MainActivity.this, RegActivity.class);
                startActivity(intent);
            }
        });
    }


}
