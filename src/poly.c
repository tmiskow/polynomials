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

/**
 * Robi pełną, głęboką kopię wszystkich jednomianów w liście.
 * @param[in] m : pierwszy jednomian w liście
 * @return sklonowana lista jednomianów
 */
Mono* MonoListClone(const Mono *m)
{
	if (m)
	{
	  	Mono* mono_list_clone = (Mono*) malloc(sizeof (Mono));
		assert(mono_list_clone);
		*mono_list_clone = MonoClone(m);
		return mono_list_clone;
	}

	else
	{
		return NULL;
	}
}


// Funkcje główne

void PolyDestroy(Poly *p)
{
	MonoListDestroy(p->mono_list);
}

Poly PolyClone(const Poly *p)
{
	return (Poly)
	{
		.mono_list = MonoListClone(p->mono_list),
		.coeff = p->coeff
	};
}

Mono MonoClone(const Mono *m)
{
	return (Mono)
	{
		.p = PolyClone(&(m->p)),
		.exp = m->exp,
		.next = MonoListClone(m->next)
	};
}
