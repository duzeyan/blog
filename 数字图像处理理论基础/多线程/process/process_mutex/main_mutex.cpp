//// ��7ƪ����
//
//
//#include<stdio.h>
//#include<windows.h>
//#include<process.h>
//
//int g_count=0;
//const int T_MAX=10;
//HANDLE g_mutexI;
//HANDLE g_eventC;
//
//
//unsigned int _stdcall Thread_Fun(LPVOID lp){
//	int i=*(int *)(lp);
//	ReleaseMutex(g_mutexI);
//	
//
//	Sleep(100);
//
//	//��Event����ؼ���
//	WaitForSingleObject(g_eventC,INFINITE);
//	g_count++;
//	Sleep(10);
//	printf("�̱߳�ţ�%d  �߳����ȫ�ֱ���:%d \n",i,g_count);
//	//���� �������ȴ����߳̽�������
//	SetEvent(g_eventC);
//
//	return 0;
//}
//
//int main(){
//	HANDLE handles[T_MAX];
//
//	//�Զ���λ ��ʼΪδ����
//	g_mutexI=CreateMutex(NULL,FALSE,NULL);
//	g_eventC=CreateEvent(NULL,FALSE,TRUE,NULL);
//
//
//	//��ʼ���ؼ��ֶ�
//
//	for(int i=0;i<T_MAX;i++){
//		handles[i]=(HANDLE)_beginthreadex(NULL,0,Thread_Fun,&i,0,NULL);
//		WaitForSingleObject(g_mutexI,INFINITE);  //δ����ʱ  ������߳�
//	}
//	WaitForMultipleObjects(T_MAX,handles,TRUE,INFINITE);
//
//	//�ر��¼����
//	CloseHandle(g_mutexI);
//	CloseHandle(g_eventC);
//
//	return 0;
//}
#include <stdio.h>
#include <conio.h>
#include <windows.h>
const char MUTEX_NAME[] = "Mutex_MoreWindows";
int main()
{
	HANDLE hMutex = CreateMutex(NULL, FALSE, (LPCWSTR)MUTEX_NAME); //����������
	printf("�������Ѿ����������ڰ����������������\n");
	getch();
	//exit(0);
	ReleaseMutex(hMutex);
	printf("�������Ѿ�����\n");
	CloseHandle(hMutex);
	return 0;
}