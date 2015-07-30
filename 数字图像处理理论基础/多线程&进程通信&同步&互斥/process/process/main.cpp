//前三篇的内容
//http://blog.csdn.net/morewindows/article/details/7429155

#include <stdio.h>
#include <process.h>
#include <windows.h>

int const HNUM=50;
volatile int g_count;

unsigned int _stdcall  Thread_Fun(LPVOID lp){
	Sleep(50);
	g_count++;
	//InterlockedIncrement((LPLONG)(&g_count));
	Sleep(100);
	//printf("%d 子线程ID  全局变量为：%d\n",GetCurrentThreadId(),g_count);
	return 0;
}


int main(){
	int num=20;
	while(num--){
		g_count=0;
		HANDLE handles[HNUM];
		for(int i=0;i<HNUM;i++)
			handles[i]=(HANDLE)_beginthreadex(NULL,0,Thread_Fun,NULL,0,NULL);
		WaitForMultipleObjects(HNUM,handles,TRUE,INFINITE);
		printf("全局变量为：%d\n",g_count);
	}
	return 0;
}