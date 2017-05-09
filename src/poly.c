#include "poly.h"

// Pomocnicze funkcje

static Mono MonoZero()
{
	return (Mono) {.p = PolyZero(), .exp = 0};
}

static bool MonoIsZero(const Mono *m)
{
	return PolyIsZero(&(m->p));
}

static bool MonoIsConst(const Mono *m)
{
	return PolyIsCoeff(&(m->p));
}

static bool MonoIsEq(const Mono *m, const Mono *n)
{
	return (m->exp == n->exp) && PolyIsEq(&(m->p), &(n->p));
}

static void MonoArrayDestroy(unsigned count, Mono monos[])
{
	for (unsigned i = 0; i < count; i++)
	{
		MonoDestroy(&monos[i]);
		monos[i] = MonoZero();
	}

	free(monos);
}

static Mono* MonoCreateArray(unsigned count)
{
	Mono* monos = (Mono*) malloc(count * sizeof(Mono));
	assert(monos);
	return monos;
}

static Mono* MonoCloneArray(unsigned count, const Mono monos[])
{
	Mono* monos_clone = MonoCreateArray(count);

	for (unsigned i = 0; i < count; i++)
	{
		monos_clone[i] = MonoClone(&(monos[i]));
	}

	return monos_clone;
}

/*
 * TODO
 * Przejmuje na własność zawartość tablicy @p monos.
 */
static Mono* MonoCopyArray(unsigned count, const Mono monos[])
{
	Mono* monos_copy = MonoCreateArray(count);

	for (unsigned i = 0; i < count; i++)
	{
		monos_copy[i] = monos[i];
	}

	return monos_copy;
}

static void MonoArraySwap(unsigned index1, unsigned index2, Mono monos[])
{
	if (index1 != index2)
	{
		Mono temp_mono = monos[index1];
		monos[index1] = monos[index2];
		monos[index2] = temp_mono;
	}
}

static unsigned MonoArrayPartition(unsigned left, unsigned right, Mono monos[])
{
	poly_exp_t pivot_exp = monos[left].exp;
    unsigned i = left + 1;
	unsigned j = right;

	while (i < j)
	{
		while (i < j && monos[i].exp <= pivot_exp)
		{
			i++;
		}

		while (i < j && monos[j].exp >= pivot_exp)
		{
			j--;
		}

		if (i < j)
		{
			MonoArraySwap(i, j, monos);
		}
    }

	if (monos[i].exp < pivot_exp)
	{
		MonoArraySwap(left, i, monos);
		return i;
	}

	else
	{
		MonoArraySwap(left, i - 1, monos);
		return i - 1;
	}
}

void MonoQuickSortArrayByExp(unsigned left, unsigned right, Mono monos[])
{
	if (left < right)
	{
		unsigned partition_point = MonoArrayPartition(left, right, monos);
		if (partition_point)
		{
			MonoQuickSortArrayByExp(left, partition_point - 1, monos);
		}
		MonoQuickSortArrayByExp(partition_point + 1, right, monos);
	}
}

static Mono MonoAdd(const Mono *m, const Mono *n)
{
	assert(m->exp == n->exp);

	return (Mono) {.p = PolyAdd(&(m->p), &(n->p)), .exp = m->exp};
}

/*
 * TODO
 * Przejmuje na własność zawartość tablicy @p monos.
 */
static Mono* MonoSortArrayByExp(unsigned count, const Mono monos[])
{
	Mono* new_monos = MonoCopyArray(count, monos);
	MonoQuickSortArrayByExp(0, count - 1, new_monos);
	return new_monos;
}

/*
 * TODO
 * Modyfikuje zawartość tablicy @p monos.
 */
static void MonoArrayReduceLikeTerms(unsigned count, Mono monos[])
{
	for (unsigned i = 0; i < count - 1; i++)
	{
		if (monos[i].exp == monos[i+1].exp)
		{
			Mono new_mono = MonoAdd(&monos[i], &monos[i+1]);
			MonoDestroy(&monos[i]);
			MonoDestroy(&monos[i+1]);
			monos[i] = MonoZero();
			monos[i+1] = new_mono;
		}
	}
}

/*
 * TODO
 * Przejmuje na własność zawartość tablicy @p monos.
 */
