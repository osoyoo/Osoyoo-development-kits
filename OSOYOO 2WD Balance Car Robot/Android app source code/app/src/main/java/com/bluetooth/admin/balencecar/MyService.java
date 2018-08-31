package com.bluetooth.admin.balencecar;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;

import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.IntentFilter;

import android.util.Log;


public class MyService extends Service{
    public static final int ACTION_BLUETOOTH_DISABLED = 1;
    public static final int ACTION_BLUETOOTH_FOUNDED = 2;
    public static final int ACTION_CONNECT_FAIL = 4;
    public static final int ACTION_CONNECTED = 8;
    public boolean threadFlag = true;
    MyThread myThread;
    CommandReceiver cmdReceiver;//继承自BroadcastReceiver对象，用于得到Activity发送过来的命令

    /**************service 命令*********/
    static final int CMD_STOP_SERVICE = 0x01;       // Main -> service
    static final int CMD_SEND_DATA = 0x02;          // Main -> service
    static final int CMD_SYSTEM_EXIT =0x03;         // service -> Main
    static final int CMD_SHOW_TOAST =0x04;          // service -> Main
    static final int CMD_CONNECT_BLUETOOTH = 0x05;  // Main -> service
    static final int CMD_RECEIVE_DATA = 0x06;       //service -> Main

    private BluetoothAdapter mBluetoothAdapter = null;
    private BluetoothSocket btSocket = null;
    private OutputStream outStream = null;
    private InputStream  inStream = null;
    public  boolean bluetoothFlag  = true;
    private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private static String address = "20:16:05:25:07:95"; // <==要连接的蓝牙设备MAC地址


    @Override
    public IBinder onBind(Intent intent) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public void onCreate() {
        // TODO Auto-generated method stub
        super.onCreate();

    }



    //前台Activity调用startService时，该方法自动执行
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        // TODO Auto-generated method stub

        address = intent.getStringExtra("Mac");  //获取主线程发送来的蓝牙地址

