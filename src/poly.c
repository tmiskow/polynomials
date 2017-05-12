/** @file
   Implemnatcja funkcji z poly.h i statycznych funkcji pomocniczych

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date 2017-05-12
*/

#include "poly.h"

/**
 * Tworzy jednomian tożsamościowo równy zeru.
 * @return jednomian
 */
static Mono MonoZero()
{
	return (Mono) {.p = PolyZero(), .exp = 0};
}

/**
 * Sprawdza, czy jednomian jest tożsamościowo równy zeru.
 * @param[in] m : jednomian
 * @return Czy jednomian jest równy zero?
 */
static bool MonoIsZero(const Mono *m)
{
	return PolyIsZero(&(m->p));
}

/**
 * Sprawdza, czy jednomian jest stały.
 * @param[in] m : jednomian
 * @return Czy jednomian jest współczynnikiem?
 */
static bool MonoIsConst(const Mono *m)
{
	return PolyIsCoeff(&(m->p)) && m->exp == 0;
}

/**
 * Sprawdza równość dwóch jednomianów.
 * @param[in] m : jednomian
 * @param[in] n : jednomian
 * @return `m = n`
 */
static bool MonoIsEq(const Mono *m, const Mono *n)
{
	return (m->exp == n->exp) && PolyIsEq(&(m->p), &(n->p));
}

/**
 * Usuwa tablicę jednomianów z pamięci.
 * @param[in] count : liczba jednomianów (rozmiar tablicy)
 * @param[in] monos : tablica jednomianów
 */
static void MonoArrayDestroy(unsigned count, Mono monos[])
{
	for (unsigned i = 0; i < count; i++)
	{
		MonoDestroy(&monos[i]);
		monos[i] = MonoZero();
	}

	free(monos);
}

/**
 * Tworzy tablicę jednomianów.
 * @param[in] count : liczba jednomianów (rozmiar tablicy)
 * @return tablica jednomianów
 */
static Mono* MonoCreateArray(unsigned count)
{
	Mono* monos = (Mono*) malloc(count * sizeof(Mono));
	assert(monos);
	return monos;
}

/**
 * Robi pełną, głęboką kopię tablicy jednomianów.
 * @param[in] count : liczba jednomianów (rozmiar tablicy)
 * @param[in] monos : tablica jednomianów
 * @return skopiowana tablica
 */
static Mono* MonoCloneArray(unsigned count, const Mono monos[])
{
	Mono* monos_clone = MonoCreateArray(count);

	for (unsigned i = 0; i < count; i++)
	{
		monos_clone[i] = MonoClone(&(monos[i]));
	}

	return monos_clone;
}

/**
 * Robi płytką kopię tablicy jednomianów.
 * Przejmuje na własność zawartość tablicy @p monos.
 * @param[in] count : liczba jednomianów (rozmiar tablicy)
 * @param[in] monos : tablica jednomianów
 * @return skopiowana tablica
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

/**
 * Dodaje dwa jednomiany.
 * @param[in] m : jednomian
 * @param[in] n : jednomian
 * @return `m + n`
 */
static Mono MonoAdd(const Mono *m, const Mono *n)
{
	assert(m->exp == n->exp);

	return (Mono) {.p = PolyAdd(&(m->p), &(n->p)), .exp = m->exp};
}

static Mono* MonoArrayMerge(
	unsigned count_1,
	const Mono monos_1[],
	unsigned count_2,
	const Mono monos_2[]
) {
	unsigned new_count = count_1 + count_2;
	Mono* new_monos = MonoCreateArray(new_count);

	for (unsigned i = 0; i < count_1; i++)
	{
		new_monos[i] = monos_1[i];
	}

	for (unsigned i = count_1; i < new_count; i++)
	{
		new_monos[i] = monos_2[i - count_1];
	}

	return new_monos;
}

/**
 * Łączy dwa wielomiany w jeden.
 * Przejmuje na własność wielomiany @p p i @p q.
 * @param[in] p : wielomian
 * @param[in] q : wielomian
 * @return `p + q`
 */
