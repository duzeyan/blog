// ��8ƪ����

#include<stdio.h>
#include<windows.h>
#include<process.h>

int g_count=0;
const int T_MAX=10;
HANDLE g_semaphoreI;
HANDLE g_eventC;


unsigned int _stdcall Thread_Fun(LPVOID lp){
	int i=*(int *)(lp);

	//���������¼�
	ReleaseSemaphore(g_semaphoreI,1,NULL);

	Sleep(100);

	//��Event����ؼ���
	WaitForSingleObject(g_eventC,INFINITE);
	g_count++;
	Sleep(10);
	printf("�̱߳�ţ�%d  �߳����ȫ�ֱ���:%d \n",i,g_count);
	//���� �������ȴ����߳̽�������
	SetEvent(g_eventC);

	return 0;
}

int main(){
	HANDLE handles[T_MAX];

	
	g_semaphoreI=CreateSemaphore(NULL,0,1,NULL);
	//�Զ���λ ��ʼΪδ����
	g_eventC=CreateEvent(NULL,FALSE,TRUE,NULL);


	//��ʼ���ؼ��ֶ�

	for(int i=0;i<T_MAX;i++){
		handles[i]=(HANDLE)_beginthreadex(NULL,0,Thread_Fun,&i,0,NULL);
		WaitForSingleObject(g_semaphoreI,INFINITE);  //δ����ʱ  ������߳�
	}
	WaitForMultipleObjects(T_MAX,handles,TRUE,INFINITE);

	//�ر��¼����
	CloseHandle(g_semaphoreI);
	CloseHandle(g_eventC);

	return 0;
}