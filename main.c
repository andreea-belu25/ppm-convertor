#include "header.h"

int main(int argc, const char *argv[])
{
	PPMImage *img = NULL;
	PPMPixel **grid;
	int comp_color = 0;
	char char1 = 'a', char2 = 'b';
	int factor = 0;
	
	for (int i = 0; i < strlen(argv[2]); i++)
		factor = factor * 10 + (argv[2][i] - '0');

	if (strcmp(argv[1], "-c1") == 0 || strcmp(argv[1], "-c2") == 0) {
		// argv[0] - is always the executable, argv[3] = input_file
		FILE *read = fopen(argv[3], "rb");
		
		if (!read)
			return 0;
		
		// read image format (P6)
		fscanf(read, "%c%c\n", &char1, &char2);
		
		// verify image format
		if (char1 != 'P' || char2 != '6')
			return 0;
		
		// allocate memory for image
		img = (PPMImage *)calloc(1, sizeof(PPMImage));
		if (!img)
			return 0;
		
		// read image dimensions
		fscanf(read, "%d %d\n", &img->width, &img->height);
		
		fscanf(read, "%d", &comp_color);
		char random = '\0';
		fread(&random, sizeof(char), 1, read);
		
		// allocate pixel matrix
		grid = (PPMPixel **)calloc(img->height, sizeof(PPMPixel *));
		if (!grid)
			return 0;
		
		for (int i = 0; i < img->height; i++) {
			grid[i] = (PPMPixel *)calloc(img->width, sizeof(PPMPixel));
			if (!grid[i])
				return 0;
		}
		
		// read the image
		for (int i = 0; i < img->width; i++)
			for (int j = 0; j < img->height; j++) {
				fread(&grid[i][j].red, sizeof(unsigned char), 1, read);
				fread(&grid[i][j].green, sizeof(unsigned char), 1, read);
				fread(&grid[i][j].blue, sizeof(unsigned char), 1, read);
			}
		fclose(read);
	}
	
	if (strcmp(argv[1], "-c1") == 0) {
		// argv[0] - is always the executable, argv[3] = input_file
		FILE *read = fopen(argv[3], "rb");
		if (!read)
			return 0;
		
		// build compression tree based on given matrix
		TArb a = arb_compresie(0, 0, grid, img->width, factor);
		int nr_niveluri = NrNiveluri(a);
		int nr_frunze = Numara_Frunze(a);
		int nivel_min_frunza = niv_min_frunza(a, 0);
		int dim_frunza_niv_min = info_nod_max(a, nivel_min_frunza, 0);
		
		FILE *write = fopen(argv[4], "w");
		if (!write)
			return 0;
		
		fprintf(write, "%d\n", nr_niveluri);
		fprintf(write, "%d\n", nr_frunze);
		fprintf(write, "%d\n", dim_frunza_niv_min);
		
		// local allocation in first if
		for (int i = 0; i < img->width; i++)
			free(grid[i]);
		free(grid);
		free(img);
		
		// destroy compression tree formed by call
		distruge(a);
		
		// local openings => local closings
		fclose(write);
		fclose(read);
	}
	
	if (strcmp(argv[1], "-c2") == 0) {
		// argv[0] - is always the executable, argv[3] = input_file
		FILE *read = fopen(argv[3], "rb");
		if (!read)
			return 0;
		
		FILE *write = fopen(argv[4], "wb");
		if (!write)
			return 0;
		
		// image dimensions
		fwrite(&img->width, sizeof(unsigned int), 1, write);
		
		// create compression tree
		TArb a = arb_compresie(0, 0, grid, img->width, factor);
		
		// display tree in breadth-first order using a queue
		TCoada c = InitQ();
		InsQ(c, a);
		TLista qStart = c->inc;
		
		while (c->inc != NULL) {
			// insertion is done only if they are not null, otherwise it causes seg fault
			if (c->inc->info->fiu1)
				InsQ(c, c->inc->info->fiu1);
			
			if (c->inc->info->fiu2)
				InsQ(c, c->inc->info->fiu2);
			
			if (c->inc->info->fiu3)
				InsQ(c, c->inc->info->fiu3);
			
			if (c->inc->info->fiu4)
				InsQ(c, c->inc->info->fiu4);
			
			TLista cell = ExtQ(c);
			TArb aux = cell->info;
			char tip = 0;
			
			if (aux->type == 1) {
				unsigned char r = (unsigned char)aux->info.red;
				unsigned char g = (unsigned char)aux->info.green;
				unsigned char b = (unsigned char)aux->info.blue;
				tip = 1;
				fwrite(&tip, sizeof(unsigned char), 1, write);
				fwrite(&r, sizeof(unsigned char), 1, write);
				fwrite(&g, sizeof(unsigned char), 1, write);
				fwrite(&b, sizeof(unsigned char), 1, write);
			} else {
				fwrite(&tip, sizeof(unsigned char), 1, write);
			}
		}
		
		c->inc = qStart;
		
		// destroy queue needed for breadth-first traversal
		DistrQ(c);
		
		// destroy tree created by compression
		distruge(a);
		
		// local allocations and openings => local deallocations and closings
		for (int i = 0; i < img->width; i++)
			free(grid[i]);
		free(grid);
		free(img);
		
		fclose(write);
		fclose(read);
	}
	
	if (strcmp(argv[1], "-d") == 0) {
		// argv[0] - is always the executable, argv[2] = input_file
		FILE *read = fopen(argv[2], "rb");
		if (!read)
			return 0;
		
		unsigned int dim = 0;
		fread(&dim, sizeof(unsigned int), 1, read);
		
		grid = (PPMPixel **)calloc(dim, sizeof(PPMPixel *));
		if (!grid)
			return 0;
		
		for (int i = 0; i < dim; i++) {
			grid[i] = (PPMPixel *)calloc(dim, sizeof(PPMPixel));
			if (!grid[i])
				return 0;
		}
		
		// initialize pixel queue + construction
		TCoada_pixel c = InitQ_pixel();
		construieste_coada_pixeli(c, read);
		TArb arb = NULL;
		
		// traverse tree multiple times until leaves
		// because recursion function creates only one level
		while (c->inc != NULL) {
			arb = construieste_arbore(c, arb);
		}
		
		Parcurge_Arb(arb, grid, 0, 0, dim);
		afisare_imagine(grid, dim, argv[3]);
		
		// local allocations => local deallocations
		for (int i = 0; i < dim; i++)
			free(grid[i]);
		free(grid);
		free(img);
		
		DistrugereQ(c);
		distruge(arb);
		
		// local opening => local closing
		fclose(read);
	}
	
	return 0;
}
