/** @file
   TODO

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date TODO
*/

#ifndef __STACK_H__
#define __STACK_H__

#include "poly.h"

#include <stdbool.h>

/** TODO */
typedef struct Stack Stack;

/* TODO */
typedef struct StackItem StackItem;

/**
 * TODO
 */
struct Stack
{
	StackItem* top_item;
};

struct StackItem
{
	Poly poly;
	StackItem* next_item;
};

/** TODO */
void StackInit(Stack *stack);

/** TODO */
void StackDestroy(Stack *stack);

/** TODO
 * Stos przejmuje na własnośc wielomian @p p.
 */
void StackPush(Stack *stack, const Poly *p);

/** TODO */
Poly StackPop(Stack *stack);

/** TODO */
Poly StackPeek(Stack *stack);

/** TODO */
bool StackIsEmpty(const Stack *stack);

#endif /* __STACK_H__ */
