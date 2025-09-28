#include "header.h"

// The check function helps me know if a node is a leaf or not according to the requirement
int check(unsigned int lin, unsigned int col, PPMPixel **grid, unsigned int width, int factor)
{
	unsigned long long red = 0, green = 0, blue = 0;
	unsigned long long mean = 0;
	
	for (int i = lin; i < lin + width; i++) {
		for (int j = col; j < col + width; j++) {
			red = red + grid[i][j].red;
			green = green + grid[i][j].green;
			blue = blue + grid[i][j].blue;
		}
	}
	
	red /= width * width;
	green /= width * width;
	blue /= width * width;
	
	for (int i = lin; i < lin + width; i++) {
		for (int j = col; j < col + width; j++) {
			mean += (red - grid[i][j].red) * (red - grid[i][j].red);
			mean += (green - grid[i][j].green) * (green - grid[i][j].green);
			mean += (blue - grid[i][j].blue) * (blue - grid[i][j].blue);
		}
	}
	
	mean = mean / (3 * width * width);
	
	if (mean <= factor)
		// The node is a leaf
		return 1;
	// The node is not a leaf
	return 0;
}

// Function for building compression tree
TArb arb_compresie(unsigned int lin, unsigned int col, PPMPixel **grid, unsigned int width, int factor)
{
	unsigned long long red = 0, green = 0, blue = 0;
	
	if (check(lin, col, grid, width, factor) == 1) {
		// No longer needs to be divided => it's a leaf => new node where I put necessary info
		// then, I return it for connections
		TArb aux1 = (TArb)calloc(1, sizeof(TNod));
		if (!aux1)  // Allocation check
			return NULL;
			
		for (int i = lin; i < lin + width; i++) {
			for (int j = col; j < col + width; j++) {
				red = red + grid[i][j].red;
				green = green + grid[i][j].green;
				blue = blue + grid[i][j].blue;
			}
		}
		
		aux1->dim = width;
		aux1->info.red = red / (width * width);
		aux1->info.green = green / (width * width);
		aux1->info.blue = blue / (width * width);
		aux1->type = 1;
		return aux1;
	} else {
		// It's not a leaf => I allocate the node and then divide it into four
		TArb aux = (TArb)calloc(1, sizeof(TNod));
		// Allocation check
		if (!aux)
			return NULL;
			
		aux->fiu1 = arb_compresie(lin, col, grid, width/2, factor);
		aux->fiu2 = arb_compresie(lin, col + width/2, grid, width/2, factor);
		aux->fiu3 = arb_compresie(lin + width/2, col + width/2, grid, width/2, factor);
		aux->fiu4 = arb_compresie(lin + width/2, col, grid, width/2, factor);
		aux->type = '0';
		return aux;
	}
}

// Min function helps me with niv_min_frunza function
int Min(int a, int b, int c, int d)
{
	int minim = a;
	if (b < minim)
		minim = b;
	if (c < minim)
		minim = c;
	if (d < minim)
		minim = d;
	return minim;
}

// Max function helps me with info_nod_max function
int Max(int a, int b, int c, int d)
{
	int maxim = a;
	if (b > maxim)
		maxim = b;
	if (c > maxim)
		maxim = c;
	if (d > maxim)
		maxim = d;
	return maxim;
}

// niv_min_frunza + info_nod_max for maximum undivided side dimension
// maximum undivided side dimension = closest leaf to root
// I determine the minimum level where a leaf is found
int niv_min_frunza(TArb a, int crt_level)
{
	if (a == NULL)
		// MAX_POSSIBLE (I reached under tree)
		return 10000;
		
	// It's a leaf => I return current level
	if (a->fiu1 == NULL && a->fiu2 == NULL) {
		if (a->fiu3 == NULL && a->fiu4 == NULL) {
			return crt_level;
		}
	}
	
	// If it's not a leaf => I continue advancing in subtrees
	int min1 = niv_min_frunza(a->fiu1, crt_level + 1);
	int min2 = niv_min_frunza(a->fiu2, crt_level + 1);
	int min3 = niv_min_frunza(a->fiu3, crt_level + 1);
	int min4 = niv_min_frunza(a->fiu4, crt_level + 1);
	
	// I determine the minimum among the 4 values for current node
	return Min(min1, min2, min3, min4);
}

// I determine maximum information from leaf on minimum level
int info_nod_max(TArb a, int nivel, int niv_crt)
{
	if (a == NULL)
		return -1;
		
	// Minimum level of leaf = current level
	if (nivel == niv_crt)
		// I return leaf dimension
		return a->dim;
		
	// I'm not on a level equal to minimum => I advance in the 4 subtrees
	int info_nod1 = info_nod_max(a->fiu1, nivel, niv_crt + 1);
	int info_nod2 = info_nod_max(a->fiu2, nivel, niv_crt + 1);
	int info_nod3 = info_nod_max(a->fiu3, nivel, niv_crt + 1);
	int info_nod4 = info_nod_max(a->fiu4, nivel, niv_crt + 1);
	
	// I return maximum among the 4 values obtained in recursion
	return Max(info_nod1, info_nod2, info_nod3, info_nod4);
}

// Additional function for debugging
void afisare(TArb a, int nivel)
{
	if (a == NULL)
		return;
		
	printf("Frunza: %d %d %d %d\n", a->info.red, a->info.green, a->info.blue, nivel);
	afisare(a->fiu1, nivel + 1);
	afisare(a->fiu2, nivel + 1);
	afisare(a->fiu3, nivel + 1);
	afisare(a->fiu4, nivel + 1);
}

// Function that returns number of leaves from a tree
int Numara_Frunze(TArb a)
{
	int k = 0, nr_fiu1 = 0, nr_fiu2 = 0, nr_fiu3 = 0, nr_fiu4 = 0;
	
	// I reached under tree
	if (a == NULL)
		return 0;
		
	// I reached a leaf => k = 1
	if (a->fiu1 == NULL && a->fiu2 == NULL) {
		if (a->fiu3 == NULL && a->fiu4 == NULL)
			k = 1;
	}
	
	// I'm not at a leaf => I advance in subtrees
	nr_fiu1 = Numara_Frunze(a->fiu1);
	nr_fiu2 = Numara_Frunze(a->fiu2);
	nr_fiu3 = Numara_Frunze(a->fiu3);
	nr_fiu4 = Numara_Frunze(a->fiu4);
	
	return k + nr_fiu1 + nr_fiu2 + nr_fiu3 + nr_fiu4;
}

// Function that returns number of levels from tree
int NrNiveluri(TArb a)
{
	// I reached under tree
	if (a == NULL)
		return 0;
		
	// I advance in the 4 subtrees
	int nfiu1 = NrNiveluri(a->fiu1);
	int nfiu2 = NrNiveluri(a->fiu2);
	int nfiu3 = NrNiveluri(a->fiu3);
	int nfiu4 = NrNiveluri(a->fiu4);
	
	// For a root node, I determine maximum among number of levels from recursion
	int Max = nfiu1;
	if (nfiu2 > Max)
		Max = nfiu2;
	if (nfiu3 > Max)
		Max = nfiu3;
	if (nfiu4 > Max)
		Max = nfiu4;
		
	return 1 + Max;
}
