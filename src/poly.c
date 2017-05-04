/** @file
   Implementacja funkcji pomocniczych i dłuższych funkcji z poly.h

   @author TODO
   @copyright Uniwersytet Warszawski
   @date TODO
*/

#include "poly.h"

// Funkcje pomocnicze

/**
 * Rekurencyjnie usuwa z pamięci jednomiany znajdujące się w liście.
 * @param[in] m : pierwszy jednomian w liście
 */
void MonoListDestroy(Mono *m)
{
	if (m)
	{
		MonoListDestroy(m->next);
		free(m);
	}
}

// Funkcje główne

void PolyDestroy(Poly *p)
{
	MonoListDestroy(p->mono_list);
}
