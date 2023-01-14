#include <stdlib.h>
#include <math.h>

long long mean;
long long standard_deviation;

struct img_matrix{
	uint8_t** matrix;
	int width;
	int height;
	int offx;
	int offy;
};

struct neighbors{
	uint8_t up;
	uint8_t down;
	uint8_t right;
	uint8_t left;
	uint8_t upright;
	uint8_t upleft;
	uint8_t downright;
	uint8_t downleft;
};

struct mask{
	int** mask;
	int mask_size;
	int mask_radius;
};

struct masks{
	struct mask mask[10];
};

struct neighbors get_neighbors(uint8_t** matrix, int x, int y) {
    struct neighbors n;

    n.up = matrix[y - 1][x];
    n.down = matrix[y + 1][x];
    n.right = matrix[y][x + 1];
    n.left = matrix[y][x - 1];
    n.upright = matrix[y - 1][x + 1];
    n.upleft = matrix[y - 1][x - 1];
    n.downright = matrix[y + 1][x + 1];
    n.downleft = matrix[y + 1][x - 1];

    return n;
}

struct img_matrix copy_matrix(struct img_matrix* mat) {
    uint8_t** output = malloc(mat->height * sizeof(*output));
    for (int i = 0; i < mat->height; i++) {
        output[i] = malloc(mat->width * sizeof(output[0]));
    }

    for (int i = 0; i < mat->height; ++i) {
        for (int j = 0; j < mat->width; ++j) {
            output[i][j] = mat->matrix[i][j];
        }
    }

    struct img_matrix out;
    out.matrix = output;
    out.width = mat->width;
    out.height = mat->height;
    return out;
}

uint8_t** img_to_matrix(uint8_t* input, int width, int height){
	if(width>0 && height>0){
		uint8_t** img=malloc(height*sizeof(*img));
		for(int i=0;i<height;i++){
			if(img){
				img[i]=malloc(width*sizeof(img[0]));
			}
		}
		
		if(img){
			for(int i=0;i<height;i++){
				if(img[i]){
					for(int j=0;j<width;j++){
						img[i][j]=input[j+i*width];
					}
				}
			}
		}
		return img;
	}
	return 0;
}

uint8_t* matrix_to_img(struct img_matrix* mat){
	if(mat->width>0 && mat->height>0){
		uint8_t* output=malloc(mat->width*mat->height*sizeof(output[0]));
		
		if(output){
			for(int i=0;i<mat->height;i++){
				for(int j=0;j<mat->width;j++){
					output[j+i*mat->width]=mat->matrix[i][j];
				}
			}
		}
		return output;
	}
	return 0;
}

struct masks create_masks(){
	struct masks write_to;

	int** mask_gaussian=malloc(5*sizeof(*mask_gaussian));
	if(mask_gaussian){
		for(int i=0; i<5; i++){
			mask_gaussian[i]=malloc(5*sizeof(mask_gaussian[0]));
		}
		
		mask_gaussian[0][0] = 2; 
		mask_gaussian[0][1] = 4; 
		mask_gaussian[0][2] = 5; 
		mask_gaussian[0][3] = 4; 
		mask_gaussian[0][4] = 2;
		mask_gaussian[1][0] = 4; 
		mask_gaussian[1][1] = 9; 
		mask_gaussian[1][2] = 12; 
		mask_gaussian[1][3] = 9; 
		mask_gaussian[1][4] = 4;
		mask_gaussian[2][0] = 5; 
		mask_gaussian[2][1] = 12; 
		mask_gaussian[2][2] = 15; 
		mask_gaussian[2][3] = 12; 
		mask_gaussian[2][4] = 5;
		mask_gaussian[3][0] = 4;
		mask_gaussian[3][1] = 9; 
		mask_gaussian[3][2] = 12; 
		mask_gaussian[3][3] = 9; 
		mask_gaussian[3][4] = 4;
		mask_gaussian[4][0] = 2; 
		mask_gaussian[4][1] = 4; 
		mask_gaussian[4][2] = 5; 
		mask_gaussian[4][3] = 4; 
		mask_gaussian[4][4] = 2;
		
		write_to.mask[0].mask=mask_gaussian;
		write_to.mask[0].mask_size=5;
		write_to.mask[0].mask_radius=2;
	}
	
