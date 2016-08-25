package com.lichao.javajni;

import java.util.Date;
import java.util.Random;
import java.util.UUID;

import android.R.integer;
import android.R.string;

public class JavaJni {
	public String key = "lichao";
	
	public static int count = 9;
	
	//1、静态方法
	public native static String getStringFromC();
	
	//2、非静态方法
	public native String getString2FromC();
	
	//3、访问非静态属性,返回修改之后的修改内容
	public native String accessField();
	
	//4、访问静态属性
	public native int accessstaticField();
	
	//5、访问java方法
	public native int accessMethod();
	//产生指定范围随机数  genRandomInt 给 accessMethod调用
    public int genRandomInt(int max){
    	System.out.println("genRandomInt 执行了...");
    	return new Random().nextInt(max);
    }
	
	//6、访问java静态方法
	public native String accessStaticMethod();
    //产生一个UUID
    public static String getUUID(){
    	return UUID.randomUUID().toString();
    }
    
    //7、访问构造方法
    public native Date accessConstructor();
    
    //8、调用父类的方法
    public Human human = new Man();
    public native void accessNonvirtualMethod();
    
    //9、中文问题
    public native String chineseChars(String in);
    
    //10、传入数组
    public native void giveArray(int[] array);
    
    //11、返回数组
    public native int[] getArray(int len);
    
    //12、局部引用，进行手动释放  模拟循环创建数组
    public native void localRef();
    
    //13、创建全局引用
    public native void createGlobalRef();
    
    //13、获得全局引用
    public native string getGlobalRef(); 
    
    //13、释放全局引用
    public native void deleteGlobalRef();
    
    //14、异常处理
    public native void exeception();
    
    //15、缓存策略
    public native void cached();
    
    //16、初始化全局变量
    public native static void initIds();

}
