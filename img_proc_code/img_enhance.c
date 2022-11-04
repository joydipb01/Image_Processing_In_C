#include "img.h"
#include <stdio.h>
#include <math.h>
#define M 256

void img_negative(const Image *orig, Image *new){
	int channels=1;
	img_create(new, orig->width, orig->height, orig->channels, false);
	unsigned char *op, *np;
	for (op=orig->img_data, np=new->img_data; op!=(orig->img_data + orig->size); op+=orig->channels, np+=new->channels){
		*np = (uint8_t)(255 - *op);
	}
}

void img_log_transform(const Image *orig, Image *new, const float c){
	int channels=1;
	img_create(new, orig->width, orig->height, channels, false);
	unsigned char *op, *np;
	for (op=orig->img_data, np=new->img_data; op!=(orig->img_data + orig->size); op+=orig->channels, np+=new->channels){
		*np = (uint8_t)(c*(log(1 + *op)));
	}
}

void img_pow_law_norm(const Image *orig, Image *new, const float c, const float gamma){
	int channels=1;
	img_create(new, orig->width, orig->height, channels, false);
	unsigned char *op, *np;
	for (op=orig->img_data, np=new->img_data; op!=(orig->img_data + orig->size); op+=orig->channels, np+=new->channels){
		*np = (uint8_t)(c*pow((*op)/255.0, gamma));
	}
}

void contrast_stretching(const Image *orig, Image *new, int newmin, int newmax){
	int channels=1;
	img_create(new, orig->width, orig->height, channels, false);
	uint8_t max=0, min=0;
	unsigned char *imgp, *op, *np;
	for (imgp=orig->img_data; imgp!=(orig->img_data + orig->size); imgp+=orig->channels){
		if((*imgp) > max)
			max=*imgp;
		else if((*imgp) <= min)
			min=*imgp;
	}
	printf("Max intensity = %hhu and Min intensity = %hhu\n", max, min);
	for (op=orig->img_data, np=new->img_data; op!=(orig->img_data + orig->size); op+=orig->channels, np+=new->channels){
		*np = (uint8_t)(((*op)-min)*((newmax-newmin)/(max-min))+newmin);
	}
}

void intensity_slicing(const Image *orig, Image *new, int min, int max){
	int channels=1;
	img_create(new, orig->width, orig->height, channels, false);
	unsigned char *op, *np;
	for (op=orig->img_data, np=new->img_data; op!=(orig->img_data + orig->size); op+=orig->channels, np+=new->channels){
		if((*op)>min && (*op)<max)
			*np = (uint8_t) 255;
		else
			*np = (uint8_t)(*op);
	}
}

void histogram_equalization(const Image *orig, Image *new){
	int channels=1;
	img_create(new, orig->width, orig->height, channels, false);
	unsigned char *imgp, *op, *np;
	int i, j, histogram[M];
	int size=(orig->height * orig->width);
	//int size=((int)orig->size);
	double prob[M], trans[M];
	for(i=0;i<M;i++){
		histogram[i]=0;
	}
	for (imgp=orig->img_data; imgp!=(orig->img_data + orig->size); imgp+=orig->channels){
		histogram[(*imgp)]+=1;
	}
	for(i=0;i<M;i++){
		prob[i]=((double) histogram[i]) / size;
	}
	for(i=0;i<M;i++){
		for(j=0;j<i;j++){
			trans[i]=trans[i]+(255*prob[j]);
		}
		trans[i]=round(trans[i]);
	}
	for (op=orig->img_data, np=new->img_data; op!=(orig->img_data + orig->size); op+=orig->channels, np+=new->channels){
		*np = trans[(*op)];
	}
	//img_gray(new1, new);
}

void bit_plane_slicing(const Image *orig, Image *new, int bit){
	int channels=1;
	img_create(new, orig->width, orig->height, channels, false);
	unsigned char *imgp, *op, *np;
	int count=0;
	int size=(orig->height * orig->width);
	int pixels[size];
	for(int i=0;i<size;i++){
		pixels[i]=0;
	}
	for(op=orig->img_data; op!=(orig->img_data + orig->size); op+=orig->channels){
		for(int i=7;i>=0;i--){
			int n = (int)(*op);
			int k = n>>i;
			if(k & 1){
				if(i==(bit-1))
					pixels[count]=1;
			}
			else{
				if(i==(bit-1))
					pixels[count]=0;
			}
		}
		count++;
	}
	int j;
	for(j=0, np=new->img_data;j<count;j++, np+=new->channels){
		*np = (uint8_t)(pixels[j]*pow(2, (bit-1)));
	}
}

int main(int argc, char **argv){
	if(argc!=2){
		printf("Usage: ./enhance <input-image-name>\n");
		exit(1);
	}
	Image inp, out;
	img_load(&inp, argv[1]);
	int choice;
	float c, gamma;
	int min, max, bit;
	char outputfile[100];
	while(1){
		printf("\nMENU\n");
        printf("----\n");
        printf("\t1. Image Negative Transformation\n");
        printf("\t2. Log Transformation\n");
        printf("\t3. Power Law (Gamma) Transformation\n");
		printf("\t4. Contrast Stretching\n");
		printf("\t5. Intensity Slicing\n");
		printf("\t6. Bit-plane slicing (for given bit-plane)\n");
		printf("\t7. Histogram Equalization\n");
        printf("\t8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
		switch(choice){
			case 1: printf("Enter output file-name: ");
					scanf("%s", outputfile);
					img_negative(&inp, &out);
					img_save(&out, outputfile);
					break;
			case 2: printf("Enter output file-name: ");
					scanf("%s", outputfile);
					printf("Enter the value of the constant: ");
					scanf("%f", &c);
					img_log_transform(&inp, &out, c);
					img_save(&out, outputfile);
					break;
			case 3: printf("Enter output file-name: ");
					scanf("%s", outputfile);
					printf("Enter the value of the constant: ");
					scanf("%f", &c);
					printf("Enter the value of gamma: ");
					scanf("%f", &gamma);
					img_pow_law_norm(&inp, &out, c, gamma);
					img_save(&out, outputfile);
					break;
			case 4: printf("Enter output file-name: ");
					scanf("%s", outputfile);
					printf("Enter new minimum intensity: ");
					scanf("%d", &min);
					printf("Enter new maximum intensity: ");
					scanf("%d", &max);
					contrast_stretching(&inp, &out, min, max);
					img_save(&out, outputfile);
					break;
			case 5: printf("Enter output file-name: ");
					scanf("%s", outputfile);
					printf("Enter minimum intensity: ");
					scanf("%d", &min);
					printf("Enter maximum intensity: ");
					scanf("%d", &max);
					intensity_slicing(&inp, &out, min, max);
					img_save(&out, outputfile);
					break;
			case 6: printf("Enter output file-name: ");
					scanf("%s", outputfile);
					printf("Enter the bit number to be sliced: ");
					scanf("%d", &bit);
					bit_plane_slicing(&inp, &out, bit);
					img_save(&out, outputfile);
					break;
			case 7: printf("Enter output file-name: ");
					scanf("%s", outputfile);
					histogram_equalization(&inp, &out);
					img_save(&out, outputfile);
					break;
			case 8: printf("Program terminated successfully!\n");
					return 0;
			default: printf("Invalid choice!\n");
		}
	}
}