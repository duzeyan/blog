//简单的生产者消费者问题 2个生产者 1个消费者 缓存区大小为4
//同步：需要两个信号量来完成。full用来通知消费者有内容，初始为0
// empty 用来通知生产者是否可以进行生产，初始为4
//互斥，用关键段
#include<stdio.h>
#include<process.h>
#include<Windows.h>

const int num_max=10;
int g_buffer[4];     //产品 
int g_i,g_j;
HANDLE g_semaphoreFull,g_semaphoreEmpty;
CRITICAL_SECTION g_sectionP;

unsigned int _stdcall Thread_producer(LPVOID lp){
	for(int i=0;i<num_max;i++){
		WaitForSingleObject(g_semaphoreEmpty,INFINITE); //wait for empty  减一操作
		Sleep(100);

		EnterCriticalSection(&g_sectionP);//锁
		g_buffer[g_i]=i;
		printf("生产者%d把数据 %d 放入缓存区 %d ----- \n",*(int *)lp,g_buffer[g_i],g_i);
			g_i=(g_i+1)%4;
		LeaveCriticalSection(&g_sectionP);


		ReleaseSemaphore(g_semaphoreFull,1,NULL);    //call the consumer
	}
	return 0;
}

unsigned int _stdcall Thread_consumer(LPVOID lp){
	bool flag=true;
	while(flag){
		WaitForSingleObject(g_semaphoreFull,INFINITE); //wait for full
		Sleep(200);

		EnterCriticalSection(&g_sectionP);
		printf("   消费者从 %d 消费数据 %d    ++++ \n",g_j,g_buffer[g_j]);
		if(g_buffer[g_j]==num_max-1)
			flag=false;
		g_j=(g_j+1)%4;
		LeaveCriticalSection(&g_sectionP);

		ReleaseSemaphore(g_semaphoreEmpty,1,NULL);   //call the procucer

	}
	return 0;
}

int main(){
	HANDLE handles[3];

	g_semaphoreFull=CreateSemaphore(NULL,0,4,NULL);
	g_semaphoreEmpty=CreateSemaphore(NULL,4,4,NULL);
	InitializeCriticalSection(&g_sectionP);
	g_i=0;
	g_j=0;

	int i1=1,i2=2;
	handles[0]=(HANDLE)_beginthreadex(NULL,0,Thread_producer,&i1,0,NULL);
	handles[1]=(HANDLE)_beginthreadex(NULL,0,Thread_consumer,NULL,0,NULL);
	handles[2]=(HANDLE)_beginthreadex(NULL,0,Thread_producer,&i2,0,NULL);


	WaitForMultipleObjects(3,handles,TRUE,INFINITE);

	CloseHandle(handles[0]);
	CloseHandle(handles[1]);
	CloseHandle(handles[2]);



	CloseHandle(g_semaphoreFull);
	CloseHandle(g_semaphoreEmpty);

	return 0;
	
}