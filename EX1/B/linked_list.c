#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "linked_list.h"

list_t list_create()
{
	list_t list = malloc( sizeof(struct list) );

	list->head = NULL;
	list->tail = NULL;
	list->length = 0;
	
	return list; 
}

void list_delete(list_t list)
{
	struct node* pNode = NULL;
	struct node* pNext = NULL;
	int i;

	assert( list );
	
	//free the nodes
	for ( i=0, pNode=list->head; i<list->length; i++, pNode=pNext ) {
		assert( pNode );
		pNext = pNode->next;
		free( pNode );
	}
	
	//free the head
	free( list );
}

void list_insert(list_t list, int index, int data)
{
	struct node* pNewNode = NULL;
	struct node* pNode = NULL;
	int i;
	
	assert( list );
	
	//create the new node
	pNewNode = malloc( sizeof(struct node) );
	pNewNode->data = data;
	
	//insert the new node
	if ( index == 0 ) {						//case of adding on the front
		if ( list->head )
			list->head->prev = pNewNode;
		else
			list->tail = pNewNode;
		pNewNode->prev = NULL;
		pNewNode->next = list->head;
		list->head = pNewNode;
	} else if ( index == list->length ) {	//case of adding on the back
		list->tail->next = pNewNode;
		pNewNode->prev = list->tail;
		pNewNode->next = NULL;
		list->tail = pNewNode;
	} else { 								//case of adding inside
		//move to the insertion location
		for ( i=0, pNode=list->head; i<index; i++, pNode=pNode->next );
		//insert
		pNewNode->prev = pNode->prev;
		pNewNode->next = pNode;
		pNode->prev->next = pNewNode;
		pNode->prev = pNewNode;
	}
	
	list->length++;
}

void list_append(list_t list, int data)
{
	list_insert( list, list->length, data );
}

void list_print(list_t list)
{
	struct node* pNode = NULL;
	int i;

	assert( list );
	
	for ( i=0, pNode=list->head; i<list->length; i++, pNode=pNode->next ) {
		assert( pNode );
		printf( "%d ", pNode->data );
	}
	printf( "\n" );
}

long list_sum(list_t list)
{
	struct node* pNode = NULL;
	int i;
	long sum = 0;

	assert( list );
	
	for ( i=0, pNode=list->head; i<list->length; i++, pNode=pNode->next ) {
		assert( pNode );
		sum += pNode->data;
	}
	
	return sum;
}

int list_get(list_t list, int index)
{
	struct node* pNode = NULL;
	int i;
	
	assert( list );
	
	//move to the index
	for ( i=0, pNode=list->head; i<index; i++, pNode=pNode->next );
	
	return pNode->data;
}

int list_extract(list_t list, int index)
{
	struct node* pNode = NULL;
	int i;
	
	assert( list );
	
	//move to the index
	for ( i=0, pNode=list->head; i<index; i++, pNode=pNode->next );
	
	//return value
	i = pNode->data;
	
	//remove the node
	if ( index == 0 ) {						//case of extracting on the front
		list->head = pNode->next;
		if ( pNode->next )
			pNode->next->prev = NULL;
	} else if ( index == list->length ) {	//case of extracting on the back
		list->tail = pNode->prev;
		pNode->prev->next = NULL;
	} else { 								//case of extracting inside
		pNode->next->prev = pNode->prev;
		pNode->prev->next = pNode->next;
	}
	free( pNode );
	list->length--;
	
	return i;
}
