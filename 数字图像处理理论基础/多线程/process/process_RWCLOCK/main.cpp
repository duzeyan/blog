//读写问题具体看书
//这里程序打算用g_eventR控制读者什么时候读和控制最多几个可以读。第一个目的达到了，但是第二个目的没有达到
//要想并发的控制最多多少个读者，还是需要信号量来控制

#include<stdio.h>
#include<process.h>
#include<Windows.h>

const int MAX_NUM=2;			//最多同时5个读者进程
int g_readCount=0;				//读者进程计数
CRITICAL_SECTION g_criticalP;   //控制读者进程 对读者数量进行修改
SRWLOCK g_srwlock;              //系统实现的读写锁


unsigned int _stdcall Thread_writer(LPVOID lp){
	
	AcquireSRWLockExclusive(&g_srwlock);
		

	printf("+写者进程开始写入。。。\n");
	Sleep(500);
	printf("+写者进程写入完成！\n");

	ReleaseSRWLockExclusive(&g_srwlock);
	
	
	

	return 0;
}


unsigned int _stdcall Thread_reader(LPVOID lp){


	AcquireSRWLockShared(&g_srwlock);

	EnterCriticalSection(&g_criticalP);


	printf("-第 %d 个读者正在读取数据....\n",GetCurrentThreadId());
	Sleep(1000);
	printf("-第 %d 个读者读取数据完成！\n",GetCurrentThreadId());

	LeaveCriticalSection(&g_criticalP);

	ReleaseSRWLockShared(&g_srwlock);

	return 0;
}
int main(){
	InitializeCriticalSection(&g_criticalP);
	InitializeSRWLock(&g_srwlock);

	HANDLE handles[7];


	//开一个写者线程
	handles[6]=(HANDLE)_beginthreadex(NULL,0,Thread_writer,NULL,TRUE,0);

	//开两个读者线程
	for(int i=0;i<6;i++)
	  handles[i]=(HANDLE)_beginthreadex(NULL,0,Thread_reader,NULL,TRUE,0);


	WaitForMultipleObjects(7,handles,TRUE,INFINITE);


	DeleteCriticalSection(&g_criticalP);
	return 0;
}
