// ����ƪ����
// http://blog.csdn.net/morewindows/article/details/7445233
// ��ƪʹ����Event���������̺߳����߳�֮���ͬ����
// ע���1������Event��ʱ��ڶ�������ȷ���Ƿ�Ϊ�ֶ���λ�����FALSE����ÿ��wait��
// �����Զ��趨��EventΪδ����״̬�� ���ΪTRUE����һֱ�Ǵ���״̬��ֱ��ʹ��ResetEvent
// ע���2����ͬ�ġ����̡�����ͨ��OpenEvent���ͬһ��Evetnt�ľ����Ҳ����˵EventҲ����
// ����֮���ͬ��
// ע���3���ڱ����У����߳̽��յĲ�����һ��ָ�룬�����߳��в�Ӧ����ʹ��ʱʹ�ø�ָ�룬
// �����籾���е�printf��ʹ�ø�ָ���ʱ��,����������Ѿ����ܱ�֤���Ǹտ�ʼ�����̵߳�ֵ�ˡ�
// ��ȷ���Ǵ���һ������ص��ڴ�����(����Ϊջ)
// ע���4����ȻEvent�������������̺߳ͽ��̵�ͬ������ôҲ����ʵ�ֻ��⣬�ڱ����н����˸�д
// ��֤�����Դ���Ͳ����и��Ĵ��벻һ��

#include<stdio.h>
#include<windows.h>
#include<process.h>

int g_count=0;
const int T_MAX=10;
HANDLE g_eventI;
HANDLE g_eventC;


unsigned int _stdcall Thread_Fun(LPVOID lp){
	int i=*(int *)(lp);

	//���������¼�
	SetEvent(g_eventI);

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

	//�Զ���λ ��ʼΪδ����
	g_eventI=CreateEvent(NULL,TRUE,FALSE,NULL);
	g_eventC=CreateEvent(NULL,FALSE,TRUE,NULL);


	//��ʼ���ؼ��ֶ�

	for(int i=0;i<T_MAX;i++){
		handles[i]=(HANDLE)_beginthreadex(NULL,0,Thread_Fun,&i,0,NULL);
		WaitForSingleObject(g_eventI,INFINITE);  //δ����ʱ  ������߳�
		ResetEvent(g_eventI); //�ֶ�����Ϊδ����
	}
	WaitForMultipleObjects(T_MAX,handles,TRUE,INFINITE);

	//�ر��¼����
	CloseHandle(g_eventI);
	CloseHandle(g_eventC);

	return 0;
}