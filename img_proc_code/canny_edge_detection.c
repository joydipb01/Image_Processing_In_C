#include "../img.h"
#include "canny.h"
#include <stdio.h>
#include <math.h>

int main(int argc, char **argv){
	if(argc!=2){
		printf("Usage: ./canny <input-image-name>\n");
		exit(1);
	}
	int tlow, tup;
	Image inp, nout;
	img_load(&inp, argv[1]);
	char outputfile[100];
	printf("Enter output file-name of edge-detected image: ");
	scanf("%s", outputfile);
	printf("Enter lower threshold: ");
	scanf("%d", &tlow);
	printf("Enter upper threshold: ");
	scanf("%d", &tup);
	canny_edge_detection(&inp, &nout, tlow, tup);
	img_save(&nout, outputfile);
}