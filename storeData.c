#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>
#include "math.h"
#include "signalProcessing.h"

//Array for storing time data. It won't be used in FFT, but further application can be made if necessary.
double time[1024];
//Array for storing signal value for FFT.
double value[1024];
//Process deals 1024 characters at first, then replace the array characters 32 elements each time. This is for storing the original data
double savedValue[1024];

//Size of the data processed by program each time.
int dataSize = 1024;
//For storing the value or time temporarily.
char buffer[1024];
int count;
//Count number of time elements stored in array
int countTime = 0;
//Count number of value elements stored in array
int countValue = 0;
//Check whether it's the first loop(datasize=1024) or not.(datasize=32)
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
			pt[i] = b[distance-1-i];
		}
		return pt;
}

//This function gets the incoming original data from device and remove or replace all unnecessary characters for further process.
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
			//smaller than 58 to eliminate alphabet，40 and 41 means(),13 and 10 are CR,LF
			if (a[t] < 58 && a[t] != 40 && a[t] != 41 && a[t] != 13 && a[t] != 35)
			{
				//LF signs a new line, replace it with " for further process 
				if (a[t] == 10)//LF
				{
					*(data + count) = 34;//"
					//printf("%c", *(data + count));
				}
				//Spacebar seperates time and value, replace it with "&" for further process
				else if (a[t] == 32)//空格
				{
					*(data + count) = 45;//&
					//printf("%c", *(data + count));
				}
				else if (a[t] == 44)
				{
					*(data + count) = '.';//Replace , with .
					//printf("%c", *(data + count));
				}
				//Exceptions are claimed, the rest of characters are numbers
				else
				{
					*(data + count) = a[t];
					//printf("%c", *(data + count));
				}
				count ++;
			}
	}
	return data;
}

//This function gets the plain text from processData() and store time & value.
int distinguishTime_Value(char *data)
{
	//Storing data temporarily.
	char * stat = (char*) calloc(dataSize, sizeof(char));
	//initialize the array
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
	//Process the text
	for (int i = 0; i < 1024; i++)
	{
		if (countTime == dataSize-1)
		{
			countTime = 0;
		}
		if (countValue == dataSize - 1 && markRealTime == 0)
		{
			for (int t = 0; t < dataSize; t++)
			{
				savedValue[t] = value[t];
			}
			countValue = 0;
			dataSize = 32;
			markRealTime = 1;
			FFT3(value, 1024);
		}
		//Every 1024 data would be put into a FFT
		if (countValue == dataSize-1 && markRealTime==1)
		{
			double * pt = moveLength(savedValue, value, 1024, 32);
			for (int t = 0; t < dataSize; t++)
			{
				savedValue[t] = pt[t];
			}
			FFT3(pt, 1024);
			countValue = 0;
		}


		//Word processing
		if (i == 1023 && *(data + i) != 45 && *(data + i) != 34 && *(data + i) != 13 && *(data + i) != 10 && markFinish == 0)
		{
			//printf("(%c)", *(data + i));
			if (*(data + i) != '\0')
			{
				//printf("+");
				//printf("%c", *(data + i));
				stat[countStat] = *(data + i);
				countStat++;
			}
			for (int n = 0; n < countStat; n++)
			{
				buffer[n] = stat[n];
				//printf("!%c", buffer[n]);
			}
				//printf(" stored to buffer\n");
			count = countStat;
		}
		else if (i == 1023 && (*(data + i) == 45 || *(data + i) == 34))
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
			//"means a new line
			else if (*(data + i) == 34)
			{
				value[countValue] = atof(stat) *5/4096;
				//printf("%f\n", value[countValue]);
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
			// "means a new line
			else if (*(data + i) == 34)
			{
				value[countValue] = atof(stat) *5/4096;
				//printf("%f\n", value[countValue]);
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
				//Gap between time and value
				if (*(data + i) == 45)
				{
					//countStat为0则没有数据录入
						time[countTime] = atof(stat);
						//printf("%f", time[countTime]);
						countTime = countTime + 1;
						//printf("timeCount:%d", countTime);
						for (int i = 0; i < dataSize; i++)
						{
							stat[i] = '\0';
						}
						countStat = 0;
						markFinish = 1;
				}
				//" means a new line
				else if (*(data + i) == 34)
				{
						value[countValue] = atof(stat) * 5 / 4096;
						//printf("%f\n", value[countValue]);
						countValue = countValue + 1;
						//printf("valueCount:%d\n", countValue);
						for (int i = 0; i < dataSize; i++)
						{
							stat[i] = '\0';
						}
						countStat = 0;
						markFinish = 1;
				}
				// Count number into buffer
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

	free(data);
	free(stat);
	data = NULL;
	stat = NULL;
	return 0;
}