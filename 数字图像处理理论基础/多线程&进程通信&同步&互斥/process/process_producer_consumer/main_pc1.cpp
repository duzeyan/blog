//�򵥵����������������� 2�������� 1�������� ��������СΪ4
//ͬ������Ҫ�����ź�������ɡ�full����֪ͨ�����������ݣ���ʼΪ0
// empty ����֪ͨ�������Ƿ���Խ�����������ʼΪ4
//���⣬�ùؼ���
#include<stdio.h>
#include<process.h>
#include<Windows.h>

const int num_max=10;
int g_buffer[4];     //��Ʒ 
int g_i,g_j;
HANDLE g_semaphoreFull,g_semaphoreEmpty;
CRITICAL_SECTION g_sectionP;

unsigned int _stdcall Thread_producer(LPVOID lp){
	for(int i=0;i<num_max;i++){
		WaitForSingleObject(g_semaphoreEmpty,INFINITE); //wait for empty  ��һ����
		Sleep(100);

		EnterCriticalSection(&g_sectionP);//��
		g_buffer[g_i]=i;
		printf("������%d������ %d ���뻺���� %d ----- \n",*(int *)lp,g_buffer[g_i],g_i);
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
		printf("   �����ߴ� %d �������� %d    ++++ \n",g_j,g_buffer[g_j]);
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