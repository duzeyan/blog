//��д������忴��
//������������g_eventR���ƶ���ʲôʱ����Ϳ�����༸�����Զ�����һ��Ŀ�Ĵﵽ�ˣ����ǵڶ���Ŀ��û�дﵽ
//Ҫ�벢���Ŀ��������ٸ����ߣ�������Ҫ�ź���������

#include<stdio.h>
#include<process.h>
#include<Windows.h>

const int MAX_NUM=2;			//���ͬʱ5�����߽���
int g_readCount=0;				//���߽��̼���
CRITICAL_SECTION g_criticalP;   //���ƶ��߽��� �Զ������������޸�
SRWLOCK g_srwlock;              //ϵͳʵ�ֵĶ�д��


unsigned int _stdcall Thread_writer(LPVOID lp){
	
	AcquireSRWLockExclusive(&g_srwlock);
		

	printf("+д�߽��̿�ʼд�롣����\n");
	Sleep(500);
	printf("+д�߽���д����ɣ�\n");

	ReleaseSRWLockExclusive(&g_srwlock);
	
	
	

	return 0;
}


unsigned int _stdcall Thread_reader(LPVOID lp){


	AcquireSRWLockShared(&g_srwlock);

	EnterCriticalSection(&g_criticalP);


	printf("-�� %d ���������ڶ�ȡ����....\n",GetCurrentThreadId());
	Sleep(1000);
	printf("-�� %d �����߶�ȡ������ɣ�\n",GetCurrentThreadId());

	LeaveCriticalSection(&g_criticalP);

	ReleaseSRWLockShared(&g_srwlock);

	return 0;
}
int main(){
	InitializeCriticalSection(&g_criticalP);
	InitializeSRWLock(&g_srwlock);

	HANDLE handles[7];


	//��һ��д���߳�
	handles[6]=(HANDLE)_beginthreadex(NULL,0,Thread_writer,NULL,TRUE,0);

	//�����������߳�
	for(int i=0;i<6;i++)
	  handles[i]=(HANDLE)_beginthreadex(NULL,0,Thread_reader,NULL,TRUE,0);


	WaitForMultipleObjects(7,handles,TRUE,INFINITE);


	DeleteCriticalSection(&g_criticalP);
	return 0;
}
