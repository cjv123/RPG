/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.zfteam.rmxprunner;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import com.google.ads.Ad;
import com.google.ads.AdListener;
import com.google.ads.AdRequest;
import com.google.ads.AdRequest.ErrorCode;
import com.google.ads.AdSize;
import com.google.ads.AdView;
import com.zfteam.rmxprunner.tonear.R;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.widget.RelativeLayout;

public class RMXPrunner extends Cocos2dxActivity implements AdListener{
	
	private static RMXPrunner mRPGActivity;
	private static AdView mAdview;
	private static RelativeLayout mRl;
	private static String MY_AD_UNIT_ID = "a1534429a3b08c2";
	private static MyHandler mHandler;
	
	public static native void initJVM();
	
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
		
		initJVM();
		mRPGActivity = this;
		
		Looper curLooper = Looper.myLooper (); 
        Looper mainLooper = Looper.getMainLooper (); 
        if (curLooper== null ){ 
               mHandler = new MyHandler(mainLooper); 
        } else { 
               mHandler = new MyHandler(curLooper); 
        } 
		
		// Create the adView
		mAdview = new AdView(this, AdSize.BANNER, MY_AD_UNIT_ID);
		mRl = new RelativeLayout(this);
	    mAdview.loadAd(new AdRequest());
	    mRl.setGravity(Gravity.BOTTOM | Gravity.CENTER_HORIZONTAL);
	    mRl.bringToFront();
		mAdview.setId(5);
		LayoutParams params = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
		mRPGActivity.addContentView(mRl, params);
		mRl.addView(mAdview);
		mAdview.setVisibility(View.VISIBLE);
		mAdview.setAdListener(this);
	}

    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// RMXP_runner should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }

    static {
        System.loadLibrary("cocos2dcpp");
    }
    
    public static void setAdViewVisable(int showflag)
	{
    	Message m = mHandler.obtainMessage(1, showflag, 1,"");
		mHandler.sendMessage(m); 
	}
    
	public static void onClickBackButton()
	{
		Message m = mHandler.obtainMessage(1, 2, 1,"");
		mHandler.sendMessage(m); 
	}

	@Override
	public void onDismissScreen(Ad arg0) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void onFailedToReceiveAd(Ad arg0, ErrorCode arg1) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void onLeaveApplication(Ad arg0) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void onPresentScreen(Ad arg0) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void onReceiveAd(Ad arg0) {
		// TODO Auto-generated method stub
		
	}     
	
	
	private class MyHandler extends Handler{              

        public MyHandler(Looper looper){ 
               super (looper); 
        } 

        @Override 
        public void handleMessage(Message msg) { // 处理消息 
        	
        	if (msg.arg1 == 0)
        	{
        		mAdview.setVisibility(View.INVISIBLE);
        	}
        	else if(msg.arg1 == 1)
        	{
        		mAdview.setVisibility(View.VISIBLE);
        	}
        	else if(msg.arg1 == 2)
        	{
        		new AlertDialog.Builder(mRPGActivity)
        		.setTitle(R.string.app_name)
        		.setMessage("退出游戏?")
        		.setPositiveButton(android.R.string.ok,
        				new OnClickListener() {
        					public void onClick(DialogInterface dialog,
        							int which) {
        						dialog.dismiss();
        						android.os.Process.killProcess(android.os.Process.myPid());
        					}
        				})
        		.setNegativeButton(android.R.string.cancel,null).
        		create().show();
        	}
    		
        }             

	} 
}