static Poly PolyMerge(const Poly *p, const Poly *q)
{
	if (PolyIsCoeff(p) && PolyIsCoeff(q))
	{
		return PolyFromCoeff(p->coeff + q->coeff);
	}

	else if (PolyIsZero(q))
	{
		return *p;
	}

	else if (PolyIsCoeff(q))
	{
		Mono* temp_monos = MonoCreateArray(1);
		temp_monos[0] = MonoFromPoly(q, 0);
		Poly temp_poly = (Poly) {.monos = temp_monos, .count = 1, .coeff = 0};
		return PolyMerge(p, &temp_poly);
	}

	else if (PolyIsCoeff(p))
	{
		return PolyMerge(q, p);
	}

	else
	{
		Mono* new_monos;
		new_monos = MonoArrayMerge(p->count, p->monos, q->count, q->monos);
		unsigned new_count = p->count + q->count;

		Poly new_poly = PolyAddMonos(new_count, new_monos);

		free(new_monos);
		free(p->monos);
		free(q->monos);

		return new_poly;
	}
}

/**
 * Łączy dwa jednomiany w jeden.
 * Przejmuje na własność jednomiany @p m i @p n.
 * @param[in] m : jednomian
 * @param[in] n : jednomian
 * @return `m + n`
 */
static Mono MonoMerge(const Mono *m, const Mono *n)
{
	assert(m->exp == n->exp);
	return (Mono) {.p = PolyMerge(&(m->p), &(n->p)), .exp = m->exp};
}

/**
 * Porównuje dwa jednomiany pod względem wykładnika.
 * @p MonoCompareByExp jest wykorzystywana jako komparator funkcji @p qsort.
 * Z tego powodu musi przyjmować parametry `typu const void*`.
 * @param[in] m : jednomian
 * @param[in] n : jednomian
 * @return różnicę wykładników jednomianów @p m i @p n (`m->exp - n->exp`)
 */
static int MonoCompareByExp(const void *m, const void *n)
{
	poly_exp_t m_exp = ((Mono*) m)->exp;
	poly_exp_t n_exp = ((Mono*) n)->exp;

	return (m_exp - n_exp);
}

/**
 * Sortuję tablicę jednomianów rosnąco pod względem wykładnika.
 * Wykorzystuje do tego algorytm QuickSort.
 * Przejmuje na własność zawartość tablicy @p monos.
 * @param[in] count : liczba jednomianów (rozmiar tablicy)
 * @param[in] monos : tablica jednomianów
 * @return posortowana tablica
 */
static Mono* MonoSortArrayByExp(unsigned count, const Mono monos[])
{
	Mono* new_monos = MonoCopyArray(count, monos);
	qsort(new_monos, count, sizeof (Mono), MonoCompareByExp);
	return new_monos;
}

/**
 * Przeprowadza na posortowanej tablicy jednomianów redukcję wyrazów podobnych.
 * Łączy ze sobą jednomiany o tym samym wykładniku,
 * a na puste miejsca wstawia jednomiany zerowe.
 * Modyfikuje zawartość tablicy @p monos.
 * @param[in] count : liczba jednomianów (rozmiar tablicy)
 * @param[in] monos : tablica jednomianów
 */
static void MonoArrayReduceLikeTerms(unsigned count, Mono monos[])
{
	for (unsigned i = 0; i < count - 1; i++)
	{
		if (monos[i].exp == monos[i+1].exp)
		{
			Mono new_mono = MonoMerge(&monos[i], &monos[i+1]);
			monos[i] = MonoZero();
			monos[i+1] = new_mono;
		}
	}
}

static unsigned MonoArrayCountMonoZeros(unsigned count, const Mono monos[])
{
	unsigned mono_zeros_count = 0;
	for (unsigned i = 0; i < count; i++)
	{
		if (MonoIsZero(&monos[i]))
		{
			mono_zeros_count++;
		}
	}

	return mono_zeros_count;
}

