#include <stdio.h>
#include <math.h>
#include "signalProcessing.h"

//产生方波数据
int setRectangularWave()
{
	bool mark = true;
	int count = 0;
	for (int i = 0; i < 64; i++)
	{
		if (count == 5)
		{
			mark = !mark;
			count = 0;
		}
		if (mark == true)
		{
			x[i].real = 5;
			count++;
		}
		else if (mark == false)
		{
			x[i].real = 0;
			count++;
		}
		x[i].img = 0;
		//printf("%f+j%f\n", x[i].real, x[i].img);
	}
	return 0;
}

//产生脉冲函数数据
int setPulseWave()
{
	x[0].real = 1;
	x[0].img = 0;
	for (int i = 1; i < 64; i++)
	{
		x[i].real = 0;
		x[i].img = 0;
	}
	return 0;
}

//产生三角波数据
int setTriangleWave()
{
	x[0].real = 0;
	int mark = 1;
	for (int i = 1; i < 64; i++)
	{
		if (i > 0 && x[i - 1].real == 5)
		{
			mark = -1;
		}
		else if (i > 0 && x[i - 1].real == 0)
		{
			mark = 1;
		}
		x[i].real = x[i - 1].real + mark;
		printf("%f\n", x[i].real);
	}
	for (int i = 0; i < 64; i++)
	{
		x[i].img = 0;
	}
	//for (int i = 0; i < 64; i++)
	//{
	//	printf("%f+j%f\n", x[i].real, x[i].img);
	//}
	return 0;
}


//Creat sin wave
void setSin()
{
	for (int i = 0; i < 64; i++)
	{
		x[i].real = sin(2*PI*5*0.05*i);
		x[i].img = 0;
	}
	
}

//Create a signal for corresponding test of coefficient
double * setTest()
{
	double * testArray = (double *)malloc(64 * sizeof(double));
	for (int i = 0; i < 64; i++)
	{
		testArray[i] = 0;
	}
	testArray[16] =testArray[48]= 1;
	return testArray;
}

//Create a array combined with noise for corresponding test of coefficient
double * setTest1()
{
	double * testArray = (double *)malloc(64 * sizeof(double));
	double f[64] = {0.098637456,
	0.029500652,
	0.094357695,
	0.129380317,
	0.165378737,
	0.18366426,
	0.14893466,
	0.022136653,
	0.126346583,
	0.131214481,
	0.144001099,
	0.179599972,
	0.027914314,
	0.049723573,
	0.128163523,
	0.13952503,
	1.184083785,
	0.011598286,
	0.018705145,
	0.021947573,
	0.121982194,
	0.031212536,
	0.041788402,
	0.095570213,
	0.175805198,
	0.102893555,
	0.011716358,
	0.093698182,
	0.107763802,
	0.086455655,
	0.100133362,
	0.157348852,
	0.181513112,
	0.050651883,
	0.043182906,
	0.059767073,
	0.026643288,
	0.160804325,
	0.103017002,
	0.016730334,
	0.115313695,
	0.074480089,
	0.082940938,
	0.031338249,
	0.018581717,
	0.131756304,
	0.082287669,
	0.081855058,
	1.014466837,
	0.188605839,
	0.109114987,
	0.120433959,
	0.153060777,
	0.160264496,
	0.054701578,
	0.196516527,
	0.062373435,
	0.100281002,
	0.005048123,
	0.157321713,
	0.074793542,
	0.143329967,
	0.011773381,
	0.17460074};
	for (int i = 0; i < 64; i++)
	{
		testArray[i] = f[i];
	}
	return testArray;
}

