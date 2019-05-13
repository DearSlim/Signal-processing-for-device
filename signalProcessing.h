#define PI 3.1415
#define log2N  6  //log2N=12#pragma once
#define N 64
#define true 1
#define false 0
#define ArrayDataLength 1024
typedef struct {
	double real;
	double img;
} col;

//This array is for test
col x[64];

//This structure is for transmitting both the array and its length.
typedef struct {
	double * doublee ;
	col * coll;
	int length;
}ary;