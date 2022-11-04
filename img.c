#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "img.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void img_load(Image *img, const char *filename){
	img->img_data=stbi_load(filename, &img->width, &img->height, &img->channels, 0);
	if(img->img_data != NULL){
		img->size=img->width*img->height*img->channels;
		img->alloc=1;
		printf("IMAGE LOADED!\nWidth: %d\nHeight: %d\nNumber of channels: %d\n\n", img->width, img->height, img->channels);
	}
}

void load_ptr(Image *img){
	if(img->img_data != NULL){
		img->size=img->width*img->height*img->channels;
		img->alloc=1;
		printf("IMAGE LOADED!\nWidth: %d\nHeight: %d\nNumber of channels: %d\n\n", img->width, img->height, img->channels);
	}
}

void img_save(const Image *img, const char *filename){
	stbi_write_jpg(filename, img->width, img->height, img->channels, img->img_data, 100);
	printf("IMAGE SAVED TO %s\n", filename);
}

void img_create(Image *img, int w, int h, int ch, bool zeroed){
	size_t size = w*h*ch;
	if(zeroed){
		img->img_data=calloc(size, 1);
	}
	else {
		img->img_data=malloc(size);
	}
	if(img->img_data != NULL){
		img->width = w;
		img->height = h;
		img->channels = ch;
		img->alloc = 2;	
	}
}

void img_gray(const Image *orig, Image *gray){
	int channels = (orig->channels == 4) ? 2: 1;
	img_create(gray, orig->width, orig->height, channels, false);
	unsigned char *p, *pg; 
	for(p= orig->img_data, pg = gray->img_data; p != (orig->img_data + orig->size); p += orig->channels, pg += gray->channels){
	*pg = (uint8_t)((*p + *(p + 1) + *(p + 2))/3.0);
	if(channels == 4){
		*(pg + 1) = *(p+3);
	}	  
} 
  
}