#include "stdio.h" 
#include "stdlib.h" 
#include "string.h" 

struct bmp fileRead(FILE * inputeFile)
{
	struct bmp bmpImage;
	fread(&bmpImage.type, 2, 1, inputeFile);
	fread(&bmpImage.size, 4, 1, inputeFile);
	fread(&bmpImage.reserved, 4, 1, inputeFile);
	fread(&bmpImage.head_end, 4, 1, inputeFile);
	fread(&bmpImage.header_size, 4, 1, inputeFile);
	fread(&bmpImage.weight, 4, 1, inputeFile);
	fread(&bmpImage.height, 4, 1, inputeFile);
	fread(bmpImage.end, 4, 7, inputeFile);
	return bmpImage;
}

void mainWork2(int weight, int w, struct bmp image, BYTE ***rastr1, BYTE ***rastr2, BYTE ***rastr)
{
	int k;
	printf("1)Соеденить справа 1->2");
	printf("2)Соеденить слева 2->1");
	scanf("%d", &k);
	if (k == 1)
		//Соединение картинок справа
		for (int i = 0; i < image.height; i++)
		{
			for (int j = 0; j < w; j++)
				for (int z = 0; z < 3; z++)
					rastr[i][j][z] = rastr1[i][j][z];
			for (int j = w; j < weight; j++)
				for (int z = 0; z < 3; z++)
					rastr[i][j][z] = rastr2[i][j - w][z];
		}
	else
		//Соединение картинок слева
		for (int i = 0; i < image.height; i++)
		{
			for (int j = 0; j < w; j++)
				for (int z = 0; z < 3; z++)
					rastr[i][j][z] = rastr2[i][j][z];
			for (int j = w; j < weight; j++)
				for (int z = 0; z < 3; z++)
					rastr[i][j][z] = rastr1[i][j - w][z];
		}

}

void fileWrite1(struct bmp bmpImage, int weight, BYTE ***rastr, BYTE pad[20], int padding, FILE * fout)
{

	int i, j;
	fwrite(bmpImage.type, 2, 1, fout);
	fwrite(&bmpImage.size, 4, 1, fout);
	fwrite(&bmpImage.reserved, 4, 1, fout);
	fwrite(&bmpImage.head_end, 4, 1, fout);
	fwrite(&bmpImage.header_size, 4, 1, fout);
	fwrite(&weight, 4, 1, fout);
	fwrite(&bmpImage.height, 4, 1, fout);
	fwrite(bmpImage.end, 4, 7, fout);
	for (i = 0; i<bmpImage.height; i++)
	{
		for (j = 0; j<weight; j++)
			fwrite(rastr[i][j], 3, 1, fout);
		fwrite(pad, padding, 1, fout);
	}

	fclose(fout);
}

char* nameCreate(char fileSymbol /*captcha[i]*/) 
{
 /* Создание названия картинки */
 char *fileName = (char*)malloc(6*sizeof(char));
 
 fileName[5]='\0';
 
 fileName[0]=fileSymbol;
 strcat(fileName,".bmp");
 
 return fileName;
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



