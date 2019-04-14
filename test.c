#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "signalProcessing.h"

//≤‚ ‘ ˝æ›∂¡»°
//int main(void)
//{
//	HANDLE hCom = InitCOM(TEXT("COM4"));
//
//	if (hCom == INVALID_HANDLE_VALUE)
//	{
//		printf("COM setup failed");
//	}
//	else { printf("COM set up success\n"); }
//
//	if (ComWrite(hCom, "dd\r", 8))
//	{
//		char readSize[4096] = { 0 };
//		while (1)
//		{
//			if (ComRead(hCom, &readSize, 4096))
//			{
//				for (int i = 0; i < 4096; i++)
//				{
//					if (readSize[i] != 0)
//					{
//						printf("%c", readSize[i]);
//					}
//					//printf("|");
//				}
//			}
//			for (int i = 0; i < 4096; i++)
//			{
//				readSize[i] = 0;
//			}
//			PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
//			//printf("||");
//		}
//
//	}
//	else
//	{
//		printf("failed");
//	}
//	return 0;
//}

//Real time processing
int main()
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
				readSize[i] = 35;//#∫≈
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



//
////Test the function of FFT
//int main()
//{
//	//Test reverse function 
//	printf("--------------------Test reverse function\n");
//	set12345();
//	reverse();
//
//	printf("--------------------Test FFT of triangle wave \n");
//	setTriangleWave();
//	FFT2();
//
//	printf("--------------------Test FFT of rectangular wave \n");
//	setRectangularWave();
//	FFT2();
//
//	printf("--------------------Test FFT of pulse wave \n");
//	setPulseWave();
//	FFT2();
//	return 0;
//}

////Test coerelation coefficient 
//int main()
//{
//	setSin();
//	FFT2();
//	
//	return 0;
//}