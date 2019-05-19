#include"source.cpp"
#include<windows.h>
#include<string>
#define THREADCOUNT 5

DWORD dwTlsIndex;

// Function declarations and definitions...
VOID ErrorExit(LPTSTR lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);
	ExitProcess(0);
}

void MyCommonFunction(void)
{
	LPVOID lpvData;

	// Retrieve a data pointer for the current thread...
	lpvData = TlsGetValue(dwTlsIndex);

	if ((lpvData == 0) && (GetLastError() != 0))
		ErrorExit("TlsGetValue() error!\n");
	else
		printf("TlsGetValue() is OK.\n");

	// Use the data stored for the current thread...
	printf("Common: thread %u: lpvData = %p\n\n", GetCurrentThreadId(), lpvData);
}

DWORD WINAPI MyThreadFunc(void)
{
	LPVOID lpvData;

	// Initialize the TLS index for this thread.
	lpvData = (LPVOID)LocalAlloc(LPTR , 256);

	if (!TlsSetValue(dwTlsIndex, lpvData))
		ErrorExit("TlsSetValue() error!\n");
	else
		printf("TlsSetValue() is OK.\n");

	printf("Thread %u: lpvData = %p\n", GetCurrentThreadId(), lpvData);
	MyCommonFunction();

	// Release the dynamic memory before the thread returns...
	lpvData = TlsGetValue(dwTlsIndex);
	if (lpvData != 0)
		LocalFree((HLOCAL)lpvData);
	return 0;
}

DWORD main(void)
{
	DWORD IDThread;
	HANDLE hThread[THREADCOUNT];
	int i;

	printf("Thread count is: %u\n", THREADCOUNT);
	//cout << "Thread Count is : %u\n" + THREADCOUNT;
	// Allocate a TLS index...
	if ((dwTlsIndex = TlsAlloc()) == -1)
		ErrorExit("TlsAlloc() failed");
	else
		printf("\nTlsAlloc() is OK.\n\n");

	// Create multiple threads...
	for (i = 0; i < THREADCOUNT; i++)
	{
		hThread[i] = CreateThread(NULL, // no security attributes
			0, // use default stack size
			(LPTHREAD_START_ROUTINE)MyThreadFunc, // thread function
			NULL, // no thread function argument
			0, // use default creation flags
			&IDThread); // returns thread identifier

						// Check the return value for success...
		if (hThread[i] == NULL)
			ErrorExit("CreateThread() error.\n");
		else
			printf("hThread[%u] is OK.\n", i);
	}
	
	for (i = 0; i < THREADCOUNT; i++)
		WaitForSingleObject(hThread[i], INFINITE);

	if (TlsFree(dwTlsIndex) == 0)
	{
		printf("TlsFree() failed!\n");
		cout << "TlsFree() failed!\n" << endl;
	}
	else
		printf("TlsFree() is OK!\n");
	    cout<<"TlsFree() is OK!\n"<<endl;

	system("Pause");
	return 0;
}
