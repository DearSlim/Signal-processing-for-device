#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>
#include "math.h"
#include "signalProcessing.h"

double time[1024];
double value[1024];
double savedValue[1024];

int dataSize = 1024;
char buffer[1024];
int count;
int countTime = 0;
int countValue = 0;

int markRealTime = 0;

/**
a[]:The data array
b[]:Newly collected array
length: length of a
distance: shift distance
markReal:whether shifting is necessary
**/
double * moveLength(double a[], double b[], int length, int distance)
{
	double * pt = (double *)calloc(length,sizeof(double));
		for (int i = length - 1; i > distance - 1; i--)
		{
			pt[i] = a[i - distance];
		}
		for (int i = 0; i < distance; i++)
		{
			pt[i] = b[i];
		}
		return pt;
}

char * processData(char a[1024])
{
	int count = 0;

	char *data = (char *)calloc(ArrayDataLength, sizeof(char));
	for (int i = 0; i < ArrayDataLength; i++)
	{
		*(data + i) = '\0';
	}

	for (int t = 0; t < ArrayDataLength; t++)
	{
			//С��58ȥ����ĸ��40��41Ϊ(),13��10ΪCR,LF
			if (a[t] < 58 && a[t] != 40 && a[t] != 41 && a[t] != 13 && a[t] != 35)
			{
				//���з�LF��ʾ��һ�����ݵĿ�ʼ,��"��ʾ
				if (a[t] == 10)//LF
				{
					*(data + count) = 34;//"
					//printf("%c", *(data + count));
				}
				//�ո��Ϊʱ������ݵļ������&����ʱ����������ڽ�һ������
				else if (a[t] == 32)//�ո�
				{
					*(data + count) = 45;//&
					//printf("%c", *(data + count));
				}
				else if (a[t] == 44)
				{
					*(data + count) = '.';//���Ż���С����
					//printf("%c", *(data + count));
				}
				//�ų����������Ϊ��Ч����
				else
				{
					*(data + count) = a[t];
					//printf("%c", *(data + count));
				}
				count = count + 1;
			}
	}
	return data;
}

//time����/��һλ��ʼ����"ǰһλ������ value:��"��һλ��ʼ����/ǰһλ����
int distinguishTime_Value(char *data)
{
	char * stat = (char*) calloc(dataSize, sizeof(char));
	for (int i = 0; i < dataSize; i++)
	{ 
		stat[i] = '\0';
	}
	for (int m = 0; m < dataSize; m++)
	{
		*(stat + m) = 0;
	}
	int countStat = 0;
	int markFinish = 1;
	for (int i = 0; i < dataSize; i++)
	{
		if (countTime == dataSize-1)
		{
			countTime = 0;
		}
		if (countValue == dataSize - 1 && markRealTime == 0)
		{
			for (int i = 0; i < dataSize; i++)
			{
				savedValue[i] = value[i];
				FFT3(value, 1024);
				countValue = 0;
				dataSize = 32;
				markRealTime = 1;
			}

		}
		//Every 1024 data would be put into a FFT
		if (countValue == dataSize-1 && markRealTime==1)
		{
			double * pt = moveLength(savedValue, value, dataSize, 32);
			for (int i = 0; i < dataSize; i++)
			{
				savedValue[i] = pt[i];
			}
			FFT3(pt, 1024);
			countValue = 0;
		}


		//Word processing
		if (i == dataSize-1 && *(data + i) != 45 && *(data + i) != 34 && *(data + i) != 13 && *(data + i) != 10 && markFinish == 0)
		{
			//printf("(%c)", *(data + i));
			if (*(data + i) != '\0')
			{
				//printf("+");
				//printf("%c", *(data + i));
				stat[countStat] = *(data + i);
				countStat = countStat + 1;
			}
			for (int n = 0; n < countStat; n++)
			{
				buffer[n] = stat[n];
				//printf("!%c", buffer[n]);
			}
				//printf(" stored to buffer\n");
			count = countStat;
		}
		else if (i == dataSize-1 && (*(data + i) == 45 || *(data + i) == 34))
		{
			if (*(data + i) == 45)
			{
				//
				time[countTime] = atof(stat);
				//printf("-Time: %f", time[countTime]);
				countTime = countTime + 1;
				//printf("timeCount:%d", countTime);
				for (int i = 0; i < dataSize; i++)
				{
					stat[i] = '\0';
				}
				countStat = 0;
				markFinish = 1;
			}
			//"�ű�ʾ��һ�п�ʼ
			else if (*(data + i) == 34)
			{
				value[countValue] = atof(stat) *5/4096;
				printf("-Value: %f\n", value[countValue]);
				countValue = countValue + 1;
				//printf("valueCount:%d\n", countValue);
				for (int i = 0; i < dataSize; i++)
				{
					stat[i] = '\0';
				}
				countStat = 0;
				markFinish = 1;
			}
		}
		else if (i == 0 && count!=0)
		{
			//printf("(%c)", *(data + i));
			for (int m = 0; m < count; m++)
			{
				//printf("(%c", buffer[m]);
				stat[m] = buffer[m];
				//printf(" picked ");
			}
			countStat = count;
			count = 0;
			for (int k = 0; k < dataSize; k++)
			{
				buffer[k] = '\0';
			}

			if (*(data + i) == 45)
			{
				//
				time[countTime] = atof(stat);
				//printf("-Time: %f", time[countTime]);
				countTime = countTime + 1;
				//printf("timeCount:%d", countTime);
				for (int i = 0; i < dataSize; i++)
				{
					stat[i] = '\0';
				}
				countStat = 0;
				markFinish = 1;
			}
			//"�ű�ʾ��һ�п�ʼ
			else if (*(data + i) == 34)
			{
				value[countValue] = atof(stat) *5/4096;
				printf("-Value: %f\n", value[countValue]);
				countValue = countValue + 1;
				//printf("valueCount:%d\n", countValue);
				for (int i = 0; i < dataSize; i++)
				{
					stat[i] = '\0';
				}
				countStat = 0;
				markFinish = 1;
			}
			else if (*(data + i) != 13 && *(data + i) != 10)
			{
				//printf("+");
				//printf("%c", *(data + i));
				stat[countStat] = *(data + i);
				countStat = countStat + 1;
				markFinish = 0;
			}
		}
		else if (*(data + i) != '\0')
		{
			if (*(data + i) != 13 && *(data + i) != 10)
			{
				//ʱ������ݵļ��
				if (*(data + i) == 45)
				{
					//countStatΪ0��û������¼��
						time[countTime] = atof(stat);
						//printf("-Time: %f", time[countTime]);
						countTime = countTime + 1;
						//printf("timeCount:%d", countTime);
						for (int i = 0; i < dataSize; i++)
						{
							stat[i] = '\0';
						}
						countStat = 0;
						markFinish = 1;
				}
				//"�ű�ʾ��һ�п�ʼ
				else if (*(data + i) == 34)
				{
						value[countValue] = atof(stat) * 5 / 4096;
						printf("-Value: %f\n", value[countValue]);
						countValue = countValue + 1;
						//printf("valueCount:%d\n", countValue);
						for (int i = 0; i < dataSize; i++)
						{
							stat[i] = '\0';
						}
						countStat = 0;
						markFinish = 1;
				}
				else
				{
					//printf("+");
					//printf("%c", *(data + i));
					stat[countStat] = *(data + i);
					countStat = countStat + 1;
					markFinish = 0;
				}
			}
		}
	}
	//printf("  -buffer end\n");
	return 0;
}