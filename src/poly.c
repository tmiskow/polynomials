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
	return PolyIsCoeff(&(m->p)) && m->exp == 0;
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

static Mono MonoAdd(const Mono *m, const Mono *n)
{
	assert(m->exp == n->exp);

	return (Mono) {.p = PolyAdd(&(m->p), &(n->p)), .exp = m->exp};
}

static int MonoCompareByExp(const void *m, const void *n)
{
	poly_exp_t m_exp = ((Mono*) m)->exp;
	poly_exp_t n_exp = ((Mono*) n)->exp;

	return (m_exp - n_exp);
}

/*
 * TODO
 * Przejmuje na własność zawartość tablicy @p monos.
 */
static Mono* MonoSortArrayByExp(unsigned count, const Mono monos[])
{
	Mono* new_monos = MonoCopyArray(count, monos);
	qsort(new_monos, count, sizeof (Mono), MonoCompareByExp);
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

static Mono MonoMul(const Mono *m, const Mono *n)
{
	if (MonoIsZero(m) || MonoIsZero(n))
	{
		return MonoZero();
	}

	else
	{
		Poly new_poly = PolyMul(&(m->p), &(n->p));

		if (PolyIsZero(&new_poly))
		{
			return MonoZero();
		}

		else
		{
			return (Mono) {.p = new_poly, .exp = m->exp + n->exp};
		}
	}
}

static Poly PolyMulByMono(const Poly *p, const Mono *m)
{
	if (PolyIsZero(p) || MonoIsZero(m))
	{
		return PolyZero();
	}

	else if (PolyIsCoeff(p) && MonoIsConst(m))
	{
		poly_coeff_t new_coeff = ((m->p).coeff) * p->coeff;
		return PolyFromCoeff(new_coeff);
	}

	else if (PolyIsCoeff(p))
	{
		Mono temp_monos[1];
		temp_monos[0] = (Mono) {.p = PolyMul(p, &(m->p)), .exp = m->exp};
		return PolyAddMonos(1, temp_monos);
	}

	else
	{
		unsigned new_count = p->count;
		Mono* new_monos = MonoCreateArray(new_count);

		for (unsigned i = 0; i < new_count; i++)
		{
			new_monos[i] = MonoMul(m, &(p->monos[i]));
		}

		return (Poly) {.monos = new_monos, .count = new_count, .coeff = 0};
	}
}

Poly PolyCoeffFromMonoExp(const Mono *m, poly_coeff_t x)
{

	poly_coeff_t result = 1;
	poly_exp_t temp_exp = m->exp;

	while (temp_exp)
    {
        if (temp_exp & 1)
            result *= x;
        temp_exp >>= 1;
        x *= x;
    }

    return PolyFromCoeff(result);
}

static Poly MonoAt(const Mono *m, poly_coeff_t x)
{
	Poly temp_poly = PolyCoeffFromMonoExp(m, x);
	return PolyMul(&(m->p), &temp_poly);
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
	if (PolyIsZero(p) || PolyIsZero(q))
	{
		return PolyZero();
	}

	else if (PolyIsCoeff(p) && PolyIsCoeff(q))
	{
		return PolyFromCoeff((p->coeff)*(q->coeff));
	}

	else if (PolyIsCoeff(q))
	{
		unsigned new_count = p->count;
		Mono* new_monos = MonoCreateArray(new_count);

		for (unsigned i = 0; i < new_count; i++)
		{
			Mono temp_mono = p->monos[i];
			Poly temp_poly = temp_mono.p;
			new_monos[i] = (Mono)
			{
				.p = PolyMul(&temp_poly, q),
				.exp = temp_mono.exp
			};
		}

		return (Poly) {.monos = new_monos, .count = new_count, .coeff = 0};
	}

	else if (PolyIsCoeff(p))
	{
		return PolyMul(q, p);
	}

	else
	{
		Poly new_poly = PolyZero();

		for (unsigned i = 0; i < q->count; i++)
		{
			Mono temp_mono = q->monos[i];
			Poly mul_poly = PolyMulByMono(p, &temp_mono);
			Poly temp_poly = PolyAdd(&new_poly, &mul_poly);

			PolyDestroy(&mul_poly);
			PolyDestroy(&new_poly);

			new_poly = temp_poly;
		}

		return new_poly;
	}
}

Poly PolyNeg(const Poly *p)
{
	Poly temp_poly = PolyFromCoeff(-1);
	return PolyMul(p, &temp_poly);
}

Poly PolySub(const Poly *p, const Poly *q)
{
	Poly temp_poly = PolyNeg(q);
	Poly new_poly = PolyAdd(p, &temp_poly);
	PolyDestroy(&temp_poly);
	return new_poly;
}

poly_exp_t PolyDegBy(const Poly *p, unsigned var_idx)
{
	if (var_idx == 0)
	{
		if (PolyIsZero(p))
		{
			return -1;
		}

		else if (PolyIsCoeff(p))
		{
			return 0;
		}

		else
		{
			return (p->monos[p->count - 1]).exp;
		}
	}

	else
	{
		if (PolyIsCoeff(p))
		{
			return 0;
		}

		else
		{
			poly_exp_t max_exp = -1;

			for (unsigned i = 0; i < p->count; i++)
			{
				Poly temp_poly = (p->monos[i]).p;
				poly_exp_t temp_exp = PolyDegBy(&temp_poly, var_idx - 1);

				if (temp_exp > max_exp)
				{
					max_exp = temp_exp;
				}
			}

			return max_exp;
		}
	}
}

poly_exp_t PolyDeg(const Poly *p)
{
	if (PolyIsZero(p))
	{
		return -1;
	}

	else if (PolyIsCoeff(p))
	{
		return 0;
	}

	else
	{
		poly_exp_t max_deg = -1;

		for (unsigned i = 0; i < p->count; i++)
		{
			Mono temp_mono = p->monos[i];
			poly_exp_t temp_deg = temp_mono.exp + PolyDeg(&(temp_mono.p));

			if (temp_deg > max_deg)
			{
				max_deg = temp_deg;
			}
		}

		return max_deg;
	}
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
	if (PolyIsCoeff(p))
	{
		return PolyFromCoeff(p->coeff);
	}

	else
	{
		Poly new_poly = PolyZero();

		for (unsigned i = 0; i < p->count; i++)
		{
			Poly mono_poly = MonoAt(&(p->monos[i]), x);
			Poly temp_poly = PolyAdd(&new_poly, &mono_poly);
			PolyDestroy(&new_poly);
			new_poly = temp_poly;
		}

		return new_poly;
	}
}
