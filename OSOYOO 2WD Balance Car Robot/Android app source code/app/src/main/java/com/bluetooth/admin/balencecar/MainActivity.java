package com.bluetooth.admin.balencecar;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.HapticFeedbackConstants;
import android.view.LayoutInflater;
import android.view.SoundEffectConstants;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.ListView;
import android.widget.PopupWindow;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import java.lang.ref.SoftReference;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.Set;


public class MainActivity extends BaseActivity {
    private static final String KEY_AUTO_UPDATE_PID = "auto_update_pid";
    private static boolean booleanConnect = false;
    private BluetoothAdapter bluetoothAdapter;
    private DeviceAdapter adapter;
    private TextView bluetoothDevice;
    private Button bluetoothConnect;
    private View[] contentViews;
    private View[] menus;
    private DirectionPadView directionPad;
    private TextView leftMotor;
    private TextView rightMotor;

    /**************service 命令*********/
    static final int CMD_STOP_SERVICE = 0x01;       // Main -> service
    static final int CMD_SEND_DATA = 0x02;          // Main -> service
    static final int CMD_SYSTEM_EXIT = 0x03;         // service -> Main
    static final int CMD_SHOW_TOAST = 0x04;          // service -> Main
    static final int CMD_CONNECT_BLUETOOTH = 0x05;  // Main -> service
    static final int CMD_RECEIVE_DATA = 0x06;       // service -> Main

    MyReceiver receiver;

    private SharedPreferences prefs;

    private boolean isAutoUpdatePID() {
        return prefs.getBoolean(KEY_AUTO_UPDATE_PID, false);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        prefs = getSharedPreferences("my_prefs", MODE_PRIVATE);

        /*按钮监听按下弹起*/
        directionPad = findViewById(R.id.directionPad);
        directionPad.setOnKeyListener(new DirectionPadListener());

        leftMotor = findViewById(R.id.leftMotor);
        rightMotor = findViewById(R.id.rightMotor);

        KeyButton levoButton = findViewById(R.id.levo);
        levoButton.setOnActionListener(new KeyButton.OnActionListener() {
            @Override
            public void onKeyDown(KeyButton v) {
                v.performHapticFeedback(HapticFeedbackConstants.LONG_PRESS, HapticFeedbackConstants.FLAG_IGNORE_GLOBAL_SETTING);
                v.playSoundEffect(SoundEffectConstants.CLICK);
                SendBlueToothProtocol("$0,1,0,0,0,0,0,0,0,0#");
            }

            @Override
            public void onKeyUp(KeyButton v) {
                if (directionPad.isUpKey()) {
                    SendBlueToothProtocol("$1,0,0,0,0,0,0,0,0,0#");
                } else if (directionPad.isDownKey()) {
                    SendBlueToothProtocol("$2,0,0,0,0,0,0,0,0,0#");
                } else if (directionPad.isLeftKey()) {
                    SendBlueToothProtocol("$3,0,0,0,0,0,0,0,0,0#");
                } else if (directionPad.isRightKey()) {
                    SendBlueToothProtocol("$4,0,0,0,0,0,0,0,0,0#");
                } else {
                    SendBlueToothProtocol("$0,0,0,0,0,0,0,0,0,0#");
                }
            }
        });

        KeyButton dextroButton = findViewById(R.id.dextro);
        dextroButton.setOnActionListener(new KeyButton.OnActionListener() {
            @Override
            public void onKeyDown(KeyButton v) {
                v.performHapticFeedback(HapticFeedbackConstants.LONG_PRESS, HapticFeedbackConstants.FLAG_IGNORE_GLOBAL_SETTING);
                v.playSoundEffect(SoundEffectConstants.CLICK);
                SendBlueToothProtocol("$0,2,0,0,0,0,0,0,0,0#");
            }

            @Override
            public void onKeyUp(KeyButton v) {
                if (directionPad.isUpKey()) {
                    SendBlueToothProtocol("$1,0,0,0,0,0,0,0,0,0#");
                } else if (directionPad.isDownKey()) {
                    SendBlueToothProtocol("$2,0,0,0,0,0,0,0,0,0#");
                } else if (directionPad.isLeftKey()) {
                    SendBlueToothProtocol("$3,0,0,0,0,0,0,0,0,0#");
                } else if (directionPad.isRightKey()) {
                    SendBlueToothProtocol("$4,0,0,0,0,0,0,0,0,0#");
                } else {
                    SendBlueToothProtocol("$0,0,0,0,0,0,0,0,0,0#");
                }
            }
        });
        /*按钮监听按下弹起*/


        /*检查手机是否支持蓝牙*/
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bluetoothAdapter == null) {
            //表明此手机不支持蓝牙
            Toast.makeText(MainActivity.this, R.string.message_01, Toast.LENGTH_SHORT).show();
        }

