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

static bool MonoIsConst(const Mono *m)
{
	return (m->exp == 0 && PolyIsCoeff(&(m->poly)));
}

static bool MonoIsZero(const Mono *m)
{
	return MonoIsConst(m) && PolyIsZero(&(m->poly));
}

static Mono MonoAdd(const Mono *m, const Mono *n)
{
	assert(m->exp == n->exp);
	Poly temp_poly = PolyAdd(&(m->poly), &(n->poly));
	return (Mono) {.poly = temp_poly, .exp = m->exp, .next = NULL};
}

static void MonoInsertRecursivelyIntoList(Mono **mono_list, Mono *m)
{
	if (*mono_list)
	{
		if ((*mono_list)->exp < m->exp)
		{
			Mono* new_mono = (Mono*) malloc(sizeof (Mono));
			*new_mono = *m;
			new_mono->next = *mono_list;
			*mono_list = new_mono;
		}

		else if ((*mono_list)->exp == m->exp)
		{
			Mono* new_mono = (Mono*) malloc(sizeof (Mono));
			*new_mono = MonoAdd(*mono_list, m);
			new_mono->next = (*mono_list)->next;

			MonoDestroy(m);
			MonoDestroy(*mono_list);

			*mono_list = new_mono;
		}

		else
		{
			MonoInsertRecursivelyIntoList(&((*mono_list)->next), m);
		}
	}

	else
	{
		Mono* new_mono = (Mono*) malloc(sizeof (Mono));
		*new_mono = *m;
		new_mono->next = NULL;
		*mono_list = new_mono;
	}
}

static void PolyInsertMono(Poly *p, Mono *m)
{
	if (!MonoIsZero(m))
	{
		if (PolyIsCoeff(p) && MonoIsConst(m))
		{
			p->coeff += (m->poly).coeff;
		}

		else if (PolyIsCoeff(p))
		{
			Mono temp_mono = MonoFromPoly(p, 0);

			*p = PolyZero();
			MonoInsertRecursivelyIntoList(&(p->mono_list), m);

			if (!MonoIsZero(&temp_mono))
			{
				MonoInsertRecursivelyIntoList(&(p->mono_list), &temp_mono);
			}
		}

		else
		{
			MonoInsertRecursivelyIntoList(&(p->mono_list), m);
		}
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

Poly PolyAdd(const Poly *p, const Poly *q)
{
	if (PolyIsCoeff(p) && PolyIsCoeff(q))
	{
		return (Poly) {.mono_list = NULL, .coeff = p->coeff + q->coeff};
	}

	else if (PolyIsCoeff(q))
	{
		Poly poly_clone = PolyClone(p);

		Poly temp_poly = PolyClone(q);
		Mono temp_mono = MonoFromPoly(&temp_poly, 0);

		PolyInsertMono(&poly_clone, &temp_mono);
		return poly_clone;
	}

	else if (PolyIsCoeff(p))
	{
		return PolyAdd(q, p);
	}

	else
	{
		Poly poly_clone = PolyClone(p);
		Poly temp_poly = PolyClone(q);
		Mono* temp_pointer = temp_poly.mono_list;

		while (temp_pointer)
		{
			Mono temp_mono = MonoClone(temp_pointer);
			PolyInsertMono(&poly_clone, &temp_mono);
			temp_pointer = temp_pointer->next;
		}

		return poly_clone;
	}
}
