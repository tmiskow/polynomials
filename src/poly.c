#include "poly.h"

static void MonoDestroyListRecursively(Mono *m)
{
	if (m)
	{
		MonoDestroyListRecursively(m->next);
		MonoDestroy(m);
		free(m);
	}
}

static Mono* MonoCloneListRecursively(const Mono *m)
{
	if (m)
	{
		Mono* new_mono = (Mono*) malloc(sizeof (Mono));
		*new_mono = MonoClone(m);
		new_mono->next = MonoCloneListRecursively(m->next);
		return new_mono;
	}

	else
	{
		return NULL;
	}
}

// Funkcje główne

void PolyDestroy(Poly *p)
{
	MonoDestroyListRecursively(p->mono_list);

	// DEBUG
	*p = PolyZero();
}

Poly PolyClone(const Poly *p)
{
	return (Poly)
	{
		.mono_list = MonoCloneListRecursively(p->mono_list),
		.coeff = p->coeff
	};
}
