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
		0/*同步方式打开串口*/, NULL);
	if (INVALID_HANDLE_VALUE == hCom)
	{
		return INVALID_HANDLE_VALUE;
	}
	SetupComm(hCom, 4096, 4096);//设置缓存

	DCB dcb;

	GetCommState(hCom, &dcb);//设置串口
	dcb.DCBlength = sizeof(dcb);
	dcb.BaudRate = 9600;
	dcb.StopBits = ONESTOPBIT;
	SetCommState(hCom, &dcb);

	PurgeComm(hCom, PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT);//清空缓存

	COMMTIMEOUTS ct;
	//设置读取超时时间，及ReadFlie最长等待时间
	ct.ReadIntervalTimeout = 0;
	ct.ReadTotalTimeoutConstant = 500;
	ct.ReadTotalTimeoutMultiplier = 50;

	ct.WriteTotalTimeoutMultiplier = 50;
	ct.WriteTotalTimeoutConstant = 5;

	SetCommTimeouts(hCom, &ct);//设置超时

	return hCom;
}

bool ComRead(HANDLE hCom, unsigned char buf[16], int len)
{
	PurgeComm(hCom, PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT);
	DWORD ReadSize = 0;
	BOOL rtn = FALSE;

	//设置读取1个字节数据，当缓存中有数据到达时则会立即返回，否则直到超时
	rtn = ReadFile(hCom, buf, 1, &ReadSize, NULL);

	//如果是超时rtn=true但是ReadSize=0，如果有数据到达，会读取一个字节ReadSize=1
	if (rtn == TRUE && 1 == ReadSize)
	{
		DWORD Error;
		COMSTAT cs = { 0 };
		int ReadLen = 0;
		//查询剩余多少字节未读取，存储于cs.cbInQue中
		ClearCommError(hCom, &Error, &cs);
		ReadLen = (cs.cbInQue > len) ? len : cs.cbInQue;
		if (ReadLen > 0)
		{
			//由于之前等待时以读取一个字节，故buf+1
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