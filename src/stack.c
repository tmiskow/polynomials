/** @file
   TODO

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date TODO
*/

#include "poly.h"
#include "stack.h"

/** TODO */
static StackItem* StackItemCreate();

/** TODO
 * Przejmuje wielomian @p p na własność.
 */
static StackItem StackItemFromPoly(const Poly *p, StackItem *next_item);

/** TODO */
static void StackItemDestroy(StackItem* stack_item);

/** TODO */
static void StackDestroyItemsRecursively(StackItem* stack_item);

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

static void StackItemDestroy(StackItem* stack_item)
{
	PolyDestroy(&(stack_item->poly));
	free(stack_item);
}

static void StackDestroyItemsRecursively(StackItem* stack_item)
{
	if (stack_item)
	{
		StackDestroyItemsRecursively(stack_item->next_item);
		StackItemDestroy(stack_item);
	}
}

void StackInit(Stack *stack)
{
	stack->top_item = NULL;
}

void StackDestroy(Stack *stack)
{
	StackDestroyItemsRecursively(stack->top_item);
}

void StackPush(Stack *stack, const Poly *p)
{
	StackItem *temp_pointer = stack->top_item;
	stack->top_item = StackItemCreate();
	*(stack->top_item) = StackItemFromPoly(p, temp_pointer);
}

Poly StackPop(Stack *stack)
{
	//DEBUG
	assert(!StackIsEmpty(stack));

	StackItem *temp_pointer = stack->top_item;
	stack->top_item = (stack->top_item)->next_item;
	Poly p = temp_pointer->poly;
	free(temp_pointer);
	return p;
}

bool StackIsEmpty(const Stack *stack)
{
	return stack->top_item == NULL;
}