	int** sobel_horiz=malloc(3*sizeof(sobel_horiz));
	if(sobel_horiz){
		for(int i=0;i<3;i++){
			sobel_horiz[i]=malloc(3*sizeof(sobel_horiz[0]));
		}
		
		sobel_horiz[0][0]=-1;
		sobel_horiz[0][1]=-2;
		sobel_horiz[0][2]=-1;
		sobel_horiz[1][0]=0;
		sobel_horiz[1][1]=0;
		sobel_horiz[1][2]=0;
		sobel_horiz[2][0]=1;
		sobel_horiz[2][1]=2;
		sobel_horiz[2][2]=1;
		
		write_to.mask[1].mask=sobel_horiz;
		write_to.mask[1].mask_size=3;
		write_to.mask[1].mask_radius=1;
	}
	
	int** sobel_vert=malloc(3*sizeof(sobel_vert));
	if(sobel_vert){
		for(int i=0;i<3;i++){
			sobel_vert[i]=malloc(3*sizeof(sobel_vert[0]));
		}
		
		sobel_vert[0][0]=-1;
		sobel_vert[0][1]=0;
		sobel_vert[0][2]=1;
		sobel_vert[1][0]=-2;
		sobel_vert[1][1]=0;
		sobel_vert[1][2]=2;
		sobel_vert[2][0]=-1;
		sobel_vert[2][1]=0;
		sobel_vert[2][2]=1;
		
		write_to.mask[2].mask=sobel_vert;
		write_to.mask[2].mask_size=3;
		write_to.mask[2].mask_radius=1;
	}
	
	int** sobel_45=malloc(3*sizeof(sobel_45));
	if(sobel_45){
		for(int i=0;i<3;i++){
			sobel_45[i]=malloc(3*sizeof(sobel_45[0]));
		}
		
		sobel_45[0][0]=0;
		sobel_45[0][1]=1;
		sobel_45[0][2]=2;
		sobel_45[1][0]=-1;
		sobel_45[1][1]=0;
		sobel_45[1][2]=1;
		sobel_45[2][0]=-2;
		sobel_45[2][1]=-1;
		sobel_45[2][2]=0;
		
		write_to.mask[3].mask=sobel_45;
		write_to.mask[3].mask_size=3;
		write_to.mask[3].mask_radius=1;
	}
	
	int** laplacian=malloc(3*sizeof(laplacian));
	if(laplacian){
		for(int i=0;i<3;i++){
			laplacian[i]=malloc(3*sizeof(laplacian[0]));
		}
		
		laplacian[0][0]=1;
		laplacian[0][1]=1;
		laplacian[0][2]=1;
		laplacian[1][0]=1;
		laplacian[1][1]=-8;
		laplacian[1][2]=1;
		laplacian[2][0]=1;
		laplacian[2][1]=1;
		laplacian[2][2]=1;
		
		write_to.mask[4].mask=laplacian;
		write_to.mask[4].mask_size=3;
		write_to.mask[4].mask_radius=1;
	}
	
	int** scharr=malloc(3*sizeof(scharr));
	if(scharr){
		for(int i=0;i<3;i++){
			scharr[i]=malloc(3*sizeof(scharr[0]));
		}
		
		scharr[0][0]=47;
		scharr[0][1]=0;
		scharr[0][2]=-47;
		scharr[1][0]=162;
		scharr[1][1]=0;
		scharr[1][2]=-162;
		scharr[2][0]=47;
		scharr[2][1]=0;
		scharr[2][2]=-47;
		
		write_to.mask[5].mask=scharr;
		write_to.mask[5].mask_size=3;
		write_to.mask[5].mask_radius=1;
	}
	
