#include "poly.h"

// Struktury

struct Poly
{
	Mono **monos; ///< wskaźnik do tablicy jednomianów
	unsigned count; ///< liczba jednomianów w @monos
	poly_coeff_t coeff; ///< współczynnik liczbowy
};

struct Mono
{
    Poly p; ///< współczynnik
    poly_exp_t exp; ///< wykładnik
};

// Główne funkcje biblioteki

void PolyDestroy(Poly *p)
{
	/* TODO */
	assert(false);
}

Poly PolyClone(const Poly *p)
{
	/* TODO */
	assert(false);
	return PolyZero();
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
