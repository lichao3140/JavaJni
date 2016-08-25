package com.lichao.javajni;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

/**
 * JNI练习
 * java 调用 C\C++
 * C\C++访问java的成员
 * @author dell
 *
 */
public class MainActivity extends Activity {
	
	private TextView tView1,tView2,tView3,tView4,tView5,tView6;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        tView1 = (TextView)findViewById(R.id.tv_1);
        tView2 = (TextView)findViewById(R.id.tv_2);
        tView3 = (TextView)findViewById(R.id.tv_3);
        tView4 = (TextView)findViewById(R.id.tv_4);
        tView5 = (TextView)findViewById(R.id.tv_5);
        tView6 = (TextView)findViewById(R.id.tv_6);
        
        //1、静态native方法
        String text1 = JavaJni.getStringFromC();
        tView1.setText(text1);
        
        //2、实例对象非静态的方法
        JavaJni javaJni = new JavaJni();
        String text2 = javaJni.getString2FromC();
        tView2.setText(text2);
        
        //3、访问java非静态属性
        System.out.println("修改key前："+javaJni.key);
        String text3 = javaJni.accessField();
        tView3.setText(text3);
        System.out.println("修改key后："+javaJni.key);
        
        //4、访问java静态属性
        System.out.println("修改count前："+JavaJni.count);
        int text4 = javaJni.accessstaticField();
        tView4.setText(String.valueOf(text4));
        System.out.println("修改count后："+JavaJni.count);
        
        //5、访问java方法
        javaJni.accessMethod();
        int text5 = javaJni.accessstaticField();
        tView5.setText(String.valueOf(text5));
        System.out.println("JNI 调用了 genRandomInt函数");
        
        //6、访问java静态方法
        String text6 =javaJni.accessStaticMethod();
        tView6.setText(text6);
        System.out.println("JNI 调用了 getUUID函数");
        
        //7、访问构造方法
        javaJni.accessConstructor();
        System.out.println("JNI 调用了Date构造函数");
        
        //8、调用父类方法----------此方法还存在bug，待修改！
        //javaJni.accessNonvirtualMethod();
        //System.out.println("JNI 调用了Human的父类方法");
        
        //9、中文问题
        System.out.println(javaJni.chineseChars("中国人"));
        System.out.println("JNI 调用了中文问题传人传出");
        
        //10、数组传入   进行排序处理
        int[] array = {9,100,10,37,5,10};
        javaJni.giveArray(array);
        for(int i: array){
        	System.out.println(i);
        }
        System.out.println("JNI 调用了数组传入进行排序处理");
        
        //11、返回数组
        int[] array2 = javaJni.getArray(10);
        System.out.println("----------");
        for(int i: array2){
        	System.out.println(i);
        }
        System.out.println("JNI 调用了返回数组");
        
        //13、全部引用
        javaJni.createGlobalRef();
        System.out.println(javaJni.getGlobalRef());
        javaJni.deleteGlobalRef();
        System.out.println("全局引用的  创建  获取 释放");
        
        //14、异常处理
        try {
			javaJni.exeception();
		} catch (Throwable e) {
			//这是一个ERROR，所以不能用Exception捕获异常，用ERROR、Throwable可以
			System.out.println("发生异常:"+e.getMessage());
		}
        System.out.println("--------异常发生之后-------");
        try {
			javaJni.exeception();
		} catch (Exception e) {
			System.out.println(e.getMessage());
		}

        //15、缓存策略
		//不断调用cached方法
		for (int i = 0; i < 100; i++) {
			javaJni.cached();
		}
		System.out.println("缓存策略");
    }
    
    static{
    	System.loadLibrary("JavaJni");
    	//16、初始化全局变量
    	JavaJni.initIds();
    	System.out.println("初始化全局变量  genRandomInt函数");
    }
    
    
}
