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
#include <limits.h>

/** TODO */
typedef long long parser_coeff_t;

/** TODO */
typedef long parser_exp_t;

/* DEKLARACJE FUNKCJI POMOCNICZYCH */

/** TODO */
static void ParserSkipLine();

/** TODO */
static bool ParserCoeffIsInRange(parser_coeff_t parser_coeff);

/** TODO */
static bool ParserExpIsInRange(parser_exp_t parser_exp);

/** TODO */
static bool CharIsStartOfCoeff(char c);

/** TODO */
static poly_coeff_t CharToCoeff(char c);

/** TODO */
static poly_exp_t CharToExp(char c);

/** TODO */
static char CharPeek();

/** TODO */
static char ParseChar(int *col);

/** TODO */
static ParserResult ParseCoeff(int *col, poly_coeff_t *coeff);

/** TODO */
static ParserResult ParseMonoExp(int *col, poly_exp_t *exp);

/** TODO */
static ParserResult ParseMono(int *col, Mono *m);

/** TODO */
static ParserResult ParsePoly(int *col, Poly *p);

/* IMPLEMENTACJA FUNKCJI POMOCNICZYCH */

static void ParserSkipLine()
{
	// TODO
	while (getchar() != '\n');
}

static bool ParserCoeffIsInRange(parser_coeff_t parser_coeff)
{
	return (LONG_MIN <= parser_coeff && parser_coeff <= LONG_MAX);
}

static bool ParserExpIsInRange(parser_exp_t parser_exp)
{
	return (0 <= parser_exp && parser_exp <= INT_MAX);
}

static inline bool CharIsStartOfCoeff(char c)
{
	return isdigit(c) || c == '-';
}

static poly_exp_t CharToExp(char c)
{
	assert(isdigit(c));
	return (poly_coeff_t) c - '0';
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
	if (CharIsStartOfCoeff(CharPeek()))
	{
		parser_coeff_t parser_coeff = 0;
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
			parser_coeff = 10 * (parser_coeff) + CharToCoeff(c);

			if (!ParserCoeffIsInRange(parser_coeff))
			{
				return PARSER_ERROR;
			}
		}

		if (is_negative)
		{
			parser_coeff *= -1;
		}

		*coeff = (poly_coeff_t) parser_coeff;
		return PARSER_SUCCESS;
	}
	else
	{
		return PARSER_ERROR;
	}
}

static ParserResult ParseMonoExp(int *col, poly_exp_t *exp)
{
	if (!isdigit(CharPeek()))
	{
		return PARSER_ERROR;
	}
	else
	{
		parser_exp_t parser_exp = 0;

		while (isdigit(CharPeek()))
		{
			char c = ParseChar(col);
			parser_exp = 10 * parser_exp + CharToExp(c);

			if (!ParserExpIsInRange(parser_exp))
			{
				return PARSER_ERROR;
			}
		}

		*exp = parser_exp;
		return PARSER_SUCCESS;
	}
}

static ParserResult ParseMono(int *col, Mono *m)
{
	if (CharPeek() == '(')
	{
		ParseChar(col);
		Poly p;
		if (ParsePoly(col, &p) != PARSER_ERROR)
		{
			if (CharPeek() == ',')
			{
				ParseChar(col);
				poly_exp_t exp;

				if (ParseMonoExp(col, &exp) != PARSER_ERROR)
				{
					*m = MonoFromPoly(&p, exp);

					if (CharPeek() == ')')
					{
						ParseChar(col);
						return PARSER_SUCCESS;
					}
					else
					{
						return PARSER_ERROR;
					}
				}
				else
				{
					return PARSER_ERROR;
				}
			}
			else
			{
				return PARSER_ERROR;
			}
		}
		else
		{
			return PARSER_ERROR;
		}
	}
	else
	{
		return PARSER_ERROR;
	}
}

static ParserResult ParsePoly(int *col, Poly *p)
{
	if (CharIsStartOfCoeff(CharPeek()))
	{
		poly_coeff_t coeff;

		if (ParseCoeff(col, &coeff) == PARSER_ERROR)
		{
			return PARSER_ERROR;
		}
		else
		{
			*p = PolyFromCoeff(coeff);
			return PARSER_SUCCESS;
		}
	}
	else if (CharPeek() == '(')
	{
		unsigned size = 10;
		Mono* monos = (Mono*) calloc(size, sizeof(Mono));
		assert(monos);

		unsigned count = 0;

		ParserResult parser_result = PARSER_SUCCESS;
		do
		{
			if (CharPeek() == '+')
			{
				ParseChar(col);
			}

			parser_result = ParseMono(col, &(monos[count]));
			count++;

			if (count == size - 1)
			{
				size *= 2;
				monos = (Mono*) realloc(monos, size * sizeof(Mono));
				assert(monos);
			}
		} while (parser_result == PARSER_SUCCESS && CharPeek() == '+');

		*p = PolyAddMonos(count, monos);
		free(monos);
		return parser_result;
	}
	else
	{
		*p = PolyZero();
		return PARSER_ERROR;
	}
}

/* IMPLEMENTACJA FUNKCJI GŁÓWNYCH */

ParserResult ParseLinePoly(Poly *p)
{
	/* TODO */

	int col = 1;

	ParserResult parser_result = ParsePoly(&col, p);

	if (parser_result == PARSER_ERROR || CharPeek() != '\n')
	{
		PolyDestroy(p);
		ErrorParserPoly(1, col);
		ParserSkipLine();
		return PARSER_ERROR;
	}
	else
	{
		return parser_result;
	}
}
