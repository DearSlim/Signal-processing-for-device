#include <stdio.h>
#include <math.h>
#include "signalProcessing.h"

//Calculate the average of the input sequence   
double  calculateAVG(double a[], int length)
{
	double sum = 0;
	for (int i = 0; i < length; i++)
	{
		sum += a[i];
	}
	double avg = sum / length;
	return avg;
}

//Eliminate the difference on amplitude by replace the amplitudes with their weightings in total
double * processAmp(double a[],int length)
{
	double * b =calloc(length,sizeof(double));
	double SUM = 0;
	for (int i = 0; i < length; i++)
	{
		SUM += a[i];
	}
	for (int i = 0; i < length; i++)
	{
		b[i] = 1000 * a[i] / SUM;
	}
	return b;
}

/**Adaptation to the length and resolution of collected data
Frequency ranges from 0 to 1
dots of collected data: 2048
length1: length of the realtime data
length2: length of the ideal data**/
double * adapt_to_data(double data[], double f1[], double f2[], int length1, int length2)
{
	double * result = calloc(length2,sizeof(double));
	int countf2 = 0;
	if (length1 > length2)
	{
		for (int i = 0; i < length1; i++)
		{
			if (f1[i] < f2[countf2])
			{
				
			}
		}
	}
	else if (length1 == length2)
	{

	}
	else
	{

	}
}

//Calculate the coerelation coefficient of two series
double * calculateCORREL(double real[], double ideal[], int length)
{

	double avgReal = calculateAVG(real,length);
	double avgIdeal = calculateAVG(ideal, length);
	double a,b,c;
	a = b = c = 0;
	for (int i = 1; i < length; i++)
	{
		a += (real[i] - avgReal)*(ideal[i] - avgIdeal);
		b += pow(real[i] - avgReal, 2);
		c += pow(ideal[i] - avgIdeal, 2);
	}
	b = sqrt(b*c);
	double CORREL = fabs(a / b);
	printf("The correlation coefficient is: %f\n", CORREL);
	return &CORREL;
}

//Dynamic monitor for data
double algorithmTest1(double a[],int length)
{
	
}


//Calculate the absolute value of the input function
double calculateABS(col a)
{
	double re=0;


	return re;
}