	return write_to;
}

int approximate_angle(double angle) {
    if (((angle < 22.5) && (angle > -22.5)) || (angle > 157.5) || (angle < -157.5)) return 0;
    else if (((angle > 22.5) && (angle < 67.5)) || (angle < -112.5) || (angle > -157.5)) return 45;
    else if (((angle > 67.5) && (angle < 112.5)) || (angle < -67.5) || (angle > -112.5)) return 90;
    else if (((angle > 112.5) && (angle < 157.5)) || (angle < -22.5) || (angle > -67.5)) return 135;
}

uint8_t calculate_mask(uint8_t** input, int x, int y, struct mask m) {
    m.mask_radius = (int)(m.mask_size / 2);
    long long sum = 0;
    
    for (int i = y - m.mask_radius; i <= y + m.mask_radius; ++i) {
        for (int j = x - m.mask_radius; j <= x + m.mask_radius; ++j) {
        }
    }

    for (int i = y - m.mask_radius; i <= y + m.mask_radius; ++i) {
        for (int j = x - m.mask_radius; j <= x + m.mask_radius; ++j) {

            uint8_t image_val = input[i][j];
            
            int mask_val = m.mask[i - y + m.mask_radius][j - x + m.mask_radius];

            sum += (long long)image_val * mask_val;

        }
    }

    if (sum < 0) sum = 0;
    if (sum > 255) sum = 255;

    return (uint8_t)sum;
}

uint8_t calculate_gaussian_mask(uint8_t** input, int x, int y, struct mask m) {
    m.mask_radius = (int)(m.mask_size / 2);
    int sum = 0;

    for (int i = y - m.mask_radius; i <= y + m.mask_radius; ++i) {
        for (int j = x - m.mask_radius; j <= x + m.mask_radius; ++j) {

            uint8_t image_val = input[i][j];

            int mask_val = m.mask[i - y + m.mask_radius][j - x + m.mask_radius];

            sum += (long long)image_val * mask_val;
        }
    }

    sum /= 159;

    if (sum < 0) sum = 0;
    if (sum > 255) sum = 255;
    return (uint8_t)sum;
}

void gaussian_blur(struct img_matrix* mat) {
    struct masks mo = create_masks();
    struct mask m = mo.mask[0];



    if (mat->width > 0 && mat->height > 0) {
        uint8_t** gaussian_blur = malloc(mat->height * sizeof(*gaussian_blur));
        for (int i = 0; i < mat->height; i++) {
            gaussian_blur[i] = malloc(mat->width * sizeof(gaussian_blur[0]));
        }

        for (int i = m.mask_radius; i < mat->height - m.mask_radius; ++i) {
            for (int j = m.mask_radius; j < mat->width - m.mask_radius; ++j) {

                uint8_t pixel_value = calculate_gaussian_mask(mat->matrix, j, i, m);

                gaussian_blur[i][j] = pixel_value;

            }
        }

        mat->matrix = gaussian_blur;
    }
}

void sobel_horizontal(struct img_matrix* mat) {
    struct masks mo = create_masks();
    struct mask m = mo.mask[2];

    uint8_t** gradient_strength = malloc(mat->height * sizeof(*gradient_strength));
    for (int i = 0; i < mat->height; i++) {
        gradient_strength[i] = malloc(mat->width * sizeof(gradient_strength[0]));
    }

    for (int i = m.mask_radius; i < mat->height - m.mask_radius - 1; i++) {
        for (int j = m.mask_radius; j < mat->width - m.mask_radius - 1; j++) {

            uint8_t pixel_value = calculate_mask(mat->matrix, j, i, m);

            gradient_strength[i][j] = pixel_value;

        }
    }

    mat->matrix = gradient_strength;
}

