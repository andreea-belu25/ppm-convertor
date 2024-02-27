/*BELU ANDREEA-DANIELA 314CB*/
#include "header.h"

//  vector de stringuri constante
//  *argv[] = vector alocat dinamic cu dim variabila
int main(int argc, const char *argv[])
{
	PPMImage *img = NULL;
	// nu il pot initializa cu null pentru ca e pointer dublu
	PPMPixel **grid;
	int comp_color = 0;
	char char1 = 'a', char2 = 'b';
	//  factorul de compresie = argv[2] (char)
	//  vreau sa il convertesc la numar intreg
	int factor = 0;
	for (int i = 0; i < strlen(argv[2]); i++)
		factor = factor * 10 + (argv[2][i] - '0');
	if (strcmp(argv[1], "-c1") == 0 || strcmp(argv[1], "-c2") == 0) {
		// argv[0] - e mereu executabilul, argv[3] = fisier_input
		FILE *read = fopen(argv[3], "rb");
		if (!read)
			return 0;
		//  citire format imagine (P6)
		fscanf(read, "%c%c\n", &char1, &char2);
		//  verificare imagine format
		if (char1 != 'P' || char2 != '6')
			return 0;
		// aloc memorie pentru imagine
		img = (PPMImage *)calloc(1, sizeof(PPMImage));
		if (!img)
			return 0;
		//  citire dimensiuni imagine
		fscanf(read, "%d %d\n", &img->width, &img->height);
		fscanf(read, "%d", &comp_color);
		char random = '\0';
		fread(&random, sizeof(char), 1, read);
		// alocare matrice de pixeli
		grid = (PPMPixel **)calloc(img->height, sizeof(PPMPixel *));
		if (!grid)
			return 0;
		for (int i = 0; i < img->height; i++) {
			grid[i] = (PPMPixel *)calloc(img->width, sizeof(PPMPixel));
			if (!grid[i])
				return 0;
		}
		//  citesc imaginea
		for (int i = 0; i < img->width; i++)
			for (int j = 0; j < img->height; j++) {
				fread(&grid[i][j].red, sizeof(unsigned char), 1, read);
				fread(&grid[i][j].green, sizeof(unsigned char), 1, read);
				fread(&grid[i][j].blue, sizeof(unsigned char), 1, read);
			}
		fclose(read);
	}
	if (strcmp(argv[1], "-c1") == 0) {
		// argv[0] - e mereu executabilul, argv[3] = fisier_input
		FILE *read = fopen(argv[3], "rb");
		if (!read)
			return 0;
		//  construire arbore de compresie pe baza matricei date
		TArb a = arb_compresie(0, 0, grid, img->width, factor);
		int nr_niveluri = NrNiveluri(a);
		int nr_frunze = Numara_Frunze(a);
		int nivel_min_frunza = niv_min_frunza(a, 0);
		//printf("lowest level %d\n", nivel_min_frunza);
		int dim_frunza_niv_min = info_nod_max(a, nivel_min_frunza, 0);
		//  afisare in fisier text
		FILE *write = fopen(argv[4], "w");
		if (!write)
			return 0;
		//  afisari cerinta 1
		fprintf(write, "%d\n", nr_niveluri);
		fprintf(write, "%d\n", nr_frunze);
		fprintf(write, "%d\n", dim_frunza_niv_min);
		//  alocare locala in primul if
		for (int i = 0; i < img->width; i++)
			free(grid[i]);
		free(grid);
		free(img);
		//  distrug arborele de compresie format prin apel
		distruge(a);
		//  deschideri locale => inchideri locale
		fclose(write);
		fclose(read);
	}
	if (strcmp(argv[1], "-c2") == 0) {
		// argv[0] - e mereu executabilul, argv[3] = fisier_input
		FILE *read = fopen(argv[3], "rb");
		if (!read)
			return 0;
		FILE *write = fopen(argv[4], "wb");
		if (!write)
			return 0;
		//  dim imagine
		fwrite(&img->width, sizeof(unsigned int), 1, write);
		//  creare arbore de compresie
		TArb a = arb_compresie(0, 0, grid, img->width, factor);
		//  afisare arbore in latime folosind o coada
		TCoada c = InitQ();
		InsQ(c, a);
		TLista qStart = c->inc;
		while (c->inc != NULL) {
			//  inserarea se face doar daca nu sunt nuli, altfel da seg fault
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
		// distruge coada necesara parcurgerii in latime
		DistrQ(c);
		//  distruge arborele creat prin comprsie
		distruge(a);
		//  alocari si deschideri locale => dezalocari si inchideri locale
		for (int i = 0; i < img->width; i++)
			free(grid[i]);
		free(grid);
		free(img);
		fclose(write);
		fclose(read);
	}
	if (strcmp(argv[1], "-d") == 0) {
		// argv[0] - e mereu executabilul, argv[2] = fisier_input
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
		//  initializare coada pixeli + construire
		TCoada_pixel c = InitQ_pixel();
		construieste_coada_pixeli(c, read);
		TArb arb = NULL;
		//  parcurg arb de mai multe ori pana la frunze
		//  pt ca fct de recursivitate creeaza doar un sg nivel
		while (c->inc != NULL) {
			arb = construieste_arbore(c, arb);
		}
		Parcurge_Arb(arb, grid, 0, 0, dim);
		afisare_imagine(grid, dim, argv[3]);
		// alocari locale => dezalocari locale
		for (int i = 0; i < dim; i++)
			free(grid[i]);
		free(grid);
		free(img);
		DistrugereQ(c);
		distruge(arb);
		//  deschidere locala => inchidere locala
		fclose(read);
	}
	return 0;
}