/** @file
   TODO

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date TODO
*/

#include "poly.h"
#include "stack.h"
#include "calc.h"
#include "error.h"
#include "parser.h"

#include <stdio.h>

/* DEKLARACJE FUNKCJI POMOCNICZYCH */

/** TODO */
static void CalcPrintMono(const Mono* m);

/** TODO */
static void CalcPrintMonoArray(unsigned count, const Mono monos[]);

/** TODO */
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

ParserResult CalcAdd(Stack *stack)
{
	if (StackIsEmpty(stack))
	{
		return PARSER_ERROR;
	}
	else
	{
		Poly p1 = StackPop(stack);

		if (StackIsEmpty(stack))
		{
			StackPush(stack, &p1);
			return PARSER_ERROR;
		}
		else
		{
			Poly p2 = StackPop(stack);
			Poly poly = PolyAdd(&p1, &p2);
			StackPush(stack, &poly);
			PolyDestroy(&p1);
			PolyDestroy(&p2);
			return PARSER_SUCCESS;
		}
	}
}

ParserResult CalcPrint(Stack *stack)
{
	if (StackIsEmpty(stack))
	{
		return PARSER_ERROR;
	}
	else
	{
		Poly p = StackPop(stack);
		CalcPrintPoly(&p);
		printf("\n");
		StackPush(stack, &p);

		return PARSER_SUCCESS;
	}
}