static Mono* MonoArrayReduceZeroTerms(unsigned count, Mono monos[], unsigned *new_count)
{
	unsigned reduced_terms = 0;
	for (unsigned i = 0; i < count; i++)
	{
		if (MonoIsZero(&monos[i]))
		{
			reduced_terms++;
		}
	}

	*new_count = count - reduced_terms;

	if (*new_count)
	{
		Mono* new_monos = MonoCreateArray(*new_count);
		unsigned i = 0;
		unsigned j = 0;

		while (i < *new_count && j < count)
		{
			while (j < count && MonoIsZero(&monos[j]))
			{
				j++;
			}

			new_monos[i] = monos[j];
			i++;
			j++;
		}

		return new_monos;
	}

	else
	{
		return NULL;
	}
}

/*
 * TODO
 * Przejmuje na własność zawartość tablicy @p monos.
 */
static Mono* MonoOptimizedArray(unsigned count, const Mono monos[], unsigned *new_count)
{
	Mono* sorted_monos = MonoSortArrayByExp(count, monos);
	MonoArrayReduceLikeTerms(count, sorted_monos);
	Mono* new_monos = MonoArrayReduceZeroTerms(count, sorted_monos, new_count);
	free(sorted_monos);
	return new_monos;
}

// Główne funkcje biblioteki

void PolyDestroy(Poly *p)
{
	MonoArrayDestroy(p->count, p->monos);
}

Poly PolyClone(const Poly *p)
{
	if (PolyIsCoeff(p))
	{
		return PolyFromCoeff(p->coeff);
	}

	else
	{
		return (Poly)
		{
			.monos = MonoCloneArray(p->count, p->monos),
			.count = p->count,
			.coeff = 0
		};
	}
}

Poly PolyAdd(const Poly *p, const Poly *q)
{
	if (PolyIsCoeff(p) && PolyIsCoeff(q))
	{
		return (Poly) {.monos = NULL, .count = 0, .coeff = p->coeff + q->coeff};
	}

	else if (PolyIsCoeff(q))
	{
		unsigned new_count = p->count + 1;
		Mono* new_monos = MonoCreateArray(new_count);

		new_monos[0] = MonoFromPoly(q, 0);

		for (unsigned i = 1; i < new_count; i++)
		{
			new_monos[i] = MonoClone(&(p->monos[i-1]));
		}

		Poly new_poly = PolyAddMonos(new_count, new_monos);
		free(new_monos);
		return new_poly;
	}

	else if (PolyIsCoeff(p))
	{
		return PolyAdd(q, p);
	}

	else
	{
		unsigned new_count = p->count + q->count;
		Mono* new_monos = MonoCreateArray(new_count);

		unsigned i = 0;

		while (i < p->count)
		{
			new_monos[i] = MonoClone(&(p->monos[i]));
			i++;
		}

		while (i < new_count)
		{
			new_monos[i] = MonoClone(&(q->monos[i - p->count]));
			i++;
		}

		Poly new_poly = PolyAddMonos(new_count, new_monos);
		free(new_monos);

		return new_poly;
	}
}

Poly PolyAddMonos(unsigned count, const Mono monos[])
{
	unsigned new_count;
	Mono* new_monos = MonoOptimizedArray(count, monos, &new_count);

	if (new_count == 0)
	{
		assert(new_monos == NULL);
		return PolyZero();
	}

	else if (new_count == 1 && MonoIsConst(&new_monos[0]))
	{
		poly_coeff_t coeff = new_monos[0].p.coeff;
		MonoArrayDestroy(1, new_monos);
		return PolyFromCoeff(coeff);
	}

	else
	{
		return (Poly) {.monos = new_monos, .count = new_count, .coeff = 0};
	}
}

Poly PolyMul(const Poly *p, const Poly *q)
{
	/* TODO */
	assert(false);
	return PolyZero();
}

Poly PolyNeg(const Poly *p)
{
	/* TODO */
	assert(false);
	return PolyZero();
}

Poly PolySub(const Poly *p, const Poly *q)
{
	/* TODO */
	assert(false);
	return PolyZero();
}

poly_exp_t PolyDegBy(const Poly *p, unsigned var_idx)
{
	/* TODO */
	assert(false);
	return 0;
}

poly_exp_t PolyDeg(const Poly *p)
{
	/* TODO */
	assert(false);
	return 0;
}

bool PolyIsEq(const Poly *p, const Poly *q)
{
	if (PolyIsCoeff(p) && PolyIsCoeff(q))
	{
		return p->coeff == q->coeff;
	}

	else if (p->count != q->count)
	{
		return false;
	}

	else
	{
		for (unsigned i = 0; i < p->count; i++)
		{
			if (!MonoIsEq(&(p->monos[i]), &(q->monos[i])))
			{
				return false;
			}
		}

		return true;
	}
}

Poly PolyAt(const Poly *p, poly_coeff_t x)
{
	/* TODO */
	assert(false);
	return PolyZero();
}
