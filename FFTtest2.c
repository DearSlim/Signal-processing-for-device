#include "signalProcessing.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

col calculateWN(int k, int n)
{
	col c;
	c.real = cos(2 * PI*k / n);
	c.img = -sin(2 * PI*k / n);
	return c;
}

/*Complex addition*/
col add(col a, col b)
{
	col c;
	c.real = a.real + b.real;
	c.img = a.img + b.img;
	return c;
}

/*Complex subtraction*/
col sub(col a, col b)
{
	col c;
	c.real = a.real - b.real;
	c.img = a.img - b.img;
	return c;
}

/*Complex multiplication*/
col mul(col a, col b)
{
	col c;
	c.real = a.real*b.real - a.img*b.img;
	c.img = a.real*b.img + a.img*b.real;
	return c;
}


int FFT()
{
	//奇偶分组，x1存储奇数位，x2存储偶数位
	col x1[32], x2[32];
	col y, z;
	y.real = 0; y.img = 0;
	z.real = 0; z.img = 0;

		for (int k = 0; k < N / 2; k++)
		{
			col WNk = calculateWN(k, N);
			for (int r = 0; r < N / 2; r++)
			{
				col temWNk = calculateWN((r*k), (N / 2));
				y = add((mul(x[2 * r], temWNk)), y);
				z = add((mul(x[2 * r + 1], temWNk)), z);
			}
			x1[k] = y;
			x2[k] = mul(WNk, z);
			y.real = 0; y.img = 0;
			z.real = 0; z.img = 0;
		}
		for (int i = 0; i < N / 2; i++)
		{
			//printf("real:%f---img:%f\n",x1[i].real,x1[i].img);
			//printf("real:%f---img:%f\n", x2[i].real, x2[i].img);
			x[i] = add(x1[i], x2[i]);
			printf("%f+j%f\n", x[i].real, x[i].img);
		}
		//printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
		for (int i = 32; i < N; i++)
		{
			//printf("real:%f---img:%f\n", x1[i - 32].real, x1[i - 32].img);
			//printf("real:%f---img:%f\n", x2[i - 32].real, x2[i - 32].img);
			x[i] = sub(x1[i - 32], x2[i - 32]);
			printf("%f+j%f\n", x[i].real, x[i].img);
		}

	return 0;
}


int reverse()
{
	int half, m, i, j=0;
	col t;
	half = N / 2;                   //变址运算，即把自然顺序变成倒位序，采用雷德算法
	m = N - 1;
	for (int i = 0; i < m; i++)
	{
		if (i < j)                      //如果i<j,即进行变址
		{
			t = x[j];
			x[j] = x[i];
			x[i] = t;
		}
		int k = half;                       //求j的下一个倒位序
		while (k <= j)                 //如果k<=j,表示j的最高位为1   
		{
			j = j - k;                 //把最高位变成0
			k = k / 2;                 //k/2，比较次高位，依次类推，逐个比较，直到某个位为0
		}
		j = j + k;                     //把0改为1
	}
	/*for (int i = 0; i < 64; i++)
	{
		printf("%f\n", x[i].real);
	}*/
	return 0;
}

//real time use
double * reverse1(double original[], int length)
{
	int half, m, j = 0;
	double t;
	half = length / 2;                   //Inverse function by rader algorithm
	m = length - 1;
	for (int p = 0; p < m; p++)
	{
		if (p < j)                      //Change address when i<j
		{
			t = original[j];
			original[j] = original[p];
			original[p] = t;
		}
		int k = half;                       //Next inverse sequence
		while (k <= j)                 //If k<=j,mark the highest position as 1   
		{
			j = j - k;                 //Change the highest position into 0
			k = k / 2;                 //k/2，比较次高位，依次类推，逐个比较，直到某个位为0
		}
		j = j + k;                     //把0改为1
	}
	for (int p = 0; p < 64; p++)
	{
		//printf("%f\n", value[p]);
	}
	return original;
}


//计算2点DFT
col * dft()
{
	col dftX[2] = { 0 };
	for (int j = 0; j < 32; j++)
	{
		col y[2] = { x[2 * j],x[2 * j + 1] }; 
		for (int k = 0; k < 2; k++)
		{
			col tempCol = { 0,0 };
			for (int n = 0; n < 2; n++)
			{
				col wnk = calculateWN((n*k), 2);
				tempCol = add(tempCol, mul(y[n], wnk));
			}
			dftX[k] = tempCol;
			tempCol.real = 0;
			tempCol.img = 0;
		}
		x[2 * j].real = dftX[0].real;
		x[2 * j].img = dftX[0].img;
		//printf("%f+j%f\n", x[2 * j].real, x[2 * j].img);
		x[2 * j + 1].real = dftX[1].real;
		x[2 * j + 1].img = dftX[1].img;
		//printf("%f+j%f\n", x[2 * j + 1].real, x[2 * j + 1].img);
	}
	
	return dftX;
}

