/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				ZQX - YOUR NAME HERE
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
	FILE *fp = fopen(filename,"r");
	if (fp == NULL)
	{
		perror("ERROR: cannot open the file!\n");
		return NULL;
	}
	Image *new_image = (Image *)malloc(sizeof(Image));
	if (new_image == NULL)
	{
		perror("ERROR: cannot malloc a new Image!\n");
		return NULL;
	}

	// read the file type
	char file_type[5];
	fscanf(fp, "%s", file_type);
	//read the row and cols
	fscanf(fp, "%d %d", &new_image->rows, &new_image->cols);
	// read the max value
	int scale;
	fscanf(fp, "%d", &scale);
	//creat a new color array according to rows and cols
	//use two dimensions version(also can use one dimension version)
	Color **image = (Color **) malloc(new_image->rows * sizeof(Color *));
	if (image == NULL)
	{
		perror("ERROR: cannot malloc a new image!\n");
		free(new_image);
		return NULL;
	}
	for (uint8_t i = 0; i < new_image->rows;i++){
		image[i] = (Color *)malloc(new_image->cols * sizeof(Color));
		if (image[i] == NULL)
		{
			for (uint8_t j = 0; j < i;j++){
				free(image[j]);
			}
			free(image);
			free(new_image);
			printf("%dth Color array:", i);
			perror("the Color array point cannot be malloc!\n");
			return NULL;
		}
	}
	for (uint8_t i = 0; i < new_image->rows; i++){
		for (uint8_t j = 0; j < new_image->cols; j++)
		{
			fscanf(fp, "%hhd %hhd %hhd", &image[i][j].R, &image[i][j].G, &image[i][j].B);
		}
	}
	new_image->image = image;
	fclose(fp);
	return new_image;

	//YOUR CODE HERE
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	printf("P3\n");
	printf("%d %d\n",image->rows,image->cols);
	printf("255\n");
	for (uint8_t i = 0; i < image->rows;i++){
		for (uint8_t j = 0; j < image->cols; j++){
			printf("%3d %3d %3d",image->image[i][j].R,image->image[i][j].G,image->image[i][j].B);
			if (j != image->cols - 1){
				printf("   ");
			}
		}
		printf("\n");
	}
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	for (uint8_t i = 0; i < image->rows;i++){
		free(image->image[i]);
	}
	free(image->image);
	free(image);
}