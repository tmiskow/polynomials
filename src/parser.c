/** @file
   TODO

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date TODO
*/

#include "poly.h"
#include "parser.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>

/* DEKLARACJE FUNKCJI POMOCNICZYCH */

/** TODO */
static bool CharIsStartOfCoeff(char c);

/** TODO */
static poly_coeff_t CharToCoeff(char c);

/** TODO */
static char CharPeek();

/** TODO */
static char ParseChar(int *col);

/** TODO */
static ParserResult ParseCoeff(int *col, poly_coeff_t *coeff);

/** TODO */
static Mono ParseMono(int *column_index);

/** TODO */
static Poly ParsePoly(int *column_index);

/* IMPLEMENTACJA FUNKCJI POMOCNICZYCH */

static inline bool CharIsStartOfCoeff(char c)
{
	return isdigit(c) || c == '-';
}

static poly_coeff_t CharToCoeff(char c)
{
	assert(isdigit(c));
	return (poly_coeff_t) c - '0';
}

static char CharPeek()
{
	char c = getchar();
	ungetc(c, stdin);
	return c;
}

static char ParseChar(int *col)
{
	(*col)++;
	return getchar();
}

static ParserResult ParseCoeff(int *col, poly_coeff_t *coeff)
{
	assert(CharIsStartOfCoeff(CharPeek()));

	*coeff = 0;
	bool is_negative = false;

	if (CharPeek() == '-')
	{
		is_negative = true;
		ParseChar(col);

		if (!isdigit(CharPeek()))
		{
			return PARSER_ERROR;
		}
	}

	while (isdigit(CharPeek()))
	{
		char c = ParseChar(col);
		*coeff = 10 * (*coeff) + CharToCoeff(c);
	}

	if (is_negative)
	{
		(*coeff) *= -1;
	}

	return PARSER_SUCCESS;
}

/* IMPLEMENTACJA FUNKCJI GŁÓWNYCH */

ParserResult ParseLinePoly(Poly *p)
{
	/* TODO */

	int col = 1;
	poly_coeff_t coeff = 0;
	ParseCoeff(&col, &coeff);
	*p = PolyFromCoeff(coeff);
	return PARSER_SUCCESS;
}
