#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

typedef unsigned char BYTE;

struct bmp
	{
		char type[2];
		int size,reserved,head_end,header_size,weight,height;
		int end[7];
	};

struct bmp image[10];
FILE * file_input[10];
int i,j,z,number,captchasize;
int weight_captcha;
char captcha[30];

struct bmp fileRead(FILE* inputeFile);

char* nameCreate(char fileSymbol /*captcha[i]*/);
void fileWrite (struct bmp bmpImage, int captchaSize, BYTE ***rastrCaptcha, BYTE padArray[][20], int *padding, int captchaWeight);
int mainWork(int captchasize,struct bmp image[10], int *padding, FILE * file_input[10], BYTE pad_array[][20], BYTE ***rastr_captcha);

void mainWork2(int weight, int w, struct bmp image, BYTE ***rastr1, BYTE ***rastr2, BYTE ***rastr);
void fileWrite1(struct bmp bmpImage, int weight, BYTE ***rastr, BYTE pad[20], int padding, FILE * fout);

