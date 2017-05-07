/** @file
   Implementacja funkcji pomocniczych i dłuższych funkcji z poly.h

   @author TODO
   @copyright Uniwersytet Warszawski
   @date TODO
*/

#include "poly.h"

// Funkcje pomocnicze

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
  * @return Czy wielomian jest stały?
  */
 static bool MonoIsConst(const Mono *m)
 {
	 return (m->exp == 0 && PolyIsCoeff(&(m->p)));
 }

/**
 * Rekurencyjnie usuwa z pamięci jednomiany znajdujące się w liście.
 * @param[in] m : pierwszy jednomian w liście
 */
static void MonoListDestroy(Mono *m)
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
static Mono* MonoListClone(const Mono *m)
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

/**
 * Dodaje dwa jednomiany posiadające te same wykładniki.
 * @param[in] m : jednomian
 * @param[in] n : jednomian
 * @return `m + n`
 */
static inline Mono MonoAdd(const Mono* m, const Mono* n)
{
	assert(m->exp == n->exp);
	return (Mono) {.exp = m->exp, .p = PolyAdd(&(m->p), &(n->p)), .next = NULL};
}

/**
 * Wstawia jednomian na odpowiednie miejsce w liście.
 * @param[in] mono_list : lista jednomianów
 * @param[in] m : jednomian
 * @return zaktualizowana lista
 */
static Mono* MonoListAddMono(Mono* mono_list, const Mono* m)
{
	Mono* new_mono = (Mono*) malloc(sizeof (Mono));

	if (mono_list)
	{
		Mono* temp_pointer = mono_list;

		if (temp_pointer->exp == m->exp)
		{
			*new_mono = MonoAdd(temp_pointer, m);

			if (MonoIsZero(new_mono))
			{
				return temp_pointer->next;
			}

			else
			{
				new_mono->next = temp_pointer->next;
				return new_mono;
			}
		}

		else if (temp_pointer->exp > m->exp)
		{
			*new_mono = (Mono)
			{
				.exp = m->exp,
				.p = m->p,
				.next = temp_pointer
			};
			return new_mono;
		}

		else
		{
			// Znajdź w liście ostatni jednomian o mniejszym wykładniku.
			while (temp_pointer->next && (temp_pointer->next)->exp < m->exp)
			{
				temp_pointer = temp_pointer->next;
			}

			if (temp_pointer->next == NULL)
			{
				*new_mono = (Mono)
				{
					.exp = m->exp,
					.p = m->p,
					.next = NULL
				};
				temp_pointer->next = new_mono;
			}

			else if ((temp_pointer->next)->exp == m->exp)
			{
				*new_mono = MonoAdd(temp_pointer->next, m);

				if (MonoIsZero(new_mono))
				{
					temp_pointer->next = (temp_pointer->next)->next;;
				}

				else
				{
					new_mono->next = (temp_pointer->next)->next;
					temp_pointer->next = new_mono;
				}
			}

			else
			{
				*new_mono = (Mono)
				{
					.exp = m->exp,
					.p = m->p,
					.next = (temp_pointer->next)->next
				};
				temp_pointer->next = new_mono;
			}

			return mono_list;
		}
	}

	else
	{
		*new_mono = (Mono)
		{
			.exp = m->exp,
			.p = m->p,
			.next = NULL
		};
		return new_mono;
	}
}

/**
 * Dodaje pojedynczy jednomian do wielomianu.
 * @param[in] p : wielomian
 * @param[in] m : jednomian
 * @return `p + m`
 */
static Poly PolyAddMono(const Poly *p, const Mono *m)
{
	if (PolyIsCoeff(p))
	{
		if (MonoIsConst(m))
		{
			poly_coeff_t new_coeff = p->coeff + (m->p).coeff;
			return (Poly) {.mono_list = NULL, .coeff = new_coeff};
		}

		else
		{
			Mono temp_mono = MonoFromPoly(p, 0);
			Poly temp_poly = (Poly)
			{
				.mono_list = MonoListAddMono((p->mono_list), m),
				.coeff = 0
			};
			return PolyAddMono(&temp_poly, &temp_mono);
		}
	}

	else
	{
		return (Poly) {
			.coeff = 0,
			.mono_list = MonoListAddMono(p->mono_list, m)
		};
	}
}