/**
 * Usuwa jednomiany zerowe z tablicy jednomianów.
 * Tworzy nową tablicę, do której przekoopiowuje wszystkie jednomiany z @p monos
 * poza jednomianami zerowymi.
 * Przejmuje na własność zawartość tablicy @p monos.
 * @param[in] count : liczba jednomianów (rozmiar tablicy)
 * @param[in] monos : tablica jednomianów
 * @param[in] new_count : wskaźnik na zmienną, na którą będzie przypisana
 * liczba niezerowych jendomianów (rozmiar nowej tablicy)
 * @return tablica z usuniętymi jednomianami zerowymi
 */
static Mono* MonoArrayReduceZeroTerms(
	unsigned count,
	const Mono monos[],
	unsigned *new_count
) {
	*new_count = count - MonoArrayCountMonoZeros(count, monos);

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

/**
 * Optymalizuje tablicę jendomianów, aby można było ją zaszczepić w wielomianie.
 * Sortuje elementy rosnąco względem wykładników, redukuje wyrazy podobne i
 * usuwa z niej jednomiany zerowe.
 * Przejmuje na własność zawartość tablicy @p monos.
 * @param[in] count : liczba jednomianów (rozmiar tablicy)
 * @param[in] monos : tablica jednomianów
 * @param[in] new_count : wskaźnik na zmienną, na którą będzie przypisana
 * liczba niezerowych jendomianów (rozmiar nowej tablicy)
 * @return zoptymalizowana tablica
 */
static Mono* MonoOptimizedArray(
	unsigned count,
	const Mono monos[],
	unsigned *new_count
) {
	Mono* sorted_monos = MonoSortArrayByExp(count, monos);
	MonoArrayReduceLikeTerms(count, sorted_monos);
	Mono* new_monos = MonoArrayReduceZeroTerms(count, sorted_monos, new_count);
	free(sorted_monos);
	return new_monos;
}

/**
 * Mnoży dwa jednomiany.
 * @param[in] m : jednomian
 * @param[in] n : jednomian
 * @return `m * n`
 */
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

/**
 * Mnoży wielomian przez jednomian.
 * @param[in] p : wielomian
 * @param[in] m : jednomian
 * @return `p * m`
 */
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

/**
 * Tworzy wielomian stały równy co do wartości zmiennej jednomianu
 * w punkcie @p x podniesionej do swojego wykładnika @p e.
 * Nie bierze pod uwagę współczynnika jednomianu @p m.
 * Dla jednomianu `p * x_0^e` wynikiem jest wielomian stały równy
 * co do wartości `x^e`.
 * @param[in] m : jednomian
 * @param[in] x : wartość do podstawienia za zmienną jednomianu @p m
 * @return wielomian stały `x^e`
 */
static Poly PolyCoeffFromMonoExp(const Mono *m, poly_coeff_t x)
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

/**
 * Wylicza wartość jednomianu w punkcie @p x.
 * Wstawia pod pierwszą zmienną jednomianu wartość @p x.
 * W wyniku może powstać wielomian, jeśli współczynniki są wielomianem
 * i zmniejszane są indeksy zmiennych w takim wielomianie o jeden.
 * Formalnie dla jednomianu @f$p(x_1, x_2, \ldots) * x_0^e@f$ wynikiem jest
 * wielomian @f$p(x_0, x_1, \ldots) * x^e@f$.
 * @param[in] m : jednomian
 * @param[in] x : wartość do podstawienia za zmienną jednomianu @p m
 * @return @f$p(x_0, x_1, \ldots) * x^e@f$
 */
static Poly MonoAt(const Mono *m, poly_coeff_t x)
{
	Poly temp_poly = PolyCoeffFromMonoExp(m, x);
	return PolyMul(&(m->p), &temp_poly);
}

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
	Poly p_clone = PolyClone(p);
	Poly q_clone = PolyClone(q);
	return PolyMerge(&p_clone, &q_clone);
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

		if (new_count == 1 && MonoIsZero(new_monos))
		{
			free(new_monos);
			return PolyZero();
		}

		else
		{
			return (Poly) {.monos = new_monos, .count = new_count, .coeff = 0};
		}
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
	if (PolyIsZero(p))
	{
		return -1;
	}

	if (var_idx == 0)
	{
		if (PolyIsCoeff(p))
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
			PolyDestroy(&mono_poly);
			new_poly = temp_poly;
		}

		return new_poly;
	}
}
