/// ����ƪ���� 
//  http://blog.csdn.net/morewindows/article/details/7442639
//  ʹ�ùؼ��ν����̼߳�Ļ��⣬�������н����ʾ,g_sectionI�޷�ʵ�����̺߳����߳�֮���
//  ��ͬ�����ùؼ��ΰ����̵߳Ļ�������ס���ȽϺ���⡣
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

	EnterCriticalSection(&g_sectionC);  //��һ������Ϊ�������
	g_count++;
	Sleep(10);
	printf("�̱߳�ţ�%d  �߳����ȫ�ֱ���:%d \n",*it,g_count);
	LeaveCriticalSection(&g_sectionC);

	return 0;
}

int main(){
	HANDLE handles[T_MAX];

	//��ʼ���ؼ��ֶ�
	InitializeCriticalSection(&g_sectionC);
	InitializeCriticalSection(&g_sectionI);

	for(int i=0;i<T_MAX;i++){
		EnterCriticalSection(&g_sectionI);
		handles[i]=(HANDLE)_beginthreadex(NULL,0,Thread_Fun,&i,0,NULL);
	}
	WaitForMultipleObjects(T_MAX,handles,TRUE,INFINITE);

	//���ٹؼ���
	DeleteCriticalSection(&g_sectionC);
	DeleteCriticalSection(&g_sectionI);
	return 0;
}