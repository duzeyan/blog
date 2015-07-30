////简单的生产者消费者问题 一个生产者 一个消费者 缓存区大小为1
////这里只要简单的使用线程同步即可
//#include<stdio.h>
//#include<process.h>
//#include<Windows.h>
//
//const int num_max=10;
//int g_buffer=0;//产品 该例中 只有0，1两种状态
//HANDLE g_eventFull,g_eventEmpty;
//CRITICAL_SECTION g_sectionP;
//
//unsigned int _stdcall Thread_producer(LPVOID lp){
//	for(int i=0;i<num_max;i++){
//		WaitForSingleObject(g_eventEmpty,INFINITE); //wait for empty
//
//		g_buffer=i;
//		printf("生产者把数据 %d 放入缓存区 ----- \n",g_buffer);
//
//		SetEvent(g_eventFull);  //call the consumer
//	}
//	return 0;
//}
//
//unsigned int _stdcall Thread_consumer(LPVOID lp){
//	bool flag=true;
//	while(flag){
//		WaitForSingleObject(g_eventFull,INFINITE); //wait for full
//
//		printf("    消费者消费数据 %d    ++++ \n",g_buffer);
//		if(g_buffer==num_max-1)
//			flag=false;
//
//		SetEvent(g_eventEmpty);   //call the procucer
//
//	}
//	return 0;
//}
//
//int main(){
//	HANDLE handles[2];
//
//	g_eventFull=CreateEvent(NULL,FALSE,FALSE,NULL);
//	g_eventEmpty=CreateEvent(NULL,FALSE,TRUE,NULL);
//	//InitializeCriticalSection(&g_sectionP);
//
//
//	handles[0]=(HANDLE)_beginthreadex(NULL,0,Thread_producer,NULL,0,NULL);
//	handles[1]=(HANDLE)_beginthreadex(NULL,0,Thread_consumer,NULL,0,NULL);
//
//	WaitForMultipleObjects(2,handles,TRUE,INFINITE);
//
//	CloseHandle(handles[0]);
//	CloseHandle(handles[1]);
//
//
//	CloseHandle(g_eventFull);
//	CloseHandle(g_eventEmpty);
//
//	return 0;
//	
//}