//realtime processing
col * dft1(double original1[],int length)
{
	col *result = (col*)calloc(length, sizeof(col));
	col dftX[2] = { 0 };
	col y[2];
	for (int j = 0; j < length/2; j++)
	{
		y[0].real = original1[2 * j];
		y[1].real = original1[2*j+1];
		y[0].img = y[1].img = 0;
		for (int k = 0; k < 2; k++)
		{
			col tempCol = { 0,0 };
			for (int n = 0; n < 2; n++)
			{
				col wnk = calculateWN((n*k), 2);
				tempCol = add(tempCol, mul(y[n], wnk));
			}
			dftX[k] = tempCol;
			tempCol.real = 0;
			tempCol.img = 0;
		}
		result[2 * j].real = dftX[0].real;
		result[2 * j].img = dftX[0].img;
		//printf("%f+j%f\n", x[2 * j].real, x[2 * j].img);
		result[2 * j + 1].real = dftX[1].real;
		result[2 * j + 1].img = dftX[1].img;
		//printf("%f+j%f\n", x[2 * j + 1].real, x[2 * j + 1].img);
	}
	return result;
}

void reset(int a[], int length, int b)
{
	for (int i = 0; i < length; i++)
	{
		a[i] = b;
	}
}

int FFT2()
{
	//for(int i=0;i<)
	//printf("%d\n", sizeof(x));
	col xtemp[64];
	reverse(64);
	//计算第一级的2点DFT
	dft();
	//设置一个相同大小的数组用于存放x[]原始数据
	for (int i = 0; i < 64; i++)
	{
		xtemp[i] = x[i];
	}
	//一共log2N级运算
	int mark[64];
	for (int m = 1; m < log2N; m++)
	{
		reset(mark, 64, 1);
		//记录每级蝶形的旋转因子变化周期，第L级运算周期为L；
		int markLoop = 0;
		//节点距离
		int distance = pow(2, m);
		for (int p = 0; p < 64; p++)
		{
			if (p == 0)
			{
				markLoop = 0;
			}
			if (mark[p] == 1)
			{
				if (markLoop < distance )
				{
					//col wnk = calculateWN((p*pow(2,(log2N-m+1))), N);
					double k = markLoop * N / pow(2, (m + 1));
					col wnk = calculateWN(k, N);
					x[p] = add(xtemp[p], mul(wnk, xtemp[p + distance]));
					x[p + distance] = sub(xtemp[p], mul(wnk, xtemp[p + distance]));
					mark[p + distance] = 0;
					markLoop++;
				}
				else
				{
					markLoop = 0;
					double k = markLoop * N / pow(2, (m + 1));
					col wnk = calculateWN(k, N);
					x[p] = add(xtemp[p], mul(wnk, xtemp[p + distance]));
					x[p + distance] = sub(xtemp[p], mul(wnk, xtemp[p + distance]));
					mark[p + distance] = 0;
					markLoop++;
				}
			}
		}
		reset(mark, 64, 1);
		for (int i = 0; i < 64; i++)
		{
			//printf("%f+j%f\n", x[i].real, x[i].img);
			xtemp[i].real = x[i].real;
			xtemp[i].img = x[i].img;
		}
	}

	//Transform the result into frequency and amplitude
	double frequency[64] = { 0 };
	double amplitude[64] = { 0 };

	for (int i = 0; i < 64; i++)
	{
		//Frequency calculation
		double i1 = i;
		frequency[i] = 20 * i1 / 64;
		//Amplitude calculation
		if (i == 0 || i == 31)
		{
			double a = pow(x[i].real, 2) + pow(x[i].img, 2);
			amplitude[i] = sqrt(a) / 64;
		}
		else
		{
			double a = pow(x[i].real, 2) + pow(x[i].img, 2);
			amplitude[i] = 2 * sqrt(a) / 64;
		}
	}

	//Test correlation coefficient
	double * d1 = setTest1();
	calculateCORREL(amplitude, d1, 64);

	//Display result
	for (int i = 0; i < 64; i++)
	{
		printf("%f\n", /**frequency[i]**/amplitude[i]);
	}
	return 0;
}

