// 第8篇内容

#include<stdio.h>
#include<windows.h>
#include<process.h>

int g_count=0;
const int T_MAX=10;
HANDLE g_semaphoreI;
HANDLE g_eventC;


unsigned int _stdcall Thread_Fun(LPVOID lp){
	int i=*(int *)(lp);

	//触发匿名事件
	ReleaseSemaphore(g_semaphoreI,1,NULL);

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

	
	g_semaphoreI=CreateSemaphore(NULL,0,1,NULL);
	//自动置位 初始为未触发
	g_eventC=CreateEvent(NULL,FALSE,TRUE,NULL);


	//初始化关键字段

	for(int i=0;i<T_MAX;i++){
		handles[i]=(HANDLE)_beginthreadex(NULL,0,Thread_Fun,&i,0,NULL);
		WaitForSingleObject(g_semaphoreI,INFINITE);  //未触发时  会挂起线程
	}
	WaitForMultipleObjects(T_MAX,handles,TRUE,INFINITE);

	//关闭事件句柄
	CloseHandle(g_semaphoreI);
	CloseHandle(g_eventC);

	return 0;
}