/**
 * Mnoży dwa jednomiany.
 * @param[in] m : jednomian
 * @param[in] n : jednomian
 * @return `n * n`
 */
static Mono MonoMul(const Mono *m, const Mono *n)
{
	return (Mono)
	{
		.exp = (m->exp) + (n->exp),
		.p = PolyMul(&(m->p), &(n->p))
	};
}

/**
 * Tworzy jednomian składający się z jednego jednomianu.
 * Przejmuje na własność zawartość struktury wskazywanej przez @p m.
 * @param[in] m : jednomian
 * @return wielomian
 */
static Poly PolyFromMono(const Mono *m)
{
   if (MonoIsConst(m))
   {
	   return m->p;
   }

   else
   {
	   Poly p = PolyZero();
	   p = PolyAddMono(&p, m);
	   return p;
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
	if (!MonoIsZero(m))
	{
		if (PolyIsZero(p))
		{
			return PolyZero();
		}

		else if (PolyIsCoeff(p))
		{
			Mono* new_mono = (Mono*) malloc(sizeof (Mono));
			*new_mono = MonoClone(m);
			return PolyFromMono(new_mono);
		}

		else
		{
			Poly new_poly = PolyZero();
			Mono* temp_pointer = p->mono_list;

			while (temp_pointer)
			{
				Mono temp_mono = MonoMul(temp_pointer, m);
				new_poly = PolyAddMono(&new_poly, &temp_mono);
				temp_pointer = temp_pointer->next;
			}

			return new_poly;
		}
	}

	else
	{
		return PolyZero();
	}
}

/**
 * Zwraca najwyższy wykładnik na liście jednomianów (-1 dla pustej listy).
 * @param[in] mono_list : pierwszy jednomian w liście
 * @return : wykładnik ostatniego wielomianu na liście
 */
static poly_exp_t MonoListHighestExp(Mono *mono_list)
{
	if (mono_list)
	{
		Mono* temp_pointer = mono_list;
		while(temp_pointer->next)
		{
			temp_pointer = temp_pointer->next;
		}
		return temp_pointer->exp;
	}

	else
	{
		return -1;
	}
}

/**
 * Sprawdza równość dwóch jednomianów.
 * @param[in] m : jednomian
 * @param[in] n : jednomian
 * @return `m = n`
 */
static bool MonoIsEq(const Mono *m, const Mono *n)
{
	return (m->exp == n->exp && PolyIsEq(&(m->p), &(n->p)));
}

/**
 * Sprawdza równość dwóch list jednomianów.
 * @param[in] mono_list_1 : jednomian
 * @param[in] mono_list_2 : jednomian
 * @return `mono_list_1 = mono_list_2`
 */
static bool MonoListIsEq(const Mono *mono_list_1, Mono *mono_list_2)
 {
 	if (mono_list_1 && mono_list_2)
	{
		return	MonoListIsEq(mono_list_1->next, mono_list_2->next) &&
				MonoIsEq(mono_list_1, mono_list_2);
	}

	else if (mono_list_1 || mono_list_2)
	{
		return false;
	}

	else
	{
		return true;
	}
 }

 /**
  * Wylicza wartość jednomianu w punkcie @p x.
  * Wstawia pod pierwszą zmienną jednomianu wartość @p x.
  * Podnosi @p x do odpowiedniej potęgi i wymnaża wielomian o tę liczbę.
  * @param[in] m : jednomian
  * @param[in] x : argument
  * @return wielomian
  */
static Poly MonoAt(const Mono *m, poly_coeff_t x)
{
	poly_coeff_t x_pow = 1;

	for (unsigned i = 0; i < m->exp; i++)
	{
		x_pow *= x;
	}

	Poly temp_poly = PolyFromCoeff(x_pow);
	return PolyMul(&(m->p), &temp_poly);
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

Poly PolyAdd(const Poly *p, const Poly *q)
{
	if (PolyIsCoeff(p) && PolyIsCoeff(q))
	{
		return (Poly) {.mono_list = NULL, .coeff = p->coeff + q->coeff};
	}

	else if (PolyIsCoeff(p))
	{
		Mono temp_mono = MonoFromPoly(p, 0);
		return PolyAddMono(q, &temp_mono);
	}

	else if (PolyIsCoeff(q))
	{
		Mono temp_mono = MonoFromPoly(q, 0);
		return PolyAddMono(p, &temp_mono);
	}

	else
	{
		Poly new_poly = PolyClone(p);
		Mono* temp_pointer = q->mono_list;

		while (temp_pointer)
		{
			new_poly = PolyAddMono(&new_poly, temp_pointer);
			temp_pointer = temp_pointer->next;
		}

		return new_poly;
	}
}

Poly PolyAddMonos(unsigned count, const Mono monos[])
{
	Mono* mono_list = NULL;

	for (unsigned i = 0; i < count; i++)
	{
		mono_list = MonoListAddMono(mono_list, &(monos[i]));
	}

	return (Poly) {.mono_list = mono_list, .coeff = 0};
}

Poly PolyMul(const Poly *p, const Poly *q)
{
	if (PolyIsZero(p) || PolyIsZero(q))
	{
		return PolyZero();
	}

	else if (PolyIsCoeff(p) && PolyIsCoeff(q))
	{
		return PolyFromCoeff((p->coeff) * (q->coeff));
	}

	else if (PolyIsCoeff(p))
	{
		Poly temp_poly = PolyClone(p);
		Mono temp_mono = MonoFromPoly(&temp_poly, 0);
		return PolyMulByMono(q, &temp_mono);
	}

	else if (PolyIsCoeff(q))
	{
		return PolyMul(q, p);
	}

	else
	{
		Poly new_poly = PolyZero();
		Mono* temp_pointer = q->mono_list;

		while (temp_pointer)
		{
			Poly temp_poly = PolyMulByMono(p, temp_pointer);
			new_poly = PolyAdd(&new_poly, &temp_poly);
		}

		return new_poly;
	}
}

Poly PolyNeg(const Poly *p)
{
	Poly temp_poly = PolyFromCoeff(-1);
	return PolyMul(&temp_poly, p);
}

Poly PolySub(const Poly *p, const Poly *q)
{
	Poly neg_q = PolyNeg(q);
	return PolyAdd(p, &neg_q);
}

poly_exp_t PolyDegBy(const Poly *p, unsigned var_idx)
{
	if (var_idx == 0)
	{
		return MonoListHighestExp(p->mono_list);
	}

	else
	{
		Mono* temp_pointer = p->mono_list;
		poly_exp_t max_exp = -1;

		while (temp_pointer)
		{
			poly_exp_t temp_exp = PolyDegBy(&(temp_pointer->p), var_idx - 1);
			max_exp = max_exp > temp_exp ? max_exp : temp_exp;
		}

		return max_exp;
	}
}

poly_exp_t PolyDeg(const Poly *p)
{
	if (PolyIsCoeff(p))
	{
		return -1;
	}

	unsigned i = 0;
	poly_exp_t last_deg = 0;
	poly_exp_t deg = 0;
	while (last_deg != -1)
	{
		last_deg = PolyDegBy(p, i);
		deg += last_deg;
		i++;
	}

	return deg + 1;
}

bool PolyIsEq(const Poly *p, const Poly *q)
{
	if (PolyIsCoeff(p) && PolyIsCoeff(q))
	{
		return (p->coeff == q->coeff);
	}

	else if (PolyIsCoeff(p) || PolyIsCoeff(q))
	{
		return false;
	}

	else
	{
		return MonoListIsEq(p->mono_list, q->mono_list);
	}
}

Poly PolyAt(const Poly *p, poly_coeff_t x)
{
	Poly new_poly = PolyZero();
	Mono* temp_pointer = p->mono_list;

	while(temp_pointer)
	{
		Poly temp_poly = MonoAt(temp_pointer, x);
		new_poly = PolyAdd(&new_poly, &temp_poly);
		temp_pointer = temp_pointer->next;
	}

	return new_poly;
}