//For realtime processing
int FFT3(double value[], int length)
{
	//For storing results
	col *fftResult = (col*)calloc(length , sizeof(col));
	//For doing calculation
	col *xtemp = (col*)calloc(length, sizeof(col));
	//Make reverse sequence
	double * pt = reverse1(value, length);
	for (int i = 0; i < length; i++)
	{
		value[i] = pt[i];
	}
	//2 points DFT
	col *pt1 = dft1(value, length);
	for (int i = 0; i < length; i++)
	{
		fftResult[i] = pt1[i];
	};
	//xtemp is for storing the data of the last series of calculation temporarily.
	for (int i = 0; i < length; i++)
	{
		xtemp[i].real = fftResult[i].real;
		xtemp[i].img = fftResult[i].img;
	}
	//log2N level of calculations in total
	int *mark = (int*)calloc(length, sizeof(int));
	for (int m = 1; m < log(length)/log(2); m++)
	{
		reset(mark, length, 1);
		//Mrark the loop of twirl factor
		int markLoop = 0;
		//Distance between nodes
		int distance = pow(2, m);
		for (int p = 0; p < length; p++)
		{
			if (p == 0)
			{
				markLoop = 0;
			}
			if (mark[p] == 1)
			{
				if (markLoop < distance)
				{
					double k = markLoop * length / pow(2, (m + 1));
					col wnk = calculateWN(k, length);
					fftResult[p] = add(xtemp[p], mul(wnk, xtemp[p + distance]));
					fftResult[p + distance] = sub(xtemp[p], mul(wnk, xtemp[p + distance]));
					mark[p + distance] = 0;
					markLoop++;
				}
				else
				{
					markLoop = 0;
					double k = markLoop * length / pow(2, (m + 1));
					col wnk = calculateWN(k, length);
					fftResult[p] = add(xtemp[p], mul(wnk, xtemp[p + distance]));
					fftResult[p + distance] = sub(xtemp[p], mul(wnk, xtemp[p + distance]));
					mark[p + distance] = 0;
					markLoop++;
				}
			}
		}
		reset(mark, length, 1);
		//Temporarily store the results into xtemp
		for (int i = 0; i < length; i++)
		{
			//printf("%f+j%f\n", x[i].real, x[i].img);
			xtemp[i].real = fftResult[i].real;
			xtemp[i].img = fftResult[i].img;
		}
	}


	//Transform the result into frequency and amplitude
	double * frequency = (double*)calloc(length, sizeof(double));
	double * amplitude = (double*)calloc(length, sizeof(double));
	if (frequency == NULL || amplitude == NULL)
	{
		exit(1);
	}

	for (int i = 0; i < length; i++)
	{
		//Frequency calculation
		double i1 = i;
		*(frequency+i) = 20 * i1 / length;
		//Amplitude calculation
		if (i == 0 || i == 31)
		{
			double a = pow(fftResult[i].real, 2) + pow(fftResult[i].img, 2);
			amplitude[i] = sqrt(a) / length;
		}
		else
		{
			double a = pow(fftResult[i].real, 2) + pow(fftResult[i].img, 2);
			amplitude[i] = 2 * sqrt(a) / length;
		}
	}


	////Display FFT result 
	//for (int j = 0; j < length; j++)
	//{
	//	printf("%f     %f\n", frequency[j], amplitude[j]);
	//}


	/**Correlation coefficient calculation,
	if the signal wave shows a correlation coefficient with the monitored signal of 0.7 or larger, 
	a warning message would occur.**/
	double * ideal = Variaresp_AB_effort();
	double * amplitude1 = processAmp(amplitude, length);
	double a2[52];
	double a3[52];
	for (int i = 0; i < 52; i++)
	{
		a2[i] = amplitude1[i];
		a3[i] = ideal[2 * i + 1];
	}
	double CorrelResult1 = calculateCORREL(a2,a3,52);

	*ideal = AS_recupEffort();
	for (int i = 0; i < 52; i++)
	{
		a3[i] = ideal[2 * i + 1];
	}
	double CorrelResult2 = calculateCORREL(a2, a3, 52);

	*ideal = AB_recupSans();
	for (int i = 0; i < 52; i++)
	{
		a3[i] = ideal[2 * i + 1];
	}
	double CorrelResult3 = calculateCORREL(a2, a3, 52);

	*ideal = AS_reposSans();
	for (int i = 0; i < 52; i++)
	{
		a3[i] = ideal[2 * i + 1];
	}
	double CorrelResult4 = calculateCORREL(a2, a3, 52);

	*ideal = JF_effort_sans();
	for (int i = 0; i < 52; i++)
	{
		a3[i] = ideal[2 * i + 1];
	}
	double CorrelResult5 = calculateCORREL(a2, a3, 52);

	*ideal = JF_repos_sans();
	for (int i = 0; i < 52; i++)
	{
		a3[i] = ideal[2 * i + 1];
	}
	double CorrelResult6 = calculateCORREL(a2, a3, 52);


	////Insert reaction here
	//if (CorrelResult >= 0.7)
	//{
	//	printf("High correlation coefficient with ,  Correlation cofficient is: %s\n",CorrelResult);
	//}


	//Display result of processed data
	double hz[52];
	for (int i = 0; i < 52; i++)
	{
		double b = i;
		hz[i]=b * 20 / 1024;
	}
	for (int j = 0; j < 52; j++)
	{
		printf("%f     %f\n", hz[j], a2[j]);
	}


	//Release resourse
	free(mark);
	free(ideal);
	free(amplitude1);
	free(frequency);
	free(amplitude);
	free(pt1);
	free(xtemp);
	free(fftResult);
	mark = NULL;
	ideal = NULL;
	frequency = NULL;
	amplitude = NULL;
	amplitude1 = NULL;
	pt = NULL;
	pt1 = NULL;
	value = NULL;
	xtemp = NULL;
	fftResult = NULL;
	xtemp = NULL;
	return 0;
} 