/** @file
   Implementacja funkcji z calc.h i statycznych funkcji pomocniczych

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date 2017-05-25
*/

#include "poly.h"
#include "stack.h"
#include "calc.h"
#include "error.h"

#include <stdio.h>

/* DEKLARACJE FUNKCJI POMOCNICZYCH */

/**
 * Wypisuje na standardowe wyjście jednomian @p m.
 * @param[in] m : jednomian
*/
static void CalcPrintMono(const Mono* m);

/**
 * Wypisuje na standardowe wyjście tablicę jednomianów @p monos.
 * @param[in] count : liczba jednomianów (rozmiar tablicy)
 * @param[in] monos : tablica jednomianów
 */
static void CalcPrintMonoArray(unsigned count, const Mono monos[]);

/**
 * Wypisuje na standardowe wyjście wielomian @p p.
 * @param[in] p : wielomian
*/
static void CalcPrintPoly(const Poly *p);

/* IMPLEMENTACJA FUNKCJI POMOCNICZYCH */

static void CalcPrintMono(const Mono* m)
{
	printf("(");
	CalcPrintPoly(&(m->p));
	printf(",%d)", m->exp);
}

static void CalcPrintMonoArray(unsigned count, const Mono monos[])
{
	CalcPrintMono(&(monos[0]));

	for (unsigned i = 1; i < count; i++)
	{
		printf("+");
		CalcPrintMono(&(monos[i]));
	}
}

static void CalcPrintPoly(const Poly *p)
{
	if (PolyIsCoeff(p))
	{
		printf("%ld", p->coeff);
	}
	else
	{
		CalcPrintMonoArray(p->count, p->monos);
	}
}

/* IMPLEMENTACJA FUNKCJI GŁÓWNYCH */

FuncResult CalcZero(Stack *stack)
{
	Poly p = PolyZero();
	StackPush(stack, &p);
	return FUNC_SUCCESS;
}

FuncResult CalcIsCoeff(Stack *stack)
{
	if (StackIsEmpty(stack))
	{
		return FUNC_ERROR;
	}
	else
	{
		Poly p = StackPeek(stack);
		bool result = PolyIsCoeff(&p);
		printf("%d\n", result);
		return FUNC_SUCCESS;
	}
}

FuncResult CalcIsZero(Stack *stack)
{
	if (StackIsEmpty(stack))
	{
		return FUNC_ERROR;
	}
	else
	{
		Poly p = StackPeek(stack);
		bool result = PolyIsZero(&p);
		printf("%d\n", result);
		return FUNC_SUCCESS;
	}
}

FuncResult CalcClone(Stack *stack)
{
	if (StackIsEmpty(stack))
	{
		return FUNC_ERROR;
	}
	else
	{
		Poly p = StackPeek(stack);
		Poly poly_clone = PolyClone(&p);
		StackPush(stack, &poly_clone);
		return FUNC_SUCCESS;
	}
}

FuncResult CalcAdd(Stack *stack)
{
	if (StackIsEmpty(stack))
	{
		return FUNC_ERROR;
	}
	else
	{
		Poly p1 = StackPop(stack);

		if (StackIsEmpty(stack))
		{
			StackPush(stack, &p1);
			return FUNC_ERROR;
		}
		else
		{
			Poly p2 = StackPop(stack);
			Poly poly = PolyAdd(&p1, &p2);
			StackPush(stack, &poly);
			PolyDestroy(&p1);
			PolyDestroy(&p2);
			return FUNC_SUCCESS;
		}
	}
}

FuncResult CalcMul(Stack *stack)
{
	if (StackIsEmpty(stack))
	{
		return FUNC_ERROR;
	}
	else
	{
		Poly p1 = StackPop(stack);

		if (StackIsEmpty(stack))
		{
			StackPush(stack, &p1);
			return FUNC_ERROR;
		}
		else
		{
			Poly p2 = StackPop(stack);
			Poly poly = PolyMul(&p1, &p2);
			StackPush(stack, &poly);
			PolyDestroy(&p1);
			PolyDestroy(&p2);
			return FUNC_SUCCESS;
		}
	}
}

FuncResult CalcNeg(Stack *stack)
{
	if (StackIsEmpty(stack))
	{
		return FUNC_ERROR;
	}
	else
	{
		Poly p = StackPop(stack);
		Poly new_poly = PolyNeg(&p);
		PolyDestroy(&p);
		StackPush(stack, &new_poly);
		return FUNC_SUCCESS;
	}
}

FuncResult CalcSub(Stack *stack)
{
	if (StackIsEmpty(stack))
	{
		return FUNC_ERROR;
	}
	else
	{
		Poly p1 = StackPop(stack);

		if (StackIsEmpty(stack))
		{
			StackPush(stack, &p1);
			return FUNC_ERROR;
		}
		else
		{
			Poly p2 = StackPop(stack);
			Poly poly = PolySub(&p1, &p2);
			StackPush(stack, &poly);
			PolyDestroy(&p1);
			PolyDestroy(&p2);
			return FUNC_SUCCESS;
		}
	}
}

FuncResult CalcIsEq(Stack *stack)
{
	if (StackIsEmpty(stack))
	{
		return FUNC_ERROR;
	}
	else
	{
		Poly p1 = StackPop(stack);

		if (StackIsEmpty(stack))
		{
			StackPush(stack, &p1);
			return FUNC_ERROR;
		}
		else
		{
			Poly p2 = StackPop(stack);
			bool result = PolyIsEq(&p1, &p2);
			printf("%d\n", result);
			StackPush(stack, &p2);
			StackPush(stack, &p1);
			return FUNC_SUCCESS;
		}
	}
}

FuncResult CalcDeg(Stack *stack)
{
	if (StackIsEmpty(stack))
	{
		return FUNC_ERROR;
	}
	else
	{
		Poly p = StackPeek(stack);
		poly_exp_t deg = PolyDeg(&p);
		printf("%d\n", deg);
		return FUNC_SUCCESS;
	}
}

FuncResult CalcDegBy(Stack *stack, unsigned var_idx)
{
	if (StackIsEmpty(stack))
	{
		return FUNC_ERROR;
	}
	else
	{
		Poly p = StackPeek(stack);
		poly_exp_t deg = PolyDegBy(&p, var_idx);
		printf("%d\n", deg);
		return FUNC_SUCCESS;
	}
}

FuncResult CalcAt(Stack *stack, poly_coeff_t x)
{
	if (StackIsEmpty(stack))
	{
		return FUNC_ERROR;
	}
	else
	{
		Poly p = StackPop(stack);
		Poly new_poly = PolyAt(&p, x);
		PolyDestroy(&p);
		StackPush(stack, &new_poly);
		return FUNC_SUCCESS;
	}
}

FuncResult CalcPrint(Stack *stack)
{
	if (StackIsEmpty(stack))
	{
		return FUNC_ERROR;
	}
	else
	{
		Poly p = StackPeek(stack);
		CalcPrintPoly(&p);
		printf("\n");
		return FUNC_SUCCESS;
	}
}

FuncResult CalcPop(Stack *stack)
{
	if (StackIsEmpty(stack))
	{
		return FUNC_ERROR;
	}
	else
	{
		Poly p = StackPop(stack);
		PolyDestroy(&p);
		return FUNC_SUCCESS;
	}
}
