/* Aashish Kumar */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HEADER_SIZE 54

/* This program takes a bmp image as input and return four 
   images which are bright, contrasted, rotated and scaled down. */

void brigthening(char header[], int w,int h, unsigned char pixels[h][w*3]);
void contrast(char header[], int w,int h, unsigned char pixels[h][w*3]);
void rotate(char header[], int w,int h, unsigned char pixels[h][w*3]);
void scaled(char header[], int w,int h, unsigned char pixels[h][w*3]);

/* Main method starting the starting the program and 
    ask user to enter the name of a bmp image, height and width of that image. */ 
int main(void) {
	char name[20];
	char temp[7] =".bmp" ;
	int height = 0, width = 0;
	printf("Enter the filename: ");
	scanf("%s", name);
	strcat (name,temp);
	printf("Enter height and width (in pixels): ");
	scanf("%d %d", &height, &width);
	printf("Done. Check the generated images.");

	FILE *infile = fopen(name, "rb");

	char header[HEADER_SIZE];
	unsigned char pixels[height][width * 3];

	fread( header, 1 , HEADER_SIZE , infile);
	fread( pixels, 1 , height * width * 3 , infile);

	
	brigthening(header, width, height, pixels);
	contrast(header, width, height, pixels);
	rotate(header, width, height, pixels);
    scaled(header, width, height, pixels);
    
	fclose(infile);
	return 0;

}

/* This method increase the bright of image */ 
void brigthening(char header[], int w,int h, unsigned char pixels[h][w*3]) {
	FILE *outfile = fopen("copy1.bmp", "wb");
	unsigned char bright[h][w * 3];
	int r,c;
	for(r = 0; r < h; r++) {
		for ( c = 0; c < w * 3; c ++) {
			if(pixels[r][c] > 205) {
				bright[r][c] = 255;
			} else {
				bright[r][c] = pixels[r][c] + 50;// increasing the each pixel by 50 to increase brightness
			}
		}
	}
	fwrite( header, sizeof(char)  , HEADER_SIZE  ,  outfile);
	fwrite( bright, sizeof(char)  , h * w * 3  ,  outfile);

	fclose(outfile);

}
/* This method change the contrast of image by using the algorithm given by professor */
void contrast(char header[], int w,int h, unsigned char pixels[h][w*3]) {
	float  contrast_ratio = 2.9695;
	FILE *outfile = fopen("copy2.bmp", "wb");
	unsigned char contrast[h][w * 3];
	int r,c;
	for(r = 0; r < h; r++) {
		for ( c = 0; c < w * 3; c++) {
			float temp = contrast_ratio * (pixels[r][c] - 128) + 128;
			if(temp > 255) {
				contrast[r][c] = 255;
			} else if(temp < 0) {
				contrast[r][c] = 0;
			} else {
				contrast[r][c] = temp;
			}
		}
	}

	fwrite( header, sizeof(char)  , HEADER_SIZE  ,  outfile);
	fwrite( contrast, sizeof(char)  , h * w * 3  ,  outfile);
	fclose(outfile);

}
/* This method rotate the image by 180 degree */
void rotate(char header[], int w,int h, unsigned char pixel[h][w*3]) {

	FILE *outfile = fopen("copy3.bmp", "wb");
	unsigned char rotate[h][w * 3];
	int r,c;
	int col = w*3-1;// storing the last value of column.
	for(r = 0; r < h; r++) {
		for ( c = 0; c < w * 3; c += 3) { 
		    // rotating each color value in one pixel(blue,green,red)
			rotate[r][c] = pixel[h-1-r][col-(c+2)];
			rotate[r][c+1] = pixel[h-1-r][col-(c+1)];
			rotate[r][c+2] = pixel[h-1-r][col-(c)];
		}
	}
	fwrite( header, sizeof(char)  , HEADER_SIZE  ,  outfile);
	fwrite( rotate, sizeof(char)  , h * w * 3  ,  outfile);

	fclose(outfile);

}
/* This method scaled the image */
void scaled(char header[], int w,int h, unsigned char pixel[h][w*3]) {
	FILE *outfile = fopen("copy4.bmp", "wb");
	unsigned char scaled[h][w * 3];
	unsigned char scaled1[h][w * 3];
	int r,c,d;
	//this nested loop make the picture of size one fourth and store it.
	for(r = 1; r < h; r+=2) {
		for ( c = 3; c < w * 3; c += 6) {
			scaled[r/2][(c/2)] = pixel[r][c+1];
			scaled[r/2][(c/2)+1] = pixel[r][c+2];
			scaled[r/2][(c/2)+2] = pixel[r][c+3];

		}
	}
	//this nested loop draw the small size at four places.
	for(r = 0; r < h/2; r++) {
		for (c = 0; c < (w * 3)/2; c++) {
			scaled1[r][c] = scaled[r][c];// this line draw small image on left bottom side.
			scaled1[r+(h/2)][c] = scaled[r][c];// this line draw small image on left top side.
			scaled1[r][c+(w*3)/2] = scaled[r][c];// this line draw small image on right bottom side.
			scaled1[r+(h/2)][c+(w*3)/2] = scaled[r][c];	// this line draw small image on right top side.		
		}
    }

	fwrite( header, sizeof(char)  , HEADER_SIZE  ,  outfile);
	fwrite( scaled1, sizeof(char)  , h * w * 3  ,  outfile);
	fclose(outfile);
}

