// 第六篇内容
// http://blog.csdn.net/morewindows/article/details/7445233
// 本篇使用了Event来进行主线程和子线程之间的同步。
// 注意点1：创建Event的时候第二个参数确定是否为手动置位。如果FALSE，则每次wait后
// 都会自动设定该Event为未触发状态。 如果为TRUE，则一直是触发状态，直到使用ResetEvent
// 注意点2：不同的【进程】可以通过OpenEvent获得同一个Evetnt的句柄。也就是说Event也可以
// 进程之间的同步
// 注意点3：在本例中，子线程接收的参数是一个指针，在子线程中不应该在使用时使用该指针，
// 【例如本例中的printf再使用该指针的时候,里面的内容已经不能保证还是刚开始进入线程的值了】
// 正确的是存在一个不相关的内存区中(本例为栈)
// 注意点4，既然Event可以用来进行线程和进程的同步，那么也可以实现互斥，在本例中进行了改写
// 验证，所以代码和博客中给的代码不一样

#include<stdio.h>
#include<windows.h>
#include<process.h>

int g_count=0;
const int T_MAX=10;
HANDLE g_eventI;
HANDLE g_eventC;


unsigned int _stdcall Thread_Fun(LPVOID lp){
	int i=*(int *)(lp);

	//触发匿名事件
	SetEvent(g_eventI);

	Sleep(100);

	//用Event代替关键段
	WaitForSingleObject(g_eventC,INFINITE);
	g_count++;
	Sleep(10);
	printf("线程编号：%d  线程输出全局变量:%d \n",i,g_count);
	//触发 让其它等待的线程进行挣用
	SetEvent(g_eventC);

	return 0;
}

int main(){
	HANDLE handles[T_MAX];

	//自动置位 初始为未触发
	g_eventI=CreateEvent(NULL,TRUE,FALSE,NULL);
	g_eventC=CreateEvent(NULL,FALSE,TRUE,NULL);


	//初始化关键字段

	for(int i=0;i<T_MAX;i++){
		handles[i]=(HANDLE)_beginthreadex(NULL,0,Thread_Fun,&i,0,NULL);
		WaitForSingleObject(g_eventI,INFINITE);  //未触发时  会挂起线程
		ResetEvent(g_eventI); //手动设置为未触发
	}
	WaitForMultipleObjects(T_MAX,handles,TRUE,INFINITE);

	//关闭事件句柄
	CloseHandle(g_eventI);
	CloseHandle(g_eventC);

	return 0;
}