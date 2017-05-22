/** @file
   TODO

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date TODO
*/

#include "poly.h"
#include "stakc.h"
#include "calc.h"

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

void CalcPrint(Stack *stack)
{
	Poly p = StackPop(stack);
	CalcPrintPoly(p);
	printf("\n");
	StackPush(stack, &p);
}
