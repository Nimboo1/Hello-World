#include "stdio.h" 
#include "stdlib.h" 
#include "string.h" 
typedef unsigned char BYTE;
struct bmp { char type[2]; int size; int reserved; int head_end; int header_size; int weight; int height; int end[7]; };



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

int main()
{
	struct bmp image[2];
	int i, j;
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


	return 0;
}