        /*添加蓝牙列表*/
        bluetoothDevice = findViewById(R.id.device);
        adapter = new DeviceAdapter(bluetoothDevice);
        bluetoothDevice.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (v.isActivated()) {
                    v.setActivated(false);
                    hideDeviceList();
                } else {
                    v.setActivated(true);
                    showDeviceList();
                }
            }
        });

        /*蓝牙总开关*/
        Switch bluetoothSwitch = findViewById(R.id.bluetooth);
        bluetoothSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if (b) {
                    if (!bluetoothAdapter.isEnabled()) { //蓝牙未开启，则开启蓝牙
                        Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                        startActivity(enableIntent);
                    } else {
                        Toast.makeText(MainActivity.this, R.string.message_02, Toast.LENGTH_SHORT).show();
                    }
                } else {
                    bluetoothAdapter.disable();
                    Toast.makeText(MainActivity.this, R.string.message_03, Toast.LENGTH_SHORT).show();
                }
            }
        });


        /*蓝牙搜索*/
        Button bluetoothSearch = findViewById(R.id.search);
        bluetoothSearch.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (bluetoothAdapter == null) {
                    Toast.makeText(MainActivity.this, R.string.message_01, Toast.LENGTH_SHORT).show();
                    return;
                } else if (!bluetoothAdapter.isEnabled()) {
                    Toast.makeText(MainActivity.this, R.string.message_04, Toast.LENGTH_SHORT).show();
                }

                Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();
                if (pairedDevices.size() > 0) {
                    for (BluetoothDevice device : pairedDevices) {
                        adapter.remove(device.getAddress());
                        adapter.add(device.getAddress());
                    }
                } else {
                    //注册，当一个设备被发现时调用mReceive
                    Log.i("seach", "hhhhhh");
                    IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
                    registerReceiver(mReceiver, filter);
                }
            }
        });

        /*蓝牙连接或断开*/
        bluetoothConnect = findViewById(R.id.connect);
        bluetoothConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String strMacAddress = adapter.getSelectedItem();
                if (strMacAddress == null) {
                    Toast.makeText(MainActivity.this, R.string.message_05, Toast.LENGTH_SHORT).show();
                } else {
                    if (!booleanConnect) {
                        Intent i = new Intent(MainActivity.this, MyService.class);
                        i.putExtra("Mac", strMacAddress);
                        startService(i);

                        bluetoothConnect.setEnabled(false);
                    } else {// 断开蓝牙
                        booleanConnect = false;
                        //stopService(new Intent(MainActivity.this, MyService.class));
                        bluetoothConnect.setText(R.string.connect);
                        setPIDEnabled(false);

                        Intent intent = new Intent();//创建Intent对象
                        intent.setAction("android.intent.action.cmd");
                        intent.putExtra("cmd", CMD_STOP_SERVICE);
                        sendBroadcast(intent);//发送广播连接蓝牙
                    }
                }
            }
        });

        if (bluetoothAdapter == null) {
            bluetoothSwitch.setEnabled(false);
            bluetoothSearch.setEnabled(false);
            bluetoothDevice.setEnabled(false);
            bluetoothConnect.setEnabled(false);
        } else  if (bluetoothAdapter.isEnabled()) {
            bluetoothSwitch.setChecked(true);
        }

        receiver = new MyReceiver();
        IntentFilter filter = new IntentFilter();
        filter.addAction("android.intent.action.bluetooth.admin.bluetooth");
        MainActivity.this.registerReceiver(receiver, filter);

        menus = new View[3];
        menus[0] = findViewById(R.id.main);
        menus[1] = findViewById(R.id.debug);
        menus[2] = findViewById(R.id.about);
        contentViews = new View[3];
        contentViews[0] = findViewById(R.id.mainContent);
        contentViews[1] = findViewById(R.id.debugContent);
        contentViews[2] = findViewById(R.id.aboutContent);
        initPID(contentViews[1]);

        final View.OnClickListener listener = new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!v.isActivated()) {
                    setMenuActivated(v);
                }
            }
        };

        for (View menu : menus) {
            menu.setOnClickListener(listener);
        }

        setMenuActivated(menus[0]);
    }

    private void updateMotor(String lv, String rv) {
        leftMotor.setText(lv);
        rightMotor.setText(rv);
    }

    private static final float PID_ANGLE_STEP = 0.01f;
    private static final float PID_SPEED_STEP = 0.01f;
    private static final float MAX_AP = 100f;
    private static final float MAX_AD = 100f;
    private static final float MAX_VP = 100f;
    private static final float MAX_VI = 100f;
    private Button[] pidButtons;
    private TextView textAP;
    private TextView textAD;
    private TextView textVP;
    private TextView textVI;
    private SeekBar seekAP;
    private SeekBar seekAD;
    private SeekBar seekVP;
    private SeekBar seekVI;

    private void setPIDEnabled(boolean enabled) {
        textAP.setEnabled(enabled);
        textAD.setEnabled(enabled);
        textVP.setEnabled(enabled);
        textVI.setEnabled(enabled);
        seekAP.setEnabled(enabled);
        seekAD.setEnabled(enabled);
        seekVP.setEnabled(enabled);
        seekVI.setEnabled(enabled);
        for (Button button : pidButtons) {
            button.setEnabled(enabled);
        }
    }

    private void updateAPD(String ap, String ad) {
        if (canUpdateUI()) {
            textAP.setText("P:(" + ap + ")");
            textAD.setText("D:(" + ad + ")");
            seekAP.setProgress(Math.round(Float.valueOf(ap) * 100));
            seekAD.setProgress(Math.round(Float.valueOf(ad) * 100));
        }
    }

    private void updateVPI(String vp, String vi) {
        if (canUpdateUI()) {
            textVP.setText("P:(" + vp + ")");
            textVI.setText("I:(" + vi + ")");
            seekVP.setProgress(Math.round(Float.valueOf(vp) * 100));
            seekVI.setProgress(Math.round(Float.valueOf(vi) * 100));
        }
    }

    /**
     * 查询PID
     */
    private void queryPID() {
        if (booleanConnect) {
            SendBlueToothProtocol("$0,0,1,0,0,0,0,0,0,0#");
        }
    }

    /**
     * 重置PID
     */
    private void resetPID() {
        if (booleanConnect) {
            SendBlueToothProtocol("$0,0,2,0,0,0,0,0,0,0#");

            new Handler().postDelayed(new Runnable() {
                @Override
                public void run() {
                    queryPID();
                }
            }, 500);
        }
    }

    private void initPID(View root) {
        textAP = root.findViewById(R.id.textAP);
        textAD = root.findViewById(R.id.textAD);
        textVP = root.findViewById(R.id.textVP);
        textVI = root.findViewById(R.id.textVI);

        pidButtons = new Button[10];
        pidButtons[0] = root.findViewById(R.id.minusAP);
        pidButtons[0].setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                addAP(-PID_ANGLE_STEP, true);
            }
        });
        pidButtons[1] = root.findViewById(R.id.plusAP);
        pidButtons[1].setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                addAP(PID_ANGLE_STEP, true);
            }
        });

        pidButtons[2] = root.findViewById(R.id.minusAD);
        pidButtons[2].setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                addAD(-PID_ANGLE_STEP, true);
            }
        });
        pidButtons[3] = root.findViewById(R.id.plusAD);
        pidButtons[3].setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                addAD(PID_ANGLE_STEP, true);
            }
        });

        //////////////////
        pidButtons[4] = root.findViewById(R.id.minusVP);
        pidButtons[4].setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                addVP(-PID_SPEED_STEP, true);
            }
        });
        pidButtons[5] = root.findViewById(R.id.plusVP);
        pidButtons[5].setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                addVP(PID_SPEED_STEP, true);
            }
        });

        pidButtons[6] = root.findViewById(R.id.minusVI);
        pidButtons[6].setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                addVI(-PID_SPEED_STEP, true);
            }
        });
        pidButtons[7] = root.findViewById(R.id.plusVI);
        pidButtons[7].setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                addVI(PID_SPEED_STEP, true);
            }
        });

        pidButtons[8] = root.findViewById(R.id.updateAngle);
        pidButtons[8].setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateAngle();
            }
        });
        pidButtons[9] = root.findViewById(R.id.updateSpeed);
        pidButtons[9].setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateSpeed();
            }
        });

        seekAP = root.findViewById(R.id.seekAP);
        seekAP.setMax(Math.round(MAX_AP * 100));
        seekAP.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if (fromUser) {
                    float value = progress / 100f;
                    addAP(value - parseValue(textAP.getText().toString()), isAutoUpdatePID());
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        seekAD = root.findViewById(R.id.seekAD);
        seekAD.setMax(Math.round(MAX_AD * 100));
        seekAD.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if (fromUser) {
                    float value = progress / 100f;
                    addAD(value - parseValue(textAD.getText().toString()), isAutoUpdatePID());
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        seekVP = root.findViewById(R.id.seekVP);
        seekVP.setMax(Math.round(MAX_VP * 100));
        seekVP.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if (fromUser) {
                    float value = progress / 100f;
                    addVP(value - parseValue(textVP.getText().toString()), isAutoUpdatePID());
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        seekVI = root.findViewById(R.id.seekVI);
        seekVI.setMax(Math.round(MAX_VI * 100));
        seekVI.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if (fromUser) {
                    float value = progress / 100f;
                    addVI(value - parseValue(textVI.getText().toString()), isAutoUpdatePID());
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        View queryPID = root.findViewById(R.id.queryPID);
        queryPID.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                queryPID();
            }
        });

        View resetPID = root.findViewById(R.id.resetPID);
        resetPID.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                resetPID();
            }
        });

        CheckBox checkBox = root.findViewById(R.id.autoUpdatePID);
        checkBox.setChecked(isAutoUpdatePID());
        checkBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                prefs.edit().putBoolean(KEY_AUTO_UPDATE_PID, isChecked).apply();
            }
        });

        updateAPD("0.00", "0.00");
        updateVPI("0.00", "0.00");
    }

    private float parseValue(String s) {
        int index = s.indexOf("(");
        int end = s.lastIndexOf(")");
        if (index != -1 && end != -1 && end > index) {
            try {
                return Float.parseFloat(s.substring(index + 1, end));
            } catch (NumberFormatException e) {
                e.printStackTrace();
            }
        }

        return 0f;
    }

    private void addAP(float v, boolean sync) {
        float ap = Math.max(0f, Math.min(parseValue(textAP.getText().toString()) + v, MAX_AP));
        float ad = parseValue(textAD.getText().toString());
        String s1 = String.format(Locale.US, "%.2f", ap);
        String s2 = String.format(Locale.US, "%.2f", ad);
        updateAPD(s1, s2);

        if (sync) {
            String cmd = String.format("$0,0,0,0,1,0,AP%s,AD%s,VP0,VI0#", s1, s2);
            SendBlueToothProtocol(cmd);
        }
    }

    private void addAD(float v, boolean sync) {
        float ap = parseValue(textAP.getText().toString());
        float ad = Math.max(0f, Math.min(parseValue(textAD.getText().toString()) + v, MAX_AD));
        String s1 = String.format(Locale.US, "%.2f", ap);
        String s2 = String.format(Locale.US, "%.2f", ad);
        updateAPD(s1, s2);

        if (sync) {
            String cmd = String.format("$0,0,0,0,1,0,AP%s,AD%s,VP0,VI0#", s1, s2);
            SendBlueToothProtocol(cmd);
        }
    }

    private void addVP(float v, boolean sync) {
        float vp = Math.max(0f, Math.min(parseValue(textVP.getText().toString()) + v, MAX_VP));
        float vi = parseValue(textVI.getText().toString());
        String s1 = String.format(Locale.US, "%.2f", vp);
        String s2 = String.format(Locale.US, "%.2f", vi);
        updateVPI(s1, s2);

        if (sync) {
            String cmd = String.format("$0,0,0,0,0,1,AP0,AD0,VP%s,VI%s#", s1, s2);
            SendBlueToothProtocol(cmd);
        }
    }

    private void addVI(float v, boolean sync) {
        float vp = parseValue(textVP.getText().toString());
        float vi = Math.max(0f, Math.min(parseValue(textVI.getText().toString()) + v, MAX_VI));
        String s1 = String.format(Locale.US, "%.2f", vp);
        String s2 = String.format(Locale.US, "%.2f", vi);
        updateVPI(s1, s2);

        if (sync) {
            String cmd = String.format("$0,0,0,0,0,1,AP0,AD0,VP%s,VI%s#", s1, s2);
            SendBlueToothProtocol(cmd);
        }
    }

    private void updateAngle() {
        float ap = parseValue(textAP.getText().toString());
        float ad = parseValue(textAD.getText().toString());
        String s1 = String.format(Locale.US, "%.2f", ap);
        String s2 = String.format(Locale.US, "%.2f", ad);
        updateAPD(s1, s2);
        String cmd = String.format("$0,0,0,0,1,0,AP%s,AD%s,VP0,VI0#", s1, s2);
        SendBlueToothProtocol(cmd);
    }

    private void updateSpeed() {
        float vp = parseValue(textVP.getText().toString());
        float vi = parseValue(textVI.getText().toString());
        String s1 = String.format(Locale.US, "%.2f", vp);
        String s2 = String.format(Locale.US, "%.2f", vi);
        updateVPI(s1, s2);
        String cmd = String.format("$0,0,0,0,0,1,AP0,AD0,VP%s,VI%s#", s1, s2);
        SendBlueToothProtocol(cmd);
    }

    @Override
    protected void onPause() {
        super.onPause();
        stopUpdateMotor();
    }

    @Override
    protected void onResume() {
        super.onResume();
        setPIDEnabled(false);
        queryPID();
        if (menus[0].isActivated()) {
            startUpdateMotor();
        }
    }

    private void forceDisconnect() {
        if (booleanConnect) {
            // 断开连接
            Intent intent = new Intent();//创建Intent对象
            intent.setAction("android.intent.action.cmd");
            intent.putExtra("cmd", CMD_STOP_SERVICE);
            sendBroadcast(intent);//发送广播连接蓝牙
            booleanConnect = false;
        }
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
        forceDisconnect();
    }

    @Override
    protected void onDestroy() {
        // TODO Auto-generated method stub
        super.onDestroy();
        MainActivity.this.unregisterReceiver(receiver);
        forceDisconnect();
    }

    private void setMenuActivated(View view) {
        for (int i = 0; i < menus.length; i++) {
            if (menus[i].getId() == view.getId()) {
                menus[i].setActivated(true);
                contentViews[i].setVisibility(View.VISIBLE);
            } else {
                menus[i].setActivated(false);
                contentViews[i].setVisibility(View.GONE);
            }
        }

        if (menus[0].isActivated()) {
            startUpdateMotor();
        } else {
            stopUpdateMotor();
        }
    }

    private PopupWindow window;

    private void hideDeviceList() {
        bluetoothDevice.setActivated(false);
        if (window != null && window.isShowing()) {
            window.dismiss();
        }
    }

    private void showDeviceList() {
        bluetoothDevice.setActivated(true);
        int width = Math.round(getResources().getDisplayMetrics().density * 200);
        ListView listView = new ListView(this);
        listView.setAdapter(adapter);
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                adapter.setSelectedItem(adapter.getItem(position));
                hideDeviceList();
            }
        });
        window = new PopupWindow(listView, width, WindowManager.LayoutParams.WRAP_CONTENT, true);
        window.setBackgroundDrawable(new ColorDrawable(Color.WHITE));
        window.setOutsideTouchable(false);
        window.setTouchable(true);
        window.showAsDropDown(bluetoothDevice, 0, 0);
        window.setOnDismissListener(new PopupWindow.OnDismissListener() {
            @Override
            public void onDismiss() {
                bluetoothDevice.setActivated(false);
            }
        });
    }

    private BroadcastReceiver mReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            Log.e("fond:", "mReceiver");

            String action = intent.getAction();
            if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                // 已经配对的则跳过
                if (device.getBondState() != BluetoothDevice.BOND_BONDED) {
                    adapter.add(device.getAddress());
                }

            } else if (BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action)) {  //搜索结束
                Log.e("fond:", "ACTION_DISCOVERY_FINISHED");
                if (adapter.getCount() == 0) {
                    Toast.makeText(MainActivity.this, R.string.message_06, Toast.LENGTH_SHORT).show();
                }
            }

        }
    };

    private void startUpdateMotor() {
        if (booleanConnect) {
            SendBlueToothProtocol("$0,0,0,1,0,0,0,0,0,0#");
        }
    }

    private void stopUpdateMotor() {
        if (booleanConnect) {
            SendBlueToothProtocol("$0,0,0,2,0,0,0,0,0,0#");
        }
    }
    /*********************************************************************************************/

    public void showToast(String str) {//显示提示信息
        Toast.makeText(getApplicationContext(), str, Toast.LENGTH_SHORT).show();
    }

    public class MyReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            // TODO Auto-generated method stub
            if (intent.getAction().equals("android.intent.action.bluetooth.admin.bluetooth")) {
                Bundle bundle = intent.getExtras();
                int cmd = bundle.getInt("cmd");

                if (cmd == CMD_SHOW_TOAST) {
                    String str = bundle.getString("str");
                    showToast(str);

                    int action = bundle.getInt("action");
                    if (MyService.ACTION_CONNECTED == action) {
                        booleanConnect = true;
                        bluetoothConnect.setEnabled(true);
                        bluetoothConnect.setText(R.string.disconnect);
                        queryPID();
                        startUpdateMotor();
                    } else if (MyService.ACTION_CONNECT_FAIL == action) {
                        booleanConnect = false;
                        bluetoothConnect.setEnabled(true);
                        bluetoothConnect.setText(R.string.connect);
                    }
                } else if (cmd == CMD_SYSTEM_EXIT) {
                    System.exit(0);
                } else if (cmd == CMD_RECEIVE_DATA)  //此处是可以接收蓝牙发送过来的数据可以解析，此例程暂时不解析返回来的数据，需要解析的在我们的全功能版会有
                {
                    String strtemp = bundle.getString("str") + "";
                    int start = strtemp.indexOf("$");
                    int end = strtemp.indexOf("#");
                    if (start >= 0 && end > 0 && end > start && strtemp.length() > 23) {
                        String str = strtemp.substring(start + 1, end);
                        if (str.contains("VP") && str.contains("VI")) {
                            Log.e("----PID----", str);
                            String ap = "0.00";
                            String ad = "0.00";
                            String vp = "0.00";
                            String vi = "0.00";
                            String[] segs = str.split(",");
                            for (String s : segs) {
                                if (s.startsWith("AP")) {
                                    ap = s.substring(2);
                                } else if (s.startsWith("AD")) {
                                    ad = s.substring(2);
                                } else if (s.startsWith("VP")) {
                                    vp = s.substring(2);
                                } else if (s.startsWith("VI")) {
                                    vi = s.substring(2);
                                }
                            }

                            if (canUpdateUI()) {
                                setPIDEnabled(true);
                                updateAPD(ap, ad);
                                updateVPI(vp, vi);
                            }
                        } else if(str.contains("LV") && str.contains("RV")) {
                            Log.e("****str****", str);
                            String lv = "0.00";
                            String rv = "0.00";
                            String[] segs = str.split(",");
                            for (String s : segs) {
                                if (s.startsWith("LV")) {
                                    lv = s.substring(2);
                                } else if (s.startsWith("RV")) {
                                    rv = s.substring(2);
                                }
                            }

                            if (canUpdateUI()) {
                                updateMotor(lv, rv);
                            }
                        }
                    }
                }

            }
        }
    }

    public void SendBlueToothProtocol(String value) {
        Intent intent = new Intent();//创建Intent对象
        intent.setAction("android.intent.action.cmd");
        intent.putExtra("cmd", CMD_SEND_DATA);
        intent.putExtra("command", (byte) 0x00);
        intent.putExtra("value", value);
        sendBroadcast(intent);//发送广播
    }

    /*********************************************************************************************/


    private class DirectionPadListener implements DirectionPadView.OnKeyListener {
        @Override
        public void onCenterAction(DirectionPadView v) {
            v.performHapticFeedback(HapticFeedbackConstants.LONG_PRESS, HapticFeedbackConstants.FLAG_IGNORE_GLOBAL_SETTING);
            v.playSoundEffect(SoundEffectConstants.CLICK);
            SendBlueToothProtocol("$0,0,0,0,0,0,0,0,0,0#");
        }

        @Override
        public void onUpAction(DirectionPadView v) {
            v.performHapticFeedback(HapticFeedbackConstants.LONG_PRESS, HapticFeedbackConstants.FLAG_IGNORE_GLOBAL_SETTING);
            v.playSoundEffect(SoundEffectConstants.CLICK);
            SendBlueToothProtocol("$1,0,0,0,0,0,0,0,0,0#");
        }

        @Override
        public void onDownAction(DirectionPadView v) {
            v.performHapticFeedback(HapticFeedbackConstants.LONG_PRESS, HapticFeedbackConstants.FLAG_IGNORE_GLOBAL_SETTING);
            v.playSoundEffect(SoundEffectConstants.CLICK);
            SendBlueToothProtocol("$2,0,0,0,0,0,0,0,0,0#");
        }

        @Override
        public void onLeftAction(DirectionPadView v) {
            v.performHapticFeedback(HapticFeedbackConstants.LONG_PRESS, HapticFeedbackConstants.FLAG_IGNORE_GLOBAL_SETTING);
            v.playSoundEffect(SoundEffectConstants.CLICK);
            SendBlueToothProtocol("$3,0,0,0,0,0,0,0,0,0#");
        }

        @Override
        public void onRightAction(DirectionPadView v) {
            v.performHapticFeedback(HapticFeedbackConstants.LONG_PRESS, HapticFeedbackConstants.FLAG_IGNORE_GLOBAL_SETTING);
            v.playSoundEffect(SoundEffectConstants.CLICK);
            SendBlueToothProtocol("$4,0,0,0,0,0,0,0,0,0#");
        }

        @Override
        public void onKeyUp(DirectionPadView v) {
//            if (levoButton.isKeyDown()) {
//                SendBlueToothProtocol("$0,1,0,0,0,0,0,0,0,0#");
//            } else if (dextroButton.isKeyDown()) {
//                SendBlueToothProtocol("$0,2,0,0,0,0,0,0,0,0#");
//            } else {
                SendBlueToothProtocol("$0,0,0,0,0,0,0,0,0,0#");
//            }
        }
    }

    private class DeviceAdapter extends BaseAdapter {
        private final List<String> list = new ArrayList<>();
        private final SoftReference<TextView> sSelectedItemDisplay;
        private String selected;

        DeviceAdapter(TextView selectedItemDisplay) {
            sSelectedItemDisplay = new SoftReference<>(selectedItemDisplay);
        }

        private void display(String content) {
            TextView textView = sSelectedItemDisplay.get();
            if (textView != null) {
                if (content != null) {
                    textView.setText(content);
                } else {
                    textView.setText("");
                }
            }
        }

        private void updateSelectedItem() {
            if (selected == null || !list.contains(selected)) {
                if (list.size() > 0) {
                    selected = list.get(0);
                    display(selected);
                }
            }
        }

        public String getSelectedItem() {
            return selected;
        }

        public void setSelectedItem(String item) {
            selected = item;
            display(item);
        }

        public void add(String item) {
            list.add(item);
            notifyDataSetChanged();
        }

        public void remove(String item) {
            list.remove(item);
            notifyDataSetChanged();
        }

        @Override
        public void notifyDataSetChanged() {
            updateSelectedItem();
            super.notifyDataSetChanged();
        }

        @Override
        public void notifyDataSetInvalidated() {
            updateSelectedItem();
            super.notifyDataSetInvalidated();
        }

        @Override
        public int getCount() {
            return list.size();
        }

        @Override
        public String getItem(int position) {
            return list.get(position);
        }

        @Override
        public long getItemId(int position) {
            return position;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            if (convertView == null) {
                convertView = LayoutInflater.from(parent.getContext()).inflate(R.layout.list_item, null);
            }

            TextView textView = (TextView) convertView;
            String content = getItem(position);
            textView.setText(content);
            textView.setActivated(content.equals(selected));
            return convertView;
        }
    }
}
