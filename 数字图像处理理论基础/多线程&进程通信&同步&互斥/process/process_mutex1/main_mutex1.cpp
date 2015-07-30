#include <stdio.h>
#include <windows.h>
const char MUTEX_NAME[] = "Mutex_MoreWindows";
int main()
{
	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, TRUE, (LPCWSTR)MUTEX_NAME); //打开互斥量
	if (hMutex == NULL)
	{
		printf("打开互斥量失败\n");
		return 0;
	}
	printf("等待中....\n");
	DWORD dwResult = WaitForSingleObject(hMutex, 20 * 1000); //等待互斥量被触发
	switch (dwResult)
	{
	case WAIT_ABANDONED:
		printf("拥有互斥量的进程意外终止\n");
		break;

	case WAIT_OBJECT_0:
		printf("已经收到信号\n");
		break;

	case WAIT_TIMEOUT:
		printf("信号未在规定的时间内送到\n");
		break;
	}
	CloseHandle(hMutex);
	return 0;
}