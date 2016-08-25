#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jni.h>
#include "com_lichao_javajni_JavaJni.h"

#ifdef ANDROID
#include <jni.h>
#include <android/log.h>
#define LOGE(format, ...)  __android_log_print(ANDROID_LOG_ERROR, "(>_<)", format, ##__VA_ARGS__)
#define LOGI(format, ...)  __android_log_print(ANDROID_LOG_INFO,  "(=_=)", format, ##__VA_ARGS__)
#else
#define LOGE(format, ...)  LOGE("(>_<) " format "\n", ##__VA_ARGS__)
#define LOGI(format, ...)  LOGE("(^_^) " format "\n", ##__VA_ARGS__)
#endif
/**
 * 每个native函数，都至少有两个参数（JNIEnv* ,jclass或jobject）
 * 1、当native为静态方法时：jclass 代表native方法所属类的Class对象（JavaJni.class）
 * 2、当native为非静态方法时：jobject 代表native所属的对象
 */

//1、静态native方法
JNIEXPORT jstring JNICALL Java_com_lichao_javajni_JavaJni_getStringFromC
  (JNIEnv *env, jclass jclas){
	//JNIEnv 结构体指针
	//env 二级指针
	return (*env)->NewStringUTF(env, "static C String");
}

//2、非静态native方法
JNIEXPORT jstring JNICALL Java_com_lichao_javajni_JavaJni_getString2FromC
  (JNIEnv *env, jobject jobj){
	return (*env)->NewStringUTF(env, "void C String2");
}

//3、访问java 非静态属性
//修改属性key
JNIEXPORT jstring JNICALL Java_com_lichao_javajni_JavaJni_accessField
  (JNIEnv *env, jobject jobj){
	//得到jcalss
	jclass cls = (*env)->GetObjectClass(env, jobj);
	//jfieldID
	//属性名称，属性签名
	//booblean  Z  byte  B  char Ｃ　 short  S   int  I  long   L  float  F   double  D  Void  V
	//string  Ljava/lang/String;   Object  Ljava/lang/Object;  int[]  [I   int[][]  [[I
	jfieldID fid = (*env)->GetFieldID(env, cls, "key", "Ljava/lang/String;");
	//lichao >>super lichao
	//获取Key属性的值   Get<Type>Field
	jstring jstr = (*env)->GetObjectField(env, jobj, fid);

	//jstring转成C的字符串
	//isCopy 是否复制由（GetStringUTFChars）函数内部决定
	//jboolean 指针
	jboolean isCopy = NULL;
	//函数内部复制了，isCopy为 JNI_TRUE,没有复制为JNI_FALSE
	char *c_str = (*env)->GetStringUTFChars(env, jstr, &isCopy);
	//意义：isCopy为JNI_FALSE,c_str和jstr都是指向同一个字符串

	//拼接得到新的字符串
	char text[20] = "super";
	strcat(text, c_str);

	//C的字符串转成jstring
	jstring new_jstr = (*env)->NewStringUTF(env, text);
	//修改key  Set<Type>Field
	(*env)->SetObjectField(env, jobj, fid, new_jstr);

	//只要使用了GetStringUTFChars，GetStringChars等一定要释放
	//释放
	(*env)->ReleaseStringUTFChars(env, jstr, c_str);
	return new_jstr;
}

//4、访问java静态属性
JNIEXPORT jint JNICALL Java_com_lichao_javajni_JavaJni_accessstaticField
  (JNIEnv *env, jobject jobj){
	//得到jclass
	jclass cls = (*env)->GetObjectClass(env, jobj);
	//jfieldID
	jfieldID fid = (*env)->GetStaticFieldID(env, cls, "count", "I");
	//GetStatic<Type>Field
	jint count = (*env)->GetStaticIntField(env, cls, fid);
	count++;
	//修改  SetStatic<Type>Field
	(*env)->SetStaticIntField(env, cls, fid, count);
	return count;
}