void sobel_vertical(struct img_matrix* mat) {
    struct masks mo = create_masks();
    struct mask m = mo.mask[1];

    uint8_t** gradient_strength = malloc(mat->height * sizeof(*gradient_strength));
    for (int i = 0; i < mat->height; i++) {
        gradient_strength[i] = malloc(mat->width * sizeof(gradient_strength[0]));
    }

    for (int i = m.mask_radius; i < mat->height - m.mask_radius - 1; i++) {
        for (int j = m.mask_radius; j < mat->width - m.mask_radius - 1; j++) {

            uint8_t pixel_value = calculate_mask(mat->matrix, j, i, m);

            gradient_strength[i][j] = pixel_value;

        }
    }

    mat->matrix = gradient_strength;
}

void hysteresis(struct img_matrix* mat, int** edge_angle, uint8_t threshold_high, uint8_t threshold_low) {
    for (int i = 1; i < mat->height - 1; ++i) {
        for (int j = 1; j < mat->width - 1; ++j) {
            uint8_t pixel_value = mat->matrix[i][j];

            struct neighbors n = get_neighbors(mat->matrix, j, i);

            if (pixel_value > threshold_high) {
                mat->matrix[i][j] = 255;
            }
            else if (pixel_value < threshold_low) {
                mat->matrix[i][j] = 0;
            }
            else if (pixel_value < 0) {
                mat->matrix[i][j] = 0;
            }
        }
    }

}

void detect_edges(struct img_matrix* mat, int thresholdUpper, int thresholdLower) {

    uint8_t** output = malloc(mat->height * sizeof(*output));
    for (int i = 0; i < mat->height; i++) {
        output[i] = malloc(mat->width * sizeof(output[0]));
    }

    int** edge_angle = malloc(mat->height * sizeof(*edge_angle));
    for (int i = 0; i < mat->height; i++) {
        edge_angle[i] = malloc(mat->width * sizeof(edge_angle[0]));
    }

    struct img_matrix gradient_vertical = copy_matrix(mat);
    struct img_matrix gradient_horizontal = copy_matrix(mat);

    sobel_vertical(&gradient_vertical);
    sobel_horizontal(&gradient_horizontal);

    long long pixel_value_sum = 0;
    long long variance = 0;

    for (int i = 0; i < mat->height; ++i) {
        for (int j = 0; j < mat->width; ++j) {
            uint8_t gx_value = gradient_horizontal.matrix[i][j];
            uint8_t gy_value = gradient_vertical.matrix[i][j];

            double value = sqrt((double)gx_value * gx_value + (double)gy_value * gy_value);
            if (value > 255) {
                value = 255;
            }

            mat->matrix[i][j] = (uint8_t)value;

            pixel_value_sum += (long long)value;

            double angle_radians = atan2((double)(gy_value), (double)gx_value);
            double angle_deg = angle_radians * 180.0 / 3.1415;

            if (angle_deg < 0) {
                angle_deg += 360;
            }
            if (angle_deg > 255) {
                angle_deg = 255;
            }

            angle_deg = fmod(angle_deg, 360.0);

            edge_angle[i][j] = approximate_angle(angle_deg);
        }
    }

    mean = (long long)pixel_value_sum / (mat->width * mat->height);

    for (int i = 0; i < mat->height; ++i) {
        for (int j = 0; j < mat->width; ++j) {
            long long difference = mat->matrix[i][j] - mean;
            variance += difference * difference;
        }
    }
    standard_deviation = (long long)sqrt(variance / (mat->width * mat->height));

    hysteresis(mat, edge_angle, thresholdUpper, thresholdLower);
}

void canny_edge_detection(const Image *orig, Image *new, int low, int up){
	int channels=1;
	img_create(new, orig->width, orig->height, channels, false);
	
	struct img_matrix mat;
	mat.width=orig->width;
	mat.height=orig->height;
	mat.matrix=img_to_matrix(orig->img_data, orig->width, orig->height);
	
	gaussian_blur(&mat);
	
	detect_edges(&mat, up, low);
	
	new->img_data=matrix_to_img(&mat);
}