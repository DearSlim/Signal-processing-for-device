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

//Calculate the coerelation coefficient of two series
double calculateKORREL(double real[], double ideal[], int length)
{
	double avgReal = calculateAVG(real,length);
	double avgIdeal = calculateAVG(ideal, length);
	double a,b,c;
	a = b = c = 0;
	for (int i = 0; i < length; i++)
	{
		a += (real[i] - avgReal)*(ideal[i] - avgIdeal);
		b += pow(real[i] - avgReal, 2);
		c += pow(ideal[i] - avgIdeal, 2);
	}
	b = sqrt(b*c);
	double KORREL = fabs(a / b);
	printf("The correlation coefficient is: %f\n", KORREL);
	return KORREL;
}

//Calculate the absolute value of the input function
double calculateABS(col a)
{
	double re=0;


	return re;
}