//5、访问java方法
JNIEXPORT jint JNICALL Java_com_lichao_javajni_JavaJni_accessMethod
  (JNIEnv *env, jobject jobj){
	//得到jclass
	jclass cls = (*env)->GetObjectClass(env, jobj);
	//方法签名 cmd中 E:\android\workpase\JavaJni\bin\classes>javap -s -p com.lichao.javajni.JavaJni
	jmethodID mid = (*env)->GetMethodID(env, cls, "genRandomInt", "(I)I");
	//调用   Call<Type>Method
	jint random = (*env)->CallIntMethod(env, jobj, mid, 200);
	printf("random num:%ld",random);
	return random;
}

//6、访问java静态方法
JNIEXPORT jstring JNICALL Java_com_lichao_javajni_JavaJni_accessStaticMethod
  (JNIEnv *env, jobject jobj){
	//得到jclass
	jclass cls = (*env)->GetObjectClass(env, jobj);
	//方法签名  GetStaticMethodID
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "getUUID", "()Ljava/lang/String;");
	//调用CallStatic<Type>Method
	jstring uuid = (*env)->CallStaticObjectMethod(env, cls, mid);

	//随机文件名称 uuid.txt
	//jstring -> char*
	//isCopy JNI_FALSE代表java和c操作同一字符串
	char *uuid_str = (*env)->GetStringUTFChars(env, uuid, NULL);
	printf("uuid_str :%s",uuid_str);
	//拼接   在android sdcard生成.txt文件  需要在AndroidManifest.xml中添加操作权限。
	FILE *fp = NULL;
	fp = fopen("/sdcard/lichaojni.txt", "a+");
	fprintf(fp, "%s\n", "中文会乱码吗？I am lichao!\n");
	fclose(fp);

	return uuid;
}

//访问构造方法
//7、用Java.util.Date产生一个时间戳
JNIEXPORT jobject JNICALL Java_com_lichao_javajni_JavaJni_accessConstructor
  (JNIEnv *env, jobject jobj){
	jclass cls = (*env)->FindClass(env, "java/util/Date");
	//jmethodID   构造方法要写城这个  有参和无参  <init>  和签名
	jmethodID constructor_mid =(*env)->GetMethodID(env, cls, "<init>", "()V");
	//实例化一个Date对象
	jobject date_obj = (*env)->NewObject(env, cls, constructor_mid);
	//调用getTime方法
	jmethodID mid = (*env)->GetMethodID(env, cls, "getTime", "()J");
	jlong time = (*env)->CallLongMethod(env, date_obj, mid);
	printf("\ntime:%lld\n", time);
	return date_obj;
}

//8、调用父类的方法
JNIEXPORT void JNICALL Java_com_lichao_javajni_JavaJni_accessNonvirtualMethod
  (JNIEnv *env, jobject jobj){
	jclass cls = (*env)->GetObjectClass(env, jobj);
	//获取Man属性对象
	jfieldID fid = (*env)->GetFieldID(env, cls, "human", "Lcom/lichao/javajni/Human;");
	LOGE("--fid--", fid);
	//获取
	jobject human_obj = (*env)->GetObjectField(env, jobj, fid);

	//执行sayHi（）方法
	jclass human_cls = (*env)->FindClass(env, "com/lichao/javajni/Human");//注意:传要调用的父类的名称
	jmethodID mid = (*env)->GetMethodID(env, human_cls, "sayHi", "()V");
	LOGE("--mid--", mid);
	//执行构造方法子类  不覆盖子类方法
	//(*env)->CallObjectMethod(env, human_obj, mid);
	//执行构造方法父类  覆盖子类方法
	(*env)->CallNonvirtualObjectMethod(env, human_obj, human_cls, mid);
}

