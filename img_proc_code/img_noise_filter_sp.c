#include "img.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define M 1000

void salt_and_pepper_noise(const Image *orig, Image *new){
	int channels=1, i=0;
	img_create(new, orig->width, orig->height, orig->channels, false);
	int row=orig->height;
	int col=orig->width;
	unsigned char *op, *np;
	for (op=orig->img_data, np=new->img_data; op!=(orig->img_data + orig->size); op+=orig->channels, np+=new->channels){
		*np=*op;
	}
	op=orig->img_data;
	np=new->img_data;
	int lower=(orig->height*orig->width*orig->channels)/3;
	int upper=(orig->height*orig->width*orig->channels)/2;
	int n_pix=(rand()%(upper-lower+1))+lower;
	for(i=0;i<n_pix;i++){
		int y_coord=(rand()%row);
		int x_coord=(rand()%col);
		*(np+(orig->width*y_coord+x_coord))=(uint8_t)255;
	}
	n_pix=(rand()%(upper-lower+1))+lower;
	for(i=0;i<n_pix;i++){
		int y_coord=(rand()%row);
		int x_coord=(rand()%col);
		*(np+(orig->width*y_coord+x_coord))=(uint8_t)0;
	}
}

void insertion_sort(unsigned char item[], int n){
	int i, j;
	unsigned char key;
	for(i=1;i<n;i++){
		key=item[i];
		j=i-1;
		while(j>=0 && item[j]>key){
			item[j+1]=item[j];
			j=j-1;
		}
		item[j+1]=key;
	}
}

uint8_t median(unsigned char *ptr, int i, int j, int n, int width){
	unsigned char temp[n*n];
	int k=0;
	int l=-1*(n/2);
	int u=(n/2);
	for(int x=l;x<=u;x++){
		for(int y=l;y<=u;y++){
			if(*(ptr+(width*i+j)+(width*x+y))<0)
				temp[k]=0;
			else
				temp[k]=*(ptr+(width*i+j)+(width*x+y));
			k++;
		}
	}
	insertion_sort(temp, k);
	return (uint8_t) temp[k/2];
}

void median_filter(const Image *orig, Image *new, int n){
	int r=0, c=0, p;
	int channels=1;
	img_create(new, orig->width, orig->height, orig->channels, false);
	unsigned char *op, *np;
	for (op=orig->img_data, np=new->img_data; op!=(orig->img_data + orig->size); op+=orig->channels, np+=new->channels){
		*np=*op;
	}
	op=orig->img_data;
	np=new->img_data;
	for(r=0;r<orig->height-1;r++){
		for(c=0;c<orig->width-1;c++){
			uint8_t med_pix=median(np, r, c, n, orig->width);
			*(np + (orig->width*r+c))=med_pix;
		}
	}
}

int main(int argc, char **argv){
	if(argc!=2){
		printf("Usage: ./noise_filter <input-image-name>\n");
		exit(1);
	}
	int kernel=3;
	Image inp, nout, out;
	img_load(&inp, argv[1]);
	char outputfile[100];
	printf("Enter output file-name of noisy image: ");
	scanf("%s", outputfile);
	salt_and_pepper_noise(&inp, &nout);
	img_save(&nout, outputfile);
	load_ptr(&nout);
	printf("Enter output file-name of filtered image: ");
	scanf("%s", outputfile);
	median_filter(&nout, &out, kernel);
	img_save(&out, outputfile);
}