/// 第五篇内容 
//  http://blog.csdn.net/morewindows/article/details/7442639
//  使用关键段进行线程间的互斥，代码运行结果显示,g_sectionI无法实现主线程和子线程之间的
//  的同步。用关键段把子线程的互斥代码包住，比较好理解。
#include<stdio.h>
#include<windows.h>
#include<process.h>

int g_count=0;
const int T_MAX=10;
CRITICAL_SECTION g_sectionI,g_sectionC;

unsigned int _stdcall Thread_Fun(LPVOID lp){
	int* it=(int *)(lp);
	LeaveCriticalSection(&g_sectionI);

	Sleep(100);

	EnterCriticalSection(&g_sectionC);  //这一段设置为互斥代码
	g_count++;
	Sleep(10);
	printf("线程编号：%d  线程输出全局变量:%d \n",*it,g_count);
	LeaveCriticalSection(&g_sectionC);

	return 0;
}

int main(){
	HANDLE handles[T_MAX];

	//初始化关键字段
	InitializeCriticalSection(&g_sectionC);
	InitializeCriticalSection(&g_sectionI);

	for(int i=0;i<T_MAX;i++){
		EnterCriticalSection(&g_sectionI);
		handles[i]=(HANDLE)_beginthreadex(NULL,0,Thread_Fun,&i,0,NULL);
	}
	WaitForMultipleObjects(T_MAX,handles,TRUE,INFINITE);

	//销毁关键段
	DeleteCriticalSection(&g_sectionC);
	DeleteCriticalSection(&g_sectionI);
	return 0;
}