//9、中文问题
JNIEXPORT jstring JNICALL Java_com_lichao_javajni_JavaJni_chineseChars
  (JNIEnv *env, jobject jobj, jstring in){

	//输出  java 转  c 字符串
	//char *c_str = (*env)->GetStringUTFChars(env, in, NULL);
	//printf("%s\n",c_str);

	//返回 c 转  jstring
	char *c_str = "说好的幸福呢";
	//执行String(byte bytes[], String charsetName)构造方法,用java方法解决乱码问题，需要的条件
	//1.jmethodID
	//2.byte数组
	//3.字符编码jstring
	jclass str_cls = (*env)->FindClass(env, "java/lang/String");
	jmethodID constructor_mid = (*env)->GetMethodID(env, str_cls, "<init>", "([BLjava/lang/String;)V");

	//一个  jbyte 对等于  一个 char
	//jbyteArray 对等于 char[]
	jbyteArray bytes = (*env)->NewByteArray(env, strlen(c_str));
	//byte数组赋值
	//0->strlen(c_str)，从头到尾
	//对等于，从c_str这个字符数组，复制到bytes这个字符数组
	(*env)->SetByteArrayRegion(env, bytes, 0, strlen(c_str), c_str);

	//字符串编码
	jstring charsetName = (*env)->NewStringUTF(env, "UTF-8");

	//调用构造函数，返回编码之后的jstring
	return (*env)->NewObject(env,str_cls,constructor_mid,bytes,charsetName);
}

//10、传入数组
//C比较器
int compare(int *a, int *b){
	return (*a) - (*b);
}
JNIEXPORT void JNICALL Java_com_lichao_javajni_JavaJni_giveArray
  (JNIEnv *env, jobject jobj, jintArray arr){
	//jintArray 转成  jint指针   转成    c  int 数组
	//JNI_TRUE  两个互不相干，java和C独自操作自己的数组
	//JNI_FALSE 后面必须同步，java和C独自操同一个数组
	//使用JNI_TRUE程序会报错，官方推荐使用NULL，常用NULL和JNI_FALSE
	jint *elems = (*env)->GetIntArrayElements(env, arr, NULL);
	//数组长度
	int len = (*env)->GetArrayLength(env, arr);
	//排序
	qsort(elems, len, sizeof(jint), compare);

	//同步
	//mode
	//0, Java数组进行更新，并且释放C/C++数组
	//JNI_ABORT, Java数组不进行更新，但是释放C/C++数组
	//JNI_COMMIT，Java数组进行更新，不释放C/C++数组（函数执行完，数组还是会释放）
	(*env)->ReleaseIntArrayElements(env,arr, elems, JNI_COMMIT);
}

//11、返回数组
JNIEXPORT jintArray JNICALL Java_com_lichao_javajni_JavaJni_getArray
  (JNIEnv *env, jobject jobj, jint len){
	//创建一个指定大小数组
	jintArray jint_arr = (*env)->NewIntArray(env, len);
	//jintArray 转成  jint指针   转成    c  int 数组
	jint *elems = (*env)->GetIntArrayElements(env, jint_arr, NULL);
	int i = 0;
	for(;i < len; i++){
		elems[i] = i;
	}
	//同步
	(*env)->ReleaseIntArrayElements(env, jint_arr, elems, 0);
	return jint_arr;
}

//12、JNI 引用变量
//引用类型：局部引用和全局引用
//作用：在JNI中告知虚拟机何时回收一个JNI变量

//局部引用，通过DeleteLocalRef手动释放对象
//1.访问一个很大的java对象，使用完之后，还要进行复杂的耗时操作
//2.创建了大量的局部引用，占用了太多的内存，而且这些局部引用跟后面的操作没有关联性
//模拟：循环创建数组
JNIEXPORT void JNICALL Java_com_lichao_javajni_JavaJni_localRef
  (JNIEnv *env, jobject jobj){
	int i = 0;
	for(; i<5 ; i++){
		//创建Date对象
		jclass cls =(*env)->FindClass(env, "java/util/Date");
		jmethodID constructor_mid = (*env)->GetMethodID(env, cls, "<init>", "()V");
		jobject obj = (*env)->NewObject(env, cls, constructor_mid);

		//其他逻辑代码...

		//不在使用jobject对象了
		//通知垃圾回收器回收这些对象
		(*env)->DeleteLocalRef(env, obj);
		//其他逻辑代码...
	}
}

