/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				zqx  YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include "imageloader.h"

int8_t adj[8][2] = {{-1,-1},{-1,0},{-1,1},
					{0,-1},		   {0,1},
					{1,-1},{1,0},{1,1}};
enum RGB
{
	R,
	G,
	B
};
uint8_t calculate_color(Image *image, uint32_t row, uint32_t col, uint32_t rule, enum RGB rgb);
// Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
// Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
// and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, uint32_t row, uint32_t col, uint32_t rule)
{
	//YOUR CODE HERE
	Color *new_color = (Color *) malloc(sizeof(Color));
	if(new_color == NULL){
		perror("cannot malloc a new color!\n");
		return NULL;
	}
	new_color->R = calculate_color(image,row,col,rule,R);
	new_color->G = calculate_color(image,row,col,rule,G);
	new_color->B = calculate_color(image,row,col,rule,B);
	return new_color;
}

uint8_t calculate_color(Image *image,uint32_t row,uint32_t col,uint32_t rule,enum RGB rgb){
	uint8_t result = 0;
	for (uint8_t i = 0; i < 8;i++){
		uint8_t alive_neighbors = 0;
		uint8_t cur_state = 0;
		switch (rgb)
		{
		case R:
			cur_state = (image->image[row][col].R >> i) & 1;
			break;
		case G:
			cur_state = (image->image[row][col].G >> i) & 1;
			break;
		case B:
			cur_state = (image->image[row][col].B >> i) & 1;
			break;
		default:
			break;
		}
		for (uint8_t j = 0; j < 8; j++)
		{
			uint32_t adj_row = (row + adj[j][0] + image->rows) % image->rows;
			uint32_t adj_col = (col + adj[j][1] + image->cols) % image->cols;
			uint8_t neighbor_state = 0;
			switch (rgb)
			{
			case R:
				neighbor_state = (image->image[adj_row][adj_col].R >> i) & 1;
				break;
			case G:
				neighbor_state = (image->image[adj_row][adj_col].G >> i) & 1;
				break;
			case B:
				neighbor_state = (image->image[adj_row][adj_col].B >> i) & 1;
				break;
			default:
				break;
			}
			alive_neighbors += neighbor_state;
		}
		uint8_t cur_bit = (cur_state) ? ((rule >> (alive_neighbors + 9)) & 1) : ((rule >> alive_neighbors) & 1);
		result |= cur_bit << i;
	}
	return result;
}
//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
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
	for (uint32_t i = 0; i < image->rows;i++){
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
	for (uint32_t i = 0; i < image->rows;i++){
		for (uint32_t j = 0; j < image->cols;j++){
			Color *new_color = evaluateOneCell(image,i,j,rule);
			new_image[i][j] = *new_color;
			free(new_color);
		}
	}
	new_Image->image = new_image;
	return new_Image;
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
	//YOUR CODE HERE
	if(argc < 3){
		printf("    usage: ./gameOfLife filename rule\n    filename is an ASCII PPM file (type P3) with maximum value 255.\n    rule is a hex number beginning with 0x; Life is 0x1808.\n");
		return -1;
	}
	Image *image = readData(argv[1]);
	if (image == NULL)
	{
		return -1;
	}
	char *endptr;
	errno = 0;
	unsigned long rule_tmp = strtol(argv[2], &endptr, 0);
	if(errno != 0 || *endptr != '\0'){
		fprintf(stderr, "Invalid rule format: %s\n", argv[2]);
    	freeImage(image);
   		return -1;
	}
	if(rule_tmp > 0x3FFFF){
		fprintf(stderr, "Warning: rule 0x%lX may be out of range\n", rule_tmp);

	}
	uint32_t rule = (uint32_t) rule_tmp;
	Image *gameOfLife_image = life(image,rule);
	if (gameOfLife_image == NULL)
	{
		return -1;
	}
	writeData(gameOfLife_image);
	freeImage(image);
	freeImage(gameOfLife_image);
	return 0;
}