        cmdReceiver = new CommandReceiver();
        IntentFilter filter = new IntentFilter();//创建IntentFilter对象
        //注册一个广播，用于接收Activity传送过来的命令，控制Service的行为，如：发送数据，停止服务等
        filter.addAction("android.intent.action.cmd");
        //注册Broadcast Receiver
        registerReceiver(cmdReceiver, filter);
        doJob();//调用方法启动线程
        return super.onStartCommand(intent, flags, startId);

    }



    @Override
    public void onDestroy() {
        // TODO Auto-generated method stub
        super.onDestroy();
        this.unregisterReceiver(cmdReceiver);//取消注册的CommandReceiver
        threadFlag = false;
        boolean retry = true;
        while(retry){
            try{
                myThread.join();
                retry = false;
            }catch(Exception e){
                e.printStackTrace();
            }

        }
    }

    public class MyThread extends Thread{
        boolean start = false;
        String strMsg = null;
        int len = 0;
        @Override
        public void run() {
            // TODO Auto-generated method stub
            super.run();
            connectDevice();//连接蓝牙设备
            while(threadFlag){
                try {
                    byte[] buff = new byte[1];
                    len = inStream.read(buff);
                    //把读取到的数据发送给UI进行显示
                    String strBuffer = new String(buff);

                    if(strBuffer.equals("$"))
                    {
                        start = true;
                        strMsg = "";
                    }

                    if (start == true)
                    {
                        strMsg+=strBuffer;
                    }
                    if(strBuffer.equals("#"))
                    {
                        start = false;
                        SendMainActiveBlutoothData(strMsg);
                    }
                    buff = null;

                } catch (Exception e) {
                    Log.e("MyThread", "Read" );
                    Log.e("MyThread", e.getMessage() );
                    break;
                }

                try{
                    Thread.sleep(5);
                }catch(Exception e){
                    e.printStackTrace();
                }
            }

        }
    }

    public void doJob(){
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        if (mBluetoothAdapter == null) {
            DisplayToast("蓝牙设备不可用，请打开蓝牙！");
            bluetoothFlag  = false;
            return;
        }

        if (!mBluetoothAdapter.isEnabled()) {
            DisplayToast("请打开蓝牙并重新运行程序！");
            bluetoothFlag  = false;
            stopService();
            showToast(getString(R.string.message_07), ACTION_BLUETOOTH_DISABLED);
            return;
        }
        showToast(getString(R.string.message_08), ACTION_BLUETOOTH_FOUNDED);
        threadFlag = true;
        myThread = new MyThread();
        myThread.start();

    }
    public  void connectDevice(){
        DisplayToast("正在尝试连接蓝牙设备，请稍后····");
        BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
        try {
            btSocket = device.createRfcommSocketToServiceRecord(MY_UUID);
        } catch (Exception e) {
            DisplayToast("套接字创建失败！");
            bluetoothFlag = false;
        }
        DisplayToast("成功连接蓝牙设备！");
        mBluetoothAdapter.cancelDiscovery();
        try {
            btSocket.connect();
            DisplayToast("连接成功建立，可以开始操控了!");
            showToast(getString(R.string.message_09), ACTION_CONNECTED);
            bluetoothFlag = true;
        } catch (Exception e) {
            showToast(getString(R.string.message_10), ACTION_CONNECT_FAIL);
            try {
                btSocket.close();
                bluetoothFlag = false;
                Log.e("connectDevice：连接失败", e.getMessage());
            } catch (Exception e2) {
                DisplayToast("连接没有建立，无法关闭套接字！");
            }
        }

        if(bluetoothFlag){
            try {
                inStream = btSocket.getInputStream();
                Log.e("connectDevice", "获取输入");
            } catch (Exception e) {
                e.printStackTrace();
                Log.e("connectDevice", e.getMessage());
            } //绑定读接口

            try {
                outStream = btSocket.getOutputStream();
                Log.e("connectDevice", "获取写成功");
            } catch (Exception e) {
                e.printStackTrace();
                Log.e("connectDevice", e.getMessage());
            } //绑定写接口

        }
    }

    public void sendCmd(byte cmd, String value)//串口发送数据
    {
        if(!bluetoothFlag){
            Log.e("Service sendCmd","bluetoothFlag false");
            return;
        }

        try {
            outStream.write(value.getBytes());
            outStream.flush();
            Log.e("Service sendCmd","ok");
        } catch (Exception e) {
            Log.e("Service sendCmd","Write err");
            Log.e("Service sendCmd",e.getMessage());
            //e.printStackTrace();
        }
    }

    public int SendMainActiveBlutoothData(String str){//return -1 if no data
        Intent intent = new Intent();
        intent.putExtra("cmd", CMD_RECEIVE_DATA);
        intent.putExtra("str", str);
        intent.setAction("android.intent.action.bluetooth.admin.bluetooth");
        sendBroadcast(intent);
        return 0;
    }

    public void stopService(){//停止服务
        threadFlag = false;//停止线程
        try {
            btSocket.close();
            inStream.close();
            outStream.close();

        }catch(Exception e)
        {
            Log.e("Service stopService","Close socket error!");
            Log.e("Service stopService",e.getMessage());
        }

        stopSelf();//停止服务
    }

    public void showToast(String str, int action){//显示提示信息
        Intent intent = new Intent();
        intent.putExtra("cmd", CMD_SHOW_TOAST);
        intent.putExtra("str", str);
        intent.putExtra("action", action);
        intent.setAction("android.intent.action.bluetooth.admin.bluetooth");
        sendBroadcast(intent);
    }

    public void DisplayToast(String str)
    {
        Log.d("Season",str);
    }

    //接收Activity传送过来的命令
    private class CommandReceiver extends BroadcastReceiver{
        @Override
        public void onReceive(Context context, Intent intent) {
            if(intent.getAction().equals("android.intent.action.cmd")){
                int cmd = intent.getIntExtra("cmd", -1);//获取Extra信息
                if(cmd == CMD_STOP_SERVICE){
                    stopService();
                    Log.i("CMD_STOP_SERVICE", "Stop service");
                }

                if(cmd == CMD_SEND_DATA)
                {
                    byte command = intent.getByteExtra("command", (byte) 0);
                    String value =  intent.getStringExtra("value");
                    Log.i("CMD_SEND_DATA", value);
                    sendCmd(command,value);
                }
                if (cmd == CMD_CONNECT_BLUETOOTH)
                {
                    //address =  intent.getStringExtra("Mac");
                    //Log.i("CMD_CONNECT_BLUETOOTH", address);
                    //doJob();
                }
            }
        }
    }



}


