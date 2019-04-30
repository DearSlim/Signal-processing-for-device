#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

char request;

HANDLE InitCOM(LPCTSTR Port)
{
	HANDLE hCom = INVALID_HANDLE_VALUE;
	hCom = CreateFile(Port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
		0/*0 stands for Synchronized method*/, NULL);
	if (INVALID_HANDLE_VALUE == hCom)
	{
		return INVALID_HANDLE_VALUE;
	}
	SetupComm(hCom, 4096, 4096);//Set buffer area

	DCB dcb;

	GetCommState(hCom, &dcb);//set COM port
	dcb.DCBlength = sizeof(dcb);
	dcb.BaudRate = 9600;
	dcb.StopBits = ONESTOPBIT;
	SetCommState(hCom, &dcb);

	PurgeComm(hCom, PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT);//Buffer clearance

	COMMTIMEOUTS ct;
	//Set timeout and the longest time Readfile accepts
	ct.ReadIntervalTimeout = 0;
	ct.ReadTotalTimeoutConstant = 500;
	ct.ReadTotalTimeoutMultiplier = 50;

	ct.WriteTotalTimeoutMultiplier = 50;
	ct.WriteTotalTimeoutConstant = 5;

	SetCommTimeouts(hCom, &ct);//set timeout

	return hCom;
}

bool ComRead(HANDLE hCom, unsigned char buf[16], int len)
{
	PurgeComm(hCom, PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT);
	DWORD ReadSize = 0;
	BOOL rtn = FALSE;

	//Read 1 bit data£¬data would be returned once buffer has data£¬otherwise wait until the timeout.
	rtn = ReadFile(hCom, buf, 1, &ReadSize, NULL);

	//rtn=true if timeout & ReadSize=0£¬if there's data coming£¬ReadSize=1
	if (rtn == TRUE && 1 == ReadSize)
	{
		DWORD Error;
		COMSTAT cs = { 0 };
		int ReadLen = 0;
		//Check how much data needs to be read£¬store it into cs.cbInQue
		ClearCommError(hCom, &Error, &cs);
		ReadLen = (cs.cbInQue > len) ? len : cs.cbInQue;
		if (ReadLen > 0)
		{
			//buf+1 since 1 bit was read during waiting
			rtn = ReadFile(hCom, buf + 1, ReadLen, &ReadSize, NULL);
			len = 0;
			if (rtn)
			{
				len = ReadLen + 1;
			}
		}
	}
	PurgeComm(hCom, PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT);
	return rtn != FALSE;
}

bool ComWrite(HANDLE hCom, unsigned char buf[16], int len)
{
	PurgeComm(hCom, PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT);
	BOOL rtn = FALSE;
	DWORD WriteSize = 0;
	rtn = WriteFile(hCom, buf, len, &WriteSize, NULL);
	len = WriteSize;
	return rtn != FALSE;
}