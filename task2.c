#include "header.h"

TCoada InitQ(void)
{
	TCoada c = (TCoada)calloc(1, sizeof(TCelulaCoada));
	// Check if allocation succeeded
	if (!c)
		return NULL;
	// Pointer to queue if allocation succeeded
	return c;
}

void InsQ(TCoada c, TArb x)
{
	// I want to insert in a queue => allocate a new cell for queue
	TLista aux1 = (TLista)calloc(1, sizeof(TCelula));
	// Check allocation
	if (!aux1)
		return;
		
	// Info is of type TArb => needs to be allocated
	aux1->info = (TArb)calloc(1, sizeof(TNod));
	if (!aux1->info) {
		free(aux1);
		return;
	}
	
	// If allocations succeeded => copy info in cell I want to add to queue
	aux1->info->dim = x->dim;
	aux1->info->fiu1 = x->fiu1;
	aux1->info->fiu2 = x->fiu2;
	aux1->info->fiu3 = x->fiu3;
	aux1->info->fiu4 = x->fiu4;
	aux1->info->type = x->type;
	aux1->info->info.red = x->info.red;
	aux1->info->info.green = x->info.green;
	aux1->info->info.blue = x->info.blue;
	
	// It's the first cell I'm about to add
	if (c->inc == NULL)
		c->inc = aux1;
	// It's not the first cell inserted => I link it to last cell in queue
	else
		c->sf->urm = aux1;
		
	// Update the sf pointer
	c->sf = aux1;
}

TLista ExtQ(TCoada c)
{
	// No values to extract
	if (c->inc == NULL)
		return NULL;
		
	// Extract first cell from queue
	TLista aux = c->inc;
	// Modify queue front
	c->inc = c->inc->urm;
	
	// Single value in queue => empty queue
	if (c->inc == NULL) {
		c->sf = NULL;
	}
	
	return aux;
}

// When I no longer need the queue => I destroy it
void DistrQ(TCoada c)
{
	TLista p = c->inc, aux = c->inc;
	
	// Traverse queue
	while (p) {
		aux = p;
		p = p->urm;
		free(aux->info);
		free(aux);
	}
	
	free(c);
	// I ensure the pointer to queue became NULL
	c = NULL;
}
