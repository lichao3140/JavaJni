package com.lichao.javajni;

public class Man extends Human{

	@Override
	public void sayHi() {
		System.out.println("我是男人，我做主!  子类");
	}
	
	public void sayHi2(){
		super.sayHi();
	}
}
