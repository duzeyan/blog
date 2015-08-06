//��д������忴��
//������������g_eventR���ƶ���ʲôʱ����Ϳ�����༸�����Զ�����һ��Ŀ�Ĵﵽ�ˣ����ǵڶ���Ŀ��û�дﵽ
//Ҫ�벢���Ŀ��������ٸ����ߣ�������Ҫ�ź���������

#include<stdio.h>
#include<process.h>
#include<Windows.h>

const int MAX_NUM=2;			//���ͬʱ5�����߽���
int g_readCount=0;				//���߽��̼���
CRITICAL_SECTION g_criticalP;   //���ƶ��߽��� �Զ������������޸�
HANDLE g_eventW,g_eventR;				//����д�߽���  ���߽��̶�

unsigned int _stdcall Thread_writer(LPVOID lp){
	
		WaitForSingleObject(g_eventW,INFINITE);
		

		printf("+д�߽��̿�ʼд�롣����\n");
		Sleep(500);
		printf("+д�߽���д����ɣ�\n");


	
		SetEvent(g_eventR);
	

	return 0;
}


unsigned int _stdcall Thread_reader(LPVOID lp){


 	WaitForSingleObject(g_eventR,INFINITE);//�ȴ�д��д���

	EnterCriticalSection(&g_criticalP);
	g_readCount++;
	if(g_readCount==1)         // ��һ������ ��סд�߽���
		 ResetEvent(g_eventW);
	if((MAX_NUM)==g_readCount){
		 //printf("���߶���������\n");
		 ResetEvent(g_eventR); //��ס�������������̽���
	}
	LeaveCriticalSection(&g_criticalP);

	printf("-�� %d ���������ڶ�ȡ����....\n",GetCurrentThreadId());
	Sleep(1000);
	printf("-�� %d �����߶�ȡ������ɣ�\n",GetCurrentThreadId());

	EnterCriticalSection(&g_criticalP);
	g_readCount--;
	if(g_readCount==0)     // ���һ������ ����д�߽���
		 SetEvent(g_eventW);
	if(g_readCount<=MAX_NUM-1){
		SetEvent(g_eventR); //���Լ�����������
	}
	LeaveCriticalSection(&g_criticalP);

	return 0;
}
int main(){
	InitializeCriticalSection(&g_criticalP);
	g_eventW=CreateEvent(NULL,TRUE,TRUE,NULL); //��ʼ��Ϊ���� �ֶ�
	g_eventR=CreateEvent(NULL,TRUE,FALSE,NULL); //��ʼ��Ϊδ���� �ֶ�

	HANDLE handles[7];


	//��һ��д�߽���
	handles[6]=(HANDLE)_beginthreadex(NULL,0,Thread_writer,NULL,TRUE,0);

	//���������߽���
	for(int i=0;i<6;i++)
	  handles[i]=(HANDLE)_beginthreadex(NULL,0,Thread_reader,NULL,TRUE,0);

	
	
	


	WaitForMultipleObjects(7,handles,TRUE,INFINITE);

	CloseHandle(g_eventW);
	CloseHandle(g_eventR);
	DeleteCriticalSection(&g_criticalP);
	return 0;
}
