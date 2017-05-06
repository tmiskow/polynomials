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
			new_mono->next = temp_pointer->next;
			return new_mono;
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
				new_mono->next = (temp_pointer->next)->next;
				temp_pointer->next = new_mono;
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
