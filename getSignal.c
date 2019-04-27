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
		0/*ͬ����ʽ�򿪴���*/, NULL);
	if (INVALID_HANDLE_VALUE == hCom)
	{
		return INVALID_HANDLE_VALUE;
	}
	SetupComm(hCom, 4096, 4096);//���û���

	DCB dcb;

	GetCommState(hCom, &dcb);//���ô���
	dcb.DCBlength = sizeof(dcb);
	dcb.BaudRate = 9600;
	dcb.StopBits = ONESTOPBIT;
	SetCommState(hCom, &dcb);

	PurgeComm(hCom, PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT);//��ջ���

	COMMTIMEOUTS ct;
	//���ö�ȡ��ʱʱ�䣬��ReadFlie��ȴ�ʱ��
	ct.ReadIntervalTimeout = 0;
	ct.ReadTotalTimeoutConstant = 500;
	ct.ReadTotalTimeoutMultiplier = 50;

	ct.WriteTotalTimeoutMultiplier = 50;
	ct.WriteTotalTimeoutConstant = 5;

	SetCommTimeouts(hCom, &ct);//���ó�ʱ

	return hCom;
}

bool ComRead(HANDLE hCom, unsigned char buf[16], int len)
{
	PurgeComm(hCom, PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT);
	DWORD ReadSize = 0;
	BOOL rtn = FALSE;

	//���ö�ȡ1���ֽ����ݣ��������������ݵ���ʱ����������أ�����ֱ����ʱ
	rtn = ReadFile(hCom, buf, 1, &ReadSize, NULL);

	//����ǳ�ʱrtn=true����ReadSize=0����������ݵ�����ȡһ���ֽ�ReadSize=1
	if (rtn == TRUE && 1 == ReadSize)
	{
		DWORD Error;
		COMSTAT cs = { 0 };
		int ReadLen = 0;
		//��ѯʣ������ֽ�δ��ȡ���洢��cs.cbInQue��
		ClearCommError(hCom, &Error, &cs);
		ReadLen = (cs.cbInQue > len) ? len : cs.cbInQue;
		if (ReadLen > 0)
		{
			//����֮ǰ�ȴ�ʱ�Զ�ȡһ���ֽڣ���buf+1
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