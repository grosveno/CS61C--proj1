/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

int dis_x[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dis_y[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	Color* cell = (Color*)malloc(sizeof(Color));
	int isAliveR = image->image[row][col].R == 255;
	int isAliveG = image->image[row][col].G == 255;
	int isAliveB = image->image[row][col].B == 255;
	int numR = 0;
	int numG = 0;
	int numB = 0;
	for (int i = 0; i < 8; i++) {
		int newRow = (dis_y[i] + row) % row;
		int newCol = (dis_x[i] + col) % col;
		if (image->image[newRow][newCol].R == 255) {
			numR++;
		}
		if (image->image[newRow][newCol].G == 255) {
			numG++;
		}
		if (image->image[newRow][newCol].B == 255) {
			numB++;
		}
	}
	int biasR = 9 * isAliveR + numR;
	int biasG = 9 * isAliveG + numG;
	int biasB = 9 * isAliveB + numB;
	if (rule & (1 << biasR)) {
		cell->R = 255;
	}else {
		cell->R = 0;
	}
	
	if (rule & (1 << biasG)) {
		cell->G = 255;
	}else {
		cell->G = 0;
	}

	if (rule & (1 << biasB)) {
		cell->B = 255;
	}else {
		cell->B = 255;
	}
	return cell;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image* new_image = (Image*)malloc(sizeof(Image));
	int row = image->rows;
	int col = image->cols;
	new_image->rows = row;
	new_image->cols = col;
	new_image->image = (Color**)malloc(row * sizeof(Color*));
	for (int i = 0; i < row; i++) {
		new_image->image[i] = (Color*)malloc(col * sizeof(Color));
	}
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			Color* pixel = evaluateOneCell(image, i, j, rule);
			new_image->image[i][j] = *pixel;
			free(pixel);
		}
	}
	return new_image;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if (argc < 3) {
		printf("usage: ./gameOfLife filename rule\n");
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		printf("rule is a hex number beginning with 0x; Life is 0x1808.\n");
		exit(-1);
	}
	char* filename = argv[1];
	Image* image = readData(filename);
	char* ruleOfString = argv[2];
	int rule = strtol(ruleOfString, NULL, 10);
	Image* newImage = life(image, rule);
	writeData(newImage);
	freeImage(image);
	freeImage(newImage);
	return 0;
}
