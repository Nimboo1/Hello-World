#include "list.h"

void main() 
{
  int choice;
  
  printf("Enter 1 or 2");
  scanf("%d", &choice);
  
  if (choice == 1) 
  {
  FILE * fin1=fopen("1.bmp", "r"); //Открытие файла картинки на чтение
	perror("Open file");
	image[0] = fileRead(fin1);
	fclose(fin1);
	FILE * fin2 = fopen("2.bmp", "r"); //Открытие файла картинки на чтение
	perror("Open file");
	image[1] = fileRead(fin2);
	fclose(fin2);
	FILE * fout = fopen("3.bmp", "w"); //Открытие файла картинки на изменение
	perror("Open file");

	
	//Создание динамического трёхмерного массива с пикселями
	BYTE ***rastr1 = (BYTE***)malloc(image[0].height * sizeof(BYTE**));
	for (i = 0; i<image[0].height; i++)
	{
		rastr1[i] = (BYTE**)malloc(image[0].weight * sizeof(BYTE*));
		for (j = 0; j<image[0].weight; j++)
			rastr1[i][j] = (BYTE*)malloc(3 * sizeof(BYTE));
	}
	int padding1 = (image[0].size - image[0].head_end - image[0].height*image[0].weight * 3) / image[0].height;
	BYTE pad1[20];
	for (i = 0; i<image[0].height; i++)
	{
		for (j = 0; j<image[0].weight; j++)
			fread(rastr1[i][j], 3, 1, fin1);
		fread(pad1, padding1, 1, fin1);
	}
	
	
	//Создание динамического трёхмерного массива с пикселями
	BYTE ***rastr2 = (BYTE***)malloc(image[1].height * sizeof(BYTE**));
	for (i = 0; i<image[1].height; i++)
	{
		rastr2[i] = (BYTE**)malloc(image[1].weight * sizeof(BYTE*));
		for (j = 0; j<image[1].weight; j++)
			rastr2[i][j] = (BYTE*)malloc(3 * sizeof(BYTE));
	}
	int padding2 = (image[1].size - image[1].head_end - image[1].height*image[1].weight * 3) / image[1].height;
	BYTE pad2[20];
	for (i = 0; i < image[1].height; i++)
	{
		for (j = 0; j < image[1].weight; j++)
			fread(rastr2[i][j], 3, 1, fin2);
		fread(pad2, padding2, 1, fin2);
	}
	
	
	int weight = image[0].weight + image[1].weight;
	int w = image[0].weight;
		
	//Создание динамического трёхмерного массива с пикселями
	BYTE ***rastr = (BYTE***)malloc(image[0].weight * sizeof(BYTE**));
	for (i = 0; i<image[0].height; i++)
	{
		rastr[i] = (BYTE**)malloc(weight * sizeof(BYTE*));
		for (j = 0; j<weight; j++)
			rastr[i][j] = (BYTE*)malloc(3 * sizeof(BYTE));
	}
	
	mainWork2(weight,w, image[0],rastr1,rastr2,rastr);
	fileWrite1(image[0], weight, rastr, pad1, padding1, fout);

	for (i = 0; i<image[0].height; i++)
	{
		for (j = 0; j<image[0].weight; j++)
		{
			free(rastr1[i][j]);
		}
		free(rastr1[i]);
	}


	for (i = 0; i<image[1].height; i++)
	{
		for (j = 0; j<image[1].weight; j++)
		{
			free(rastr2[i][j]);
		}
		free(rastr2[i]);
	}


	for (i = 0; i<image[0].height; i++)
	{
		for (j = 0; j<weight; j++)
		{
			free(rastr[i][j]);
		}
		free(rastr[i]);
	}
  }
  else 
  {
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
  }
}

