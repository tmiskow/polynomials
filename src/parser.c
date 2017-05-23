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
static ParserResult ParseConstMono(int *col, Mono *m);

/** TODO */
static ParserResult ParseMono(int *col, Mono *m);

/** TODO */
static ParserResult ParsePoly(int *col, Poly *p);

/* IMPLEMENTACJA FUNKCJI POMOCNICZYCH */

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
	assert(CharIsStartOfCoeff(CharPeek()));

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

static ParserResult ParseConstMono(int *col, Mono *m)
{
	assert(CharIsStartOfCoeff(CharPeek()));

	poly_coeff_t coeff;
	ParserResult parser_result = ParseCoeff(col, &coeff);

	if (parser_result == PARSER_ERROR)
	{
		return PARSER_ERROR;
	}

	else
	{
		Poly temp_poly = PolyFromCoeff(coeff);
		*m = MonoFromPoly(&temp_poly, 0);
		return PARSER_SUCCESS;
	}
}

static ParserResult ParseMono(int *col, Mono *m)
{
	char c = CharPeek();

	if (CharIsStartOfCoeff(c))
	{
		return ParseConstMono(col, m);
	}
	else if (c == '(')
	{
		// TODO
		ParseChar(col);
		poly_coeff_t coeff;

		if (CharIsStartOfCoeff(CharPeek()))
		{
			if (ParseCoeff(col, &coeff) != PARSER_ERROR)
			{
				Poly temp_poly = PolyFromCoeff(coeff);

				if (CharPeek() == ',')
				{
					ParseChar(col);
					poly_exp_t exp;

					if (ParseMonoExp(col, &exp) != PARSER_ERROR)
					{
						*m = MonoFromPoly(&temp_poly, exp);

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
	else
	{
		return PARSER_ERROR;
	}
}

static ParserResult ParsePoly(int *col, Poly *p)
{
	//TODO
	Mono monos[20];
	ParserResult result = PARSER_SUCCESS;
	int i = 0;

	while (result == PARSER_SUCCESS)
	{
		result = ParseMono(col, &(monos[i]));
		i++;
	}

	*p = PolyAddMonos(i, monos);

	return PARSER_SUCCESS;
}

/* IMPLEMENTACJA FUNKCJI GŁÓWNYCH */

ParserResult ParseLinePoly(Poly *p)
{
	/* TODO */

	int col = 1;
	Mono m;
	ParserResult parser_result = ParseMono(&col, &m);

	if (parser_result == PARSER_ERROR)
	{
		ErrorParserPoly(1, col);
	}
	else if (parser_result == PARSER_SUCCESS)
	{
		// TODO
		*p = PolyAddMonos(1, &m);
	}

	return parser_result;
}
