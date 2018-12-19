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

char* nameCreate(char fileSymbol /*captcha[i]*/) 
{
 /* Создание названия картинки */
 char *fileName = (char*)malloc(6*sizeof(char));
 
 fileName[5]='\0';
 
 fileName[0]=fileSymbol;
 strcat(fileName,".bmp");
 
 return fileName;
}

 struct bmp fileRead(FILE* inputeFile)
{
  /* Чтение "хедера" бмп */
  
  struct bmp bmpImage;
  
  		fread(bmpImage.type, 2, 1, inputeFile);
		fread(&bmpImage.size, 4, 1,inputeFile);
		fread(&bmpImage.reserved, 4, 1, inputeFile);
		fread(&bmpImage.head_end, 4, 1, inputeFile);
		fread(&bmpImage.header_size, 4, 1, inputeFile);
		fread(&bmpImage.weight, 4, 1, inputeFile);
		fread(&bmpImage.height, 4, 1, inputeFile);
		fread(bmpImage.end, 4, 7, inputeFile);
		
 return bmpImage; /*image[i]*/
}

void fileWrite (struct bmp bmpImage, int captchaSize, BYTE ***rastrCaptcha, BYTE padArray[][20], int *padding, int captchaWeight)  
{	

	/*записывает результат в результирующий файл*/
	
	 FILE* outputFile = fopen("captcha.bmp","w");
	 int i,j;
	 
  	 fwrite(bmpImage.type, 2, 1, outputFile);   
	 fwrite(&bmpImage.size, 4, 1, outputFile);
	 fwrite(&bmpImage.reserved, 4, 1, outputFile);
	 fwrite(&bmpImage.head_end, 4, 1, outputFile);
	 fwrite(&bmpImage.header_size, 4, 1, outputFile);
	 fwrite(&captchaWeight, 4, 1, outputFile);
	 fwrite(&bmpImage.height, 4, 1, outputFile);
	 fwrite(bmpImage.end, 4, 7, outputFile);
	 for(i=0;i<bmpImage.height; i++)
	   {
		    for (j=0;j<bmpImage.weight*captchaSize; j++)
		    fwrite(rastrCaptcha[i][j], 3, 1, outputFile);
		    fwrite(padArray[0], padding[0], 1, outputFile);
	   }
	   
	 fclose(outputFile);
}

int mainWork(int captchasize,struct bmp image[10], int *padding, FILE * file_input[10], BYTE pad_array[][20], BYTE ***rastr_captcha)
{
int i,j,z,number;
int weight_captcha=0;

for(number=0;number<captchasize;number++)
	{
		BYTE ***rastr=(BYTE***)malloc(image[0].height*sizeof(BYTE**));
		for(i=0;i<image[0].height;i++)
		{
			rastr[i]=(BYTE**)malloc(image[0].weight*sizeof(BYTE*));
			for(j=0;j<image[0].weight;j++)
			{
				rastr[i][j]=(BYTE*)malloc(3*sizeof(BYTE));
			}
		}
		
		padding[number] = (image[0].size - image[0].head_end - image[0].height*image[number].weight * 3) / image[number].height;
		
		for(i=0;i<image[0].height;i++)
		{
			for(j=0;j<image[number].weight;j++)
			{
				fread(rastr[i][j], 3, 1, file_input[number]);
			}
		fread(pad_array, padding[number], 1, file_input[number]);
		}

		for(i=0;i<image[0].height;i++)
		{
			for(j=weight_captcha;j<(image[0].weight+image[0].weight*number);j++)
			{
			for(z=0;z<3;z++)
				{
					rastr_captcha[i][j][z] = rastr[i][j-image[0].weight*number][z];
				}
			}
		}
		
		weight_captcha+=image[0].weight;
		
		for(i=0;i<image[0].height;i++)
		{
			for(j=0;j<image[0].weight;j++)
			{
				free(rastr[i][j]);
			}
			free(rastr[j]);
		}
		free(rastr);
	}
return weight_captcha;
}

int main()
{	
	struct bmp image[10];
	FILE * file_input[10];
	int i,j,z,number,captchasize;
	int weight_captcha;
	char captcha[30];

	//запрос капчи
	printf("Enter the captcha:");
	scanf("%s",captcha);
	captchasize=strlen(captcha);

	//открытие и чтение файлов
	for(i=0;i<captchasize;i++)
	{
		file_input[i] = fopen(nameCreate(captcha[i]),"r");
		perror("Input file");

		image[i] = fileRead(file_input[i]);
	}
	
	//sozdanie massivov
	int *padding = (int*)malloc(sizeof(int)*captchasize);
	BYTE pad_array[captchasize][20]; 
	
	BYTE ***rastr_captcha=(BYTE***)malloc(image[0].height*sizeof(BYTE**));

	for(i=0;i<image[0].height;i++)
	{
		rastr_captcha[i]=(BYTE**)malloc((image[0].weight*captchasize)*sizeof(BYTE*));
		for(j=0;j<image[0].weight*captchasize;j++)
		{
			rastr_captcha[i][j]=(BYTE*)malloc(3*sizeof(BYTE));
		}
	}

	//osnovnoi cycle
	weight_captcha = mainWork(captchasize,image, padding, file_input, pad_array, rastr_captcha);

	//запись в файл
	fileWrite (image[0], captchasize, rastr_captcha, pad_array, padding, weight_captcha);

	// освобождение памяти
	free(padding);	
	for(i=0; i<image[0].height; i++)
	{
		for(j=0; j<weight_captcha; j++)	
		{
			free(rastr_captcha[i][j]);
		}
		free(rastr_captcha[i]);
	}
	free(rastr_captcha[i][j]);
	
	return 0;
}
