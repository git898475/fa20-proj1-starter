/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				ZQX YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	Color *new_color = (Color *) malloc(sizeof(Color));
	if (new_color == NULL){
		perror("cannot malloc a new color!\n");
		return NULL;
	}
	//YOUR CODE HERE
	if ((image->image[row][col].B & 1) == 0){
		new_color->R = 0;
		new_color->G = 0;
		new_color->B = 0;
	} else{
		new_color->R = 255;
		new_color->G = 255;
		new_color->B = 255;
	}
	return new_color;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
	if (image == NULL || image->image == NULL)
	{
		return NULL;
	}
	Image *new_Image = (Image *)malloc(sizeof(Image));
	if(new_Image == NULL){
		perror("cannot alloc a new Image!\n");
		return NULL;
	}
	new_Image->rows = image->rows;
	new_Image->cols = image->cols;
	Color **new_image = (Color **)malloc(image->rows * sizeof(Color *));
	if(new_image == NULL){
		perror("cannot alloc a new image!\n");
		free(new_Image);
		return NULL;
	}
	for (uint8_t i = 0; i < image->rows;i++){
		new_image[i] = (Color *)malloc(image->cols * sizeof(Color));
		if (new_image[i] == NULL){
			for (uint8_t j = 0; j < i;j++){
				free(new_image[j]);
			}
			free(new_image);
			free(new_Image);
			return NULL;
			
		}
	}
	for (uint8_t i = 0; i < image->rows;i++){
		for (uint8_t j = 0; j < image->cols;j++){
			Color *new_color = evaluateOnePixel(image, i, j);
			new_image[i][j] = *new_color;
			free(new_color);
		}
	}
	new_Image->image = new_image;
	return new_Image;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if(argc < 2){
		return -1;
	}
	Image *image = readData(argv[1]);
	if (image == NULL)
	{
		return -1;
	}
	Image *steganography_image = steganography(image);
	if (steganography_image == NULL)
	{
		return -1;
	}
	writeData(steganography_image);
	freeImage(image);
	freeImage(steganography_image);
	return 0;
}
