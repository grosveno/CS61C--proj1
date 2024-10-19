/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	FILE* input = fopen(filename, "r");
	// read mode from the first line
	char mode[3];
	fscanf(input, "%s", mode);
	// read row and col from the second line
	uint32_t row, col;
	fscanf(input, "%u %u", &row, &col);
	// read 255 from the third line
	int rate;
	fscanf(input, "%d", &rate);
	//construct an Image object
	Image* images = (Image*)malloc(sizeof(Image));
	images->rows = row;
	images->cols = col;
	images->image = (Color**)malloc(row * sizeof(Color*));
	for (int i = 0; i < row; i++) {
		images->image[i] = (Color*)malloc(col * sizeof(Color));
	}
	// read images from next lines
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			uint8_t R, G, B;
			fscanf(input, "%hhu %hhu %hhu", &R, &G, &B);
			images->image[i][j].R = R;
			images->image[i][j].G = G;
			images->image[i][j].B = B;
		}
	}
	fclose(input);
	return images;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	printf("%s\n", "P3");
	uint32_t row = image->rows;
	uint32_t col = image->cols;
	printf("%u %u\n", row, col);
	printf("%d\n", 255);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			int R = image->image[i][j].R;
			int G = image->image[i][j].G;
			int B = image->image[i][j].B;
			printf("%3d %3d %3d", R, G, B);
			if (j != col - 1) {
				printf("   ");
			}
		}
		printf("\n");
	}
}

//Frees an image
void freeImage(Image *image)
{
	int row = image->rows;
	for (int i = 0; i < row; i++) {
		free(image->image[i]);
	}
	free(image->image);
	free(image);
}