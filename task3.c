#include "header.h"

// Initialize pixel queue
TCoada_pixel InitQ_pixel(void)
{
	TCoada_pixel c = (TCoada_pixel)calloc(1, sizeof(TCelulaCoada_pixel));
	c->inc = c->sf = NULL;
	if (!c)
		return NULL;
	return c;
}

// Insert pixel into a pixel queue
void InsQ_pixel(TCoada_pixel c, PPMPixel x)
{
	// Allocate space for a new cell in queue
	TLista_pixel aux1 = (TLista_pixel)calloc(1, sizeof(TCelula_pixel));
	if (!aux1)
		return;
		
	// Copy pixel
	aux1->info = x;
	
	// It's the first cell I'm about to add
	if (c->inc == NULL)
		c->inc = aux1;
	// Otherwise, link it to last cell added in queue
	else
		c->sf->urm = aux1;
		
	// Modify queue end
	c->sf = aux1;
}

PPMPixel ExtQ_pixel(TCoada_pixel c)
{
	// Save first cell from queue
	TLista_pixel aux = c->inc;
	PPMPixel p = aux->info;
	
	// Modify queue front
	c->inc = aux->urm;
	
	// Single value in queue => empty queue
	if (c->inc == NULL)
		c->sf = NULL;
		
	// Free space allocated for first cell in queue
	free(aux);
	
	// Return info from first cell in queue (pixel type)
	return p;
}

void construieste_coada_pixeli(TCoada_pixel c, FILE *read)
{
	unsigned char type;
	
	while (fread(&type, sizeof(unsigned char), 1, read)) {
		PPMPixel pixel;
		pixel.type = type;
		
		if (pixel.type == 0) {
			// If pixel type read is 0 => it's root => has no color pixels
			pixel.red = 0;
			pixel.green = 0;
			pixel.blue = 0;
		} else {
			// Pixel type = 1 => it's leaf => has color pixels
			// which I read from file
			fread(&pixel.red, sizeof(unsigned char), 1, read);
			fread(&pixel.green, sizeof(unsigned char), 1, read);
			fread(&pixel.blue, sizeof(unsigned char), 1, read);
		}
		
		// After I put info to a pixel => I insert it in pixel queue
		InsQ_pixel(c, pixel);
	}
}

// To build the tree I use a queue where I put the read pixels
TArb construieste_arbore(TCoada_pixel c, TArb r)
{
	// Enters first if the first time and creates root of entire tree
	if (r == NULL) {
		PPMPixel y = ExtQ_pixel(c);
		// Allocate a tree type cell where I put info from extracted pixel
		TArb aux = (TArb)calloc(1, sizeof(TNod));
		if (!aux)
			return NULL;
			
		aux->info.type = y.type;
		aux->info.red = y.red;
		aux->info.green = y.green;
		aux->info.blue = y.blue;
		
		// For creating connections
		return aux;
	}
	
	// I'm at a root => type is 0 =>
	// has 4 children which I create and link to root
	if (r->info.type == 0) {
		r->fiu1 = construieste_arbore(c, r->fiu1);
		r->fiu2 = construieste_arbore(c, r->fiu2);
		r->fiu3 = construieste_arbore(c, r->fiu3);
		r->fiu4 = construieste_arbore(c, r->fiu4);
	}
	
	// Return created tree
	return r;
}

void Parcurge_Arb(TArb a, PPMPixel **grid, int x, int y, int lat)
{
	// Build image => create tree and then matrix
	// If it's leaf => a square from matrix will have same info
	if (a->info.type == 1) {
		for (int i = x; i < x + lat; i++) {
			for (int j = y; j < y + lat; j++) {
				grid[i][j].type = a->info.type;
				grid[i][j].red = a->info.red;
				grid[i][j].green = a->info.green;
				grid[i][j].blue = a->info.blue;
			}
		}
		// Exit from recursion
		return;
	}
	
	// If I'm at a root => I advance in tree until I find leaves
	Parcurge_Arb(a->fiu1, grid, x, y, lat/2);
	Parcurge_Arb(a->fiu2, grid, x, y + lat/2, lat/2);
	Parcurge_Arb(a->fiu3, grid, x + lat/2, y + lat/2, lat/2);
	Parcurge_Arb(a->fiu4, grid, x + lat/2, y, lat/2);
}

void afisare_imagine(PPMPixel **grid, unsigned int dim, const char *filename)
{
	// Write image to binary file
	FILE *write = fopen(filename, "wb");
	if (!write)
		return;
		
	fprintf(write, "P6\n");
	fprintf(write, "%u %u\n", dim, dim);
	fprintf(write, "255\n");
	
	// Display matrix created in previous step
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			fwrite(&grid[i][j].red, sizeof(unsigned char), 1, write);
			fwrite(&grid[i][j].green, sizeof(unsigned char), 1, write);
			fwrite(&grid[i][j].blue, sizeof(unsigned char), 1, write);
		}
	}
	
	// Close binary file where I wrote info
	fclose(write);
}

// Function for freeing space occupied by a quaternary tree
void distruge(TArb r)
{
	if (!r)
		return;
		
	distruge(r->fiu1);
	distruge(r->fiu2);
	distruge(r->fiu3);
	distruge(r->fiu4);
	free(r);
}

// Function for destroying pixel queue
void DistrugereQ(TCoada_pixel c)
{
	TLista_pixel p = c->inc, aux = c->inc;
	
	// Traverse queue
	while (p) {
		aux = p;
		p = p->urm;
		free(aux);
	}
	
	free(c);
	// I ensure the pointer to queue became NULL
	c = NULL;
}
