#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct{
	int height;
	int width;
	int channels;
	size_t size;
	uint8_t *img_data;
	int alloc;
}Image;

void img_load(Image *img, const char *filename);
void load_ptr(Image *img);
void img_save(const Image *img, const char *filename);
void img_create(Image *img, int w, int h, int ch, bool zeroed);
void img_gray(const Image *orig, Image *gray);