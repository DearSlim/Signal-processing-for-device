#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "signalProcessing.h"

int startLink()
{
	HANDLE hCom = InitCOM(TEXT("COM4"));

	if (hCom == INVALID_HANDLE_VALUE)
	{
		printf("COM setup failed");
	}
	else { printf("COM set up success\n"); }

	if (ComWrite(hCom, "ddf\r", 8))
	{
		char readSize[4096] = { 0 };
		while (1)
		{
			if (ComRead(hCom, &readSize, 4096))
			{
				distinguishTime_Value(processData(readSize));
			}
			for (int i = 0; i < 4096; i++)
			{
				readSize[i] = 35;//#ºÅ
			}
			PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
			//printf("||");
		}

	}
	else
	{
		printf("failed");
	}
	return 0;
}