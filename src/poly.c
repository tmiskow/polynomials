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

bool MonoIsEq(const Mono *m, const Mono *n)
{
	return m->exp == n->exp && PolyIsEq(&(m->poly), &(n->poly));
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

Poly PolyAddMonos(unsigned count, const Mono monos[])
{
	Poly new_poly = PolyZero();

	for (int i = 0; i < count; i++)
	{
		Mono temp_mono = monos[i];
		PolyInsertMono(&new_poly, &temp_mono);
	}

	return new_poly;
}

bool PolyIsEq(const Poly *p, const Poly *q)
{
	if (PolyIsCoeff(p) && PolyIsCoeff(q))
	{
		return p->coeff == q->coeff;
	}

	else if (PolyIsCoeff(q) || PolyIsCoeff(q))
	{
		return false;
	}

	else
	{
		Mono *temp_pointer_p = p->mono_list;
		Mono *temp_pointer_q = q->mono_list;

		while (temp_pointer_p && temp_pointer_q)
		{
			if (!MonoIsEq(temp_pointer_p, temp_pointer_q))
			{
				return false;
			}
		}

		return temp_pointer_p && temp_pointer_q;
	}
}

// Placeholdery

Poly PolyMul(const Poly *p, const Poly *q)
{
	assert(false);
	return PolyZero();
}

Poly PolyNeg(const Poly *p)
{
	assert(false);
	return PolyZero();
}

Poly PolySub(const Poly *p, const Poly *q)
{
	assert(false);
	return PolyZero();
}

poly_exp_t PolyDegBy(const Poly *p, unsigned var_idx)
{
	assert(false);
	return 0;
}

poly_exp_t PolyDeg(const Poly *p)
{
	assert(false);
	return 0;
}

Poly PolyAt(const Poly *p, poly_coeff_t x)
{
	assert(false);
	return PolyZero();
}
