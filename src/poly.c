#include "poly.h"

// Pomocnicze funkcje

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

// Główne funkcje biblioteki

void PolyDestroy(Poly *p)
{
	/* TODO */
	assert(false);
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
	/* TODO */
	assert(false);
	return PolyZero();
}

Poly PolyAddMonos(unsigned count, const Mono monos[])
{
	/* TODO */
	assert(false);
	return PolyZero();
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
	/* TODO */
	assert(false);
	return 0;
}

Poly PolyAt(const Poly *p, poly_coeff_t x)
{
	/* TODO */
	assert(false);
	return PolyZero();
}
