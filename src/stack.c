/** @file
   Implementacja funkcji z stack.h i funkcji pomocniczych

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date 2017-05-25
*/

#include "poly.h"
#include "stack.h"

#include <stdlib.h>
#include <assert.h>

/**
 * Alokuje nowy element stosu.
 * @return wskaźnik na nowy element
 */
static StackItem* StackItemCreate();

/**
 * Tworzy nowy element stosu z wielomianu.
 * Przejmuje wielomian @p p na własność.
 * @param[in] p : wielomian
 * @param[in] next_item : wskaźnik na następny element
 * @return nowy element stosu
 */
static StackItem StackItemFromPoly(const Poly *p, StackItem *next_item);

/** @name Funkcje pomocnicze
 * Funkcje przeznaczone do użycia tylko przez funkcje główne tego pliku.
 */
///@{

static StackItem* StackItemCreate()
{
	StackItem *new_item = (StackItem*) malloc(sizeof (StackItem));
	assert(new_item);
	return new_item;
}

static StackItem StackItemFromPoly(const Poly *p, StackItem *next_item)
{
	return (StackItem)
	{
		.poly = *p,
		.next_item = next_item
	};
}

///@}

/** @name Funkcje główne
 * Funkcje zadeklarowane w stack.h przeznaczone do używania w innych plikach.
 */
///@{

void StackInit(Stack *stack)
{
	stack->top_item = NULL;
}

void StackDestroy(Stack *stack)
{
	while (!StackIsEmpty(stack))
	{
		Poly p = StackPop(stack);
		PolyDestroy(&p);
	}
}

void StackPush(Stack *stack, const Poly *p)
{
	StackItem *temp_pointer = stack->top_item;
	stack->top_item = StackItemCreate();
	*(stack->top_item) = StackItemFromPoly(p, temp_pointer);
}

Poly StackPop(Stack *stack)
{
	assert(!StackIsEmpty(stack));

	StackItem *temp_pointer = stack->top_item;
	stack->top_item = (stack->top_item)->next_item;
	Poly p = temp_pointer->poly;
	free(temp_pointer);
	return p;
}

Poly StackPeek(Stack *stack)
{
	assert(!StackIsEmpty(stack));
	return (stack->top_item)->poly;
}

bool StackIsEmpty(const Stack *stack)
{
	return stack->top_item == NULL;
}

///@}
