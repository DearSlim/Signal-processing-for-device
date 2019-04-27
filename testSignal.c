#include <stdio.h>
#include <math.h>
#include "signalProcessing.h"
#include <stdbool.h>
#include <stdlib.h>

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

//Create a signal for corresponding test of coefficient.
double * setTest()
{
	double * testArray = (double *)calloc(64, sizeof(double));
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
	0.17460074 };
	for (int i = 0; i < 64; i++)
	{
		testArray[i] = f[i];
	}
	return testArray;
}

double * Variaresp_AB_frequency()
{
	double f[104] = { 0,
0.00976563,
0.0195313,
0.0292969,
0.0390625,
0.0488281,
0.0585938,
0.0683594,
0.078125,
0.0878906,
0.0976563,
0.107422,
0.117188,
0.126953,
0.136719,
0.146484,
0.15625,
0.166016,
0.175781,
0.185547,
0.195313,
0.205078,
0.214844,
0.224609,
0.234375,
0.244141,
0.253906,
0.263672,
0.273438,
0.283203,
0.292969,
0.302734,
0.3125,
0.322266,
0.332031,
0.341797,
0.351563,
0.361328,
0.371094,
0.380859,
0.390625,
0.400391,
0.410156,
0.419922,
0.429688,
0.439453,
0.449219,
0.458984,
0.46875,
0.478516,
0.488281,
0.498047,
0.507813,
0.517578,
0.527344,
0.537109,
0.546875,
0.556641,
0.566406,
0.576172,
0.585938,
0.595703,
0.605469,
0.615234,
0.625,
0.634766,
0.644531,
0.654297,
0.664063,
0.673828,
0.683594,
0.693359,
0.703125,
0.712891,
0.722656,
0.732422,
0.742188,
0.751953,
0.761719,
0.771484,
0.78125,
0.791016,
0.800781,
0.810547,
0.820313,
0.830078,
0.839844,
0.849609,
0.859375,
0.869141,
0.878906,
0.888672,
0.898438,
0.908203,
0.917969,
0.927734,
0.9375,
0.947266,
0.957031,
0.966797,
0.976563,
0.986328,
0.996094,
1.00586 };
return f;
}
//Collected data for calculation of correlation coefficient, corresponding frequency ranges from 0-1hz
double * Variaresp_AB_effort()
{
	double * testArray = (double *)malloc(104 * sizeof(double));
	double f[104] = { 4.07388230519528,
7.78929835244811,
8.23985462697529,
6.35535572218109,
6.45856172349366,
9.3018870596552,
8.6413686512547,
6.52436524914258,
3.07404506804321,
4.37242312365379,
5.85478721619077,
5.52425641725022,
7.11921592926431,
10.1749477519618,
8.4111339099807,
4.65876157842327,
7.33266999551287,
8.37990051484663,
6.46731095405606,
4.07780102930527,
3.84025262966757,
6.51548022121004,
8.81681885348608,
11.3472670488263,
18.0961635496967,
18.4912562983305,
20.3247923900706,
23.7490200764772,
22.8587713170347,
32.847831868888,
37.2641563423483,
33.8077252795922,
41.3580590598282,
27.1664578943764,
36.0458599358765,
43.3816338712784,
20.4390561772381,
19.451421303775,
16.6686421950753,
8.93428357866422,
10.414610710649,
10.5276135222365,
12.3885612831222,
6.64464291984521,
6.94902302522009,
5.16741972774955,
4.18628372842932,
4.35275190460661,
4.21996147622605,
2.80557366726036,
3.51495972928988,
2.65031847393243,
2.46968857201111,
3.54400096687728,
4.63197069725548,
4.3826079264149,
4.12015040916718,
3.42389789279341,
3.11018656812692,
3.60761233497952,
4.08012898422209,
3.8298738306634,
3.46756644710818,
3.36686299733119,
4.25726695376817,
4.95105571785497,
4.36053115395367,
4.76476112563604,
3.80599289314163,
4.64419246056881,
3.02001711434856,
1.94042802167093,
1.34490223462332,
0.954153061871804,
1.42601982370009,
1.66598347656399,
2.19078017335116,
2.32663574237221,
1.6205185170384,
2.83154976420727,
2.35177765547392,
1.97152561943485,
1.67712080087857,
1.89851125343107,
1.69216520952855,
1.46775229550905,
1.57657448805846,
2.07381983840501,
2.51609247335316,
1.56756336256792,
1.53053723961582,
1.61239589434111,
1.4039954302245,
1.67528171649428,
1.05849006128147,
1.05578187372823,
1.431956108738,
1.79728789372265,
2.11304587875351,
1.88797919739486,
1.94116520739459,
1.71955165916257,
1.24961321999038,
1.14094428447299 };
	for (int i = 0; i < 104; i++)
	{
		testArray[i] = f[i];
	}
	return testArray;
}
