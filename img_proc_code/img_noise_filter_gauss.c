#include "img.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

float randn (float mu, float sigma){
	float U1, U2, W, mult;
	static float X1, X2;
	static int call = 0;

	if (call == 1){
		call = !call;
		return (mu + sigma * (float) X2);
	}
 
	do{
		U1 = -1 + ((float) rand () / RAND_MAX) * 2;
		U2 = -1 + ((float) rand () / RAND_MAX) * 2;
		W = pow (U1, 2) + pow (U2, 2);
	}while (W >= 1 || W == 0);
	mult = sqrt ((-2 * log (W)) / W);
	X1 = U1 * mult;
	X2 = U2 * mult;
 
	call = !call;
 
	return (mu + sigma * (float) X1);
}

void gaussian_noise(const Image *orig, Image *new){
	int channels=1;
	float mu=5, sigma=0.1;
	img_create(new, orig->width, orig->height, orig->channels, false);
	unsigned char *op, *np;
	for (op=orig->img_data, np=new->img_data; op!=(orig->img_data + orig->size); op+=orig->channels, np+=new->channels){
		*np=(uint8_t)(((*op)+randn(0,1)))%256;
	}
}

uint8_t mean(unsigned char *ptr, int i, int j, int n, int width){
	//unsigned char temp[n*n];
	int k=0;
	int l=-1*(n/2);
	int u=(n/2);
	int sum=0;
	for(int x=l;x<=u;x++){
		for(int y=l;y<=u;y++){
			if(*(ptr+(width*i+j)+(width*x+y))<0)
				sum+=0;
			else
				sum+=*(ptr+(width*i+j)+(width*x+y));
			k++;
		}
	}
	int mean=sum/(n*n);
	return (uint8_t) mean;
}

void mean_filter(const Image *orig, Image *new, int n){
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
			uint8_t mean_pix=mean(np, r, c, n, orig->width);
			*(np + (orig->width*r+c))=mean_pix;
		}
	}
}

int main(int argc, char **argv){
	if(argc!=2){
		printf("Usage: ./noise_filter_gauss <input-image-name>\n");
		exit(1);
	}
	int kernel=3;
	Image inp, nout, out;
	img_load(&inp, argv[1]);
	char outputfile[100];
	printf("Enter output file-name of noisy image: ");
	scanf("%s", outputfile);
	gaussian_noise(&inp, &nout);
	img_save(&nout, outputfile);
	load_ptr(&nout);
	printf("Enter output file-name of filtered image: ");
	scanf("%s", outputfile);
	mean_filter(&nout, &out, kernel);
	img_save(&out, outputfile);
}
