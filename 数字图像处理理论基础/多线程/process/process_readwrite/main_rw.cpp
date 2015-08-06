//读写问题具体看书
//这里程序打算用g_eventR控制读者什么时候读和控制最多几个可以读。第一个目的达到了，但是第二个目的没有达到
//要想并发的控制最多多少个读者，还是需要信号量来控制

#include<stdio.h>
#include<process.h>
#include<Windows.h>

const int MAX_NUM=2;			//最多同时5个读者进程
int g_readCount=0;				//读者进程计数
CRITICAL_SECTION g_criticalP;   //控制读者进程 对读者数量进行修改
HANDLE g_eventW,g_eventR;				//控制写者进程  读者进程读

unsigned int _stdcall Thread_writer(LPVOID lp){
	
		WaitForSingleObject(g_eventW,INFINITE);
		

		printf("+写者进程开始写入。。。\n");
		Sleep(500);
		printf("+写者进程写入完成！\n");


	
		SetEvent(g_eventR);
	

	return 0;
}


unsigned int _stdcall Thread_reader(LPVOID lp){


 	WaitForSingleObject(g_eventR,INFINITE);//等待写者写完成

	EnterCriticalSection(&g_criticalP);
	g_readCount++;
	if(g_readCount==1)         // 第一个读者 锁住写者进程
		 ResetEvent(g_eventW);
	if((MAX_NUM)==g_readCount){
		 //printf("读者队列已满！\n");
		 ResetEvent(g_eventR); //锁住不让其他读进程进入
	}
	LeaveCriticalSection(&g_criticalP);

	printf("-第 %d 个读者正在读取数据....\n",GetCurrentThreadId());
	Sleep(1000);
	printf("-第 %d 个读者读取数据完成！\n",GetCurrentThreadId());

	EnterCriticalSection(&g_criticalP);
	g_readCount--;
	if(g_readCount==0)     // 最后一个读者 解锁写者进程
		 SetEvent(g_eventW);
	if(g_readCount<=MAX_NUM-1){
		SetEvent(g_eventR); //可以加入其他读者
	}
	LeaveCriticalSection(&g_criticalP);

	return 0;
}
int main(){
	InitializeCriticalSection(&g_criticalP);
	g_eventW=CreateEvent(NULL,TRUE,TRUE,NULL); //初始化为激活 手动
	g_eventR=CreateEvent(NULL,TRUE,FALSE,NULL); //初始化为未激活 手动

	HANDLE handles[7];


	//开一个写者进程
	handles[6]=(HANDLE)_beginthreadex(NULL,0,Thread_writer,NULL,TRUE,0);

	//开两个读者进程
	for(int i=0;i<6;i++)
	  handles[i]=(HANDLE)_beginthreadex(NULL,0,Thread_reader,NULL,TRUE,0);

	
	
	


	WaitForMultipleObjects(7,handles,TRUE,INFINITE);

	CloseHandle(g_eventW);
	CloseHandle(g_eventR);
	DeleteCriticalSection(&g_criticalP);
	return 0;
}
