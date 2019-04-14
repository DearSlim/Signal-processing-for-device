#include "signalProcessing.h"
#include <stdio.h>
#include <math.h>

col calculateWN(int k, int n)
{
	col c;
	c.real = cos(2 * PI*k / n);
	c.img = -sin(2 * PI*k / n);
	return c;
}

/*�����ӷ�*/
col add(col a, col b)
{
	col c;
	c.real = a.real + b.real;
	c.img = a.img + b.img;
	return c;
}

/*��������*/
col sub(col a, col b)
{
	col c;
	c.real = a.real - b.real;
	c.img = a.img - b.img;
	return c;
}

/*�����˷�*/
col mul(col a, col b)
{
	col c;
	c.real = a.real*b.real - a.img*b.img;
	c.img = a.real*b.img + a.img*b.real;
	return c;
}


int FFT()
{
	//��ż���飬x1�洢����λ��x2�洢ż��λ
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
	half = N / 2;                   //��ַ���㣬������Ȼ˳���ɵ�λ�򣬲����׵��㷨
	m = N - 1;
	for (int i = 0; i < m; i++)
	{
		if (i < j)                      //���i<j,�����б�ַ
		{
			t = x[j];
			x[j] = x[i];
			x[i] = t;
		}
		int k = half;                       //��j����һ����λ��
		while (k <= j)                 //���k<=j,��ʾj�����λΪ1   
		{
			j = j - k;                 //�����λ���0
			k = k / 2;                 //k/2���Ƚϴθ�λ���������ƣ�����Ƚϣ�ֱ��ĳ��λΪ0
		}
		j = j + k;                     //��0��Ϊ1
	}
	/*for (int i = 0; i < 64; i++)
	{
		printf("%f\n", x[i].real);
	}*/
	return 0;
}

//real time use
double * reverse1(double value[], int length)
{
	int half, m, p, j = 0;
	double t;
	half = length / 2;                   //��ַ���㣬������Ȼ˳���ɵ�λ�򣬲����׵��㷨
	m = length - 1;
	for (int p = 0; p < m; p++)
	{
		if (p < j)                      //���i<j,�����б�ַ
		{
			t = value[j];
			value[j] = value[p];
			value[p] = t;
		}
		int k = half;                       //��j����һ����λ��
		while (k <= j)                 //���k<=j,��ʾj�����λΪ1   
		{
			j = j - k;                 //�����λ���0
			k = k / 2;                 //k/2���Ƚϴθ�λ���������ƣ�����Ƚϣ�ֱ��ĳ��λΪ0
		}
		j = j + k;                     //��0��Ϊ1
	}
	for (int p = 0; p < 64; p++)
	{
		printf("%f\n", value[p]);
	}
	return value;
}


//����2��DFT!
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
col * dft1(double value[],int length)
{
	col *result = (col*)malloc(length * sizeof(col));
	col dftX[2] = { 0 };
	col y[2];
	for (int j = 0; j < length/2; j++)
	{
		y[0].real =value[2 * j];
		y[1].real =value[2*j+1];
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
	//�����һ����2��DFT
	dft();
	//����һ����ͬ��С���������ڴ��x[]ԭʼ����
	for (int i = 0; i < 64; i++)
	{
		xtemp[i] = x[i];
	}
	//һ��log2N������
	int mark[64];
	for (int m = 1; m < log2N; m++)
	{
		reset(mark, 64, 1);
		//��¼ÿ�����ε���ת���ӱ仯���ڣ���L����������ΪL��
		int markLoop = 0;
		//�ڵ����
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
	calculateKORREL(amplitude, d1, 64);


	for (int i = 0; i < 64; i++)
	{
		printf("%f\n", /**frequency[i]**/amplitude[i]);
	}
	return 0;
}

//For realtime processing
int FFT3(double value[], int length)
{
	col *fftResult = (col*)malloc(length * sizeof(col));
	col *xtemp = (col*)malloc(length*sizeof(col));
	double *pt = reverse1(value, length);
	for (int i = 0; i < length; i++)
	{
		value[i] = *(pt + i);
	}
	//�����һ����2��DFT
	col *pt1 = dft1(value, length);
	for (int i = 0; i < length; i++)
	{
		fftResult[i] = *(pt1 + i);
	};
	//����һ����ͬ��С���������ڴ��x[]ԭʼ����
	for (int i = 0; i < length; i++)
	{
		xtemp[i].real = fftResult[i].real;
		xtemp[i].img = fftResult[i].img;
	}
	//һ��log2N������
	int *mark = (int*)malloc(length * sizeof(int));
	for (int m = 1; m < log(length)/log(2); m++)
	{
		reset(mark, length, 1);
		//��¼ÿ�����ε���ת���ӱ仯���ڣ���L����������ΪL��
		int markLoop = 0;
		//�ڵ����
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
					//col wnk = calculateWN((p*pow(2,(log2N-m+1))), N);
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
		for (int i = 0; i < length; i++)
		{
			//printf("%f+j%f\n", x[i].real, x[i].img);
			xtemp[i].real = fftResult[i].real;
			xtemp[i].img = fftResult[i].img;
		}
	}


	//Transform the result into frequency and amplitude
	double * frequency = (double*)malloc(length*sizeof(double));
	double * amplitude = (double*)malloc(length*sizeof(double));
	if (frequency == NULL || amplitude == NULL)
	{
		exit(1);
	}
	//memset(frequency, 0, length*sizeof(double));
	//memset(amplitude, 0, length * sizeof(double));

	for (int i = 0; i < length; i++)
	{
		//Frequency calculation
		double i1 = i;
		frequency[i] = 20 * i1 / length;
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
	for (int i = 0; i < length; i++)
	{
		printf("%f     %f\n", frequency[i], amplitude[i]);
	}
	printf("||||||||||Process end\n");
	//Release resourse
	//free(frequency);
	//free(amplitude);
	//frequency = 0;
	//amplitude = 0;
	return 0;
} 