//13、全局引用
//共享(可以跨多个线程)，手动控制内存使用
jstring global_str;

//创建
JNIEXPORT void JNICALL Java_com_lichao_javajni_JavaJni_createGlobalRef
  (JNIEnv *env, jobject jobj){
	jstring obj = (*env)->NewStringUTF(env, "jni development is powerful!");
	global_str = (*env)->NewGlobalRef(env, obj);
}

//获得
JNIEXPORT jstring JNICALL Java_com_lichao_javajni_JavaJni_getGlobalRef
  (JNIEnv *env, jobject jobj){
	return global_str;
}

//释放
JNIEXPORT void JNICALL Java_com_lichao_javajni_JavaJni_deleteGlobalRef
  (JNIEnv *env, jobject jobj){
	(*env)->DeleteGlobalRef(env, global_str);
}

//14、弱全局引用
//节省内存，在内存不足时可以是释放所引用的对象
//可以引用一个不常用的对象，如果为NULL，临时创建
//创建：NewWeakGlobalRef,销毁：DeleteGlobalWeakRef

//异常处理
//1.保证Java代码可以运行
//2.补救措施保证C代码继续运行

//JNI自己抛出的异常，在Java层无法被捕捉，只能在C层清空
//用户通过ThrowNew抛出的异常，可以在Java层捕捉
JNIEXPORT void JNICALL Java_com_lichao_javajni_JavaJni_exeception
  (JNIEnv *env, jobject jobj){
	jclass cls = (*env)->GetObjectClass(env, jobj);
	jfieldID fid = (*env)->GetFieldID(env, cls, "key2", "Ljava/lang/String;");
	//检测是否发生java异常
	jthrowable exception = (*env)->ExceptionOccurred(env);
	if(exception != NULL){
		//让java代码可以继续运行
		//清空异常信息
		(*env)->ExceptionClear(env);

		//补救措施
		fid = (*env)->GetFieldID(env, cls, "key", "Ljava/lang/String;");
	}
	//获取属性的值
	jstring jstr = (*env)->GetObjectField(env, jobj, fid);
	char *str = (*env)->GetStringUTFChars(env, jstr, NULL);

	//对比属性值是否合法  在android jni中 使用 strcmp 不能使用  stricmp
	//如果key2 和 key 不一样 则抛出异常给java处理
	//如果key2 和 key 一样，java不能捕捉异常，程序直接挂掉
	int result = strcmp(str, "super jason");
	if(result != 0){
		//认为抛出异常，给Java层处理
		jclass newExcCls = (*env)->FindClass(env,"java/lang/IllegalArgumentException");
		(*env)->ThrowNew(env,newExcCls,"key's value is invalid!");
	}
}

//15、缓存策略
//static jfieldID key_id   //全局静态
JNIEXPORT void JNICALL Java_com_lichao_javajni_JavaJni_cached
  (JNIEnv *env, jobject jobj){
	jclass cls = (*env)->GetObjectClass(env, jobj);
	//获取jfieldID只获取一次
	//局部静态变量（只能在这个作用域使用）。使用 static缓存循环只执行1次  去掉 static则执行for循环次数
	static jfieldID key_id = NULL;
	if (key_id == NULL){
		key_id = (*env)->GetFieldID(env, cls, "key", "Ljava/lang/String;");
		printf("--------GetFieldID-------\n");
		LOGE("%s","--------GetFieldID-------\n");
	}
}

//16、初始化全局变量，动态库加载完成之后，立刻缓存起来
jfieldID key_fid;
jmethodID random_mid;
JNIEXPORT void JNICALL Java_com_lichao_javajni_JavaJni_initIds
  (JNIEnv *env, jclass jcls){
	key_fid = (*env)->GetFieldID(env, jcls, "key", "Ljava/lang/String;");
	random_mid = (*env)->GetMethodID(env, jcls, "genRandomInt", "(I)I");
}
