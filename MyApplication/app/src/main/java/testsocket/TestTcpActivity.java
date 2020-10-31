package testsocket;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.ToggleButton;

import androidx.appcompat.app.AppCompatActivity;

import com.example.myapplication.R;

import java.io.IOException;

public class TestTcpActivity extends AppCompatActivity {

    private EditText edtTxt_Addr;
    private EditText edtTxt_Port;
    private ToggleButton tglBtn;
    private TextView tv_Msg;
    private EditText edtTxt_Data;
    private Button btn_Send;

    private TcpClientConnector connector;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_test_tcp);
//        WebView webView = findViewById(R.id.webview);
//        webView.loadUrl("http://www.baidu.com");
//
//        webView.setWebViewClient(new WebViewClient() {
//            @Override
//            public boolean shouldOverrideUrlLoading(WebView view, String url) {
//                //使用WebView加载显示url
//                view.loadUrl(url);
//                //返回true
//                return true;
//            }
//        });

        InitWidgets();
        connector = TcpClientConnector.getInstance();   //获取connector实例
        tglBtn.setOnCheckedChangeListener(new TestTcpActivity.TglBtnCheckedChangeEvents());
        btn_Send.setOnClickListener(new TestTcpActivity.ButtonClickEvent());


    }


    private void InitWidgets() {
        edtTxt_Addr = (EditText) findViewById(R.id.edtTxt_Addr);
        edtTxt_Port = (EditText) findViewById(R.id.edtTxt_Port);
        tglBtn = (ToggleButton) findViewById(R.id.tglBtn);
        tv_Msg = (TextView) findViewById(R.id.tv_Msg);
        edtTxt_Data = (EditText) findViewById(R.id.edtTxt_Data);
        btn_Send = (Button) findViewById(R.id.btn_Send);
    }


    private class TglBtnCheckedChangeEvents implements ToggleButton.OnCheckedChangeListener {

        @Override
        public void onCheckedChanged(CompoundButton btnView, boolean isChecked) {
            if (btnView == tglBtn) {
                if (isChecked == true) {
                    //连接Tcp服务器端
                    Log.e("tag", "connect start");
                    //connector.createConnect("172.16.46.41",8888);   //调试使用
                    Log.e("tag", "ip:" + edtTxt_Addr.getText().toString() + "||||" + "port:" + Integer.parseInt(edtTxt_Port.getText().toString()));
                    connector.createConnect(edtTxt_Addr.getText().toString(), Integer.parseInt(edtTxt_Port.getText().toString()));
                    connector.setOnConnectListener(new TcpClientConnector.ConnectListener() {
                        @Override
                        public void onReceiveData(String data) {
                            //Received Data,do somethings.
                            tv_Msg.append("Server:" + data + "\n");
                        }
                    });
                } else {
                    try {   //断开与服务器的连接
                        //connector.disconnect();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }


    class ButtonClickEvent implements View.OnClickListener {
        public void onClick(View v) {
            if (v == btn_Send) {
                //发送数据
                try {
                    new Thread() {
                        @Override
                        public void run() {
                            try {
                                connector.send(edtTxt_Data.getText().toString());
                                Log.e("tag,", "send success");
                            } catch (IOException e) {
                                e.printStackTrace();
                                Log.e("tag,", e.getMessage());
                            }
                            tv_Msg.post(new Runnable() {
                                @Override
                                public void run() {
                                    tv_Msg.append("Client:" + edtTxt_Data.getText().toString() + "\n");
                                }
                            });
                        }
                    }.start();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
