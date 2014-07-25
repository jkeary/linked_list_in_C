/****************************************************************
 * llist.c
 * MPATE-GE 2618
 * Problem Set 5
 *
 * Implementation of a doubly-linked list.
 * 
 * The list is accessed by both a head and a tail pointer. 
 * Dynamically allocated string (name) and  an associated int (ID number) 
 * is stored in the elements as well. The list is not allowed to 
 * contain two identical strings.  The nodes are stored in ascending 
 * alphabetical order from head to tail. 
 *
 * Functions can insert a new string/id pair, delete a node when 
 * identified by a matching string, and search for a string and return 
 * the associated id number.  The following error conditions are detected:  
 * inserting a duplicate string, deleting a non-existent string, failing to
 * find a specified search string.
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "llist.h"
#include "inputlib.h"


// Initialize list
void init_list(llist *list)
{
  if (list == NULL)
    return;

  list->length = 0;		
  list->head = NULL;	
  list->tail = NULL;	
}

// Delete the entire list.  This includes deleting all memory that's
// been allocated for nodes and strings.
void delete_list(llist *list) {		
	
// declare & initialize a node ptr to the first position in the llist and declare a temporary node ptr.
	node* nptr = list->head, *tmp;
	
// Stops iterating through llist when nptr is NULL 
	while ( nptr != NULL ) { 
		tmp = nptr->next;	 
		free( nptr );		 
		nptr = tmp;			 
	}
}

// Function to dynamically allocate space for a new node and node ptr.
node* create_node (const char *name, int id) {

// allocate space for ptr, allocate space for pointee, and dereference
	node* nptr = (node*)malloc(sizeof(node));
	if ( nptr == NULL ) {
		printf("No more memory!\n");
		exit(1);
	}
	
// allocate memory for node's name,and assign name to new node 
	nptr->name = (char*)malloc(sizeof(char)*strlen(name)+1);

// check for successful memory allocation
	strcpy(nptr->name, name);
	
// get id # and assign to new node
	nptr->id = id;
	
// Initialize yer pointers to NULL b/c they are unused at this point
	nptr->next = NULL;
	nptr->prev = NULL;
	
// exit
	return nptr;
}

// Inserts an element alphabetically into a list. Checks to see if an 
// element with the same name exists already. 
// Returns false if duplicate string found.
// Recommendation: Use the function strcasecmp() from the string.h library; 
// it is _not_ case sensitive.  strcasecmp(s1, s2) returns a value less 
// than 0 if s1 comes alphabetically before s2, greater than 0 if s1 comes 
// after s2, and equal to zero if s1 and s2 are identical. 
bool insert(llist *list, const char *string, int number) 
{	
	node *new = create_node( string, number );

// If first element...
	if (list->head == NULL) { 
		list->head = new;
		list->tail = new;
		new->next = NULL;
		new->prev = NULL;
		list->length++;
		return true;
	}

// if there are 1 or more nodes...
	while (list->head != NULL) {
// start at the beginning of the list
		node *curr = list->head;
		node *before = curr->prev; // NULL
		if (strcasecmp(new->name, curr->name) == 0) {
			return false; // do not insert a new node
		}
// comes after
		else {
			while (strcasecmp(new->name, curr->name) > 0) {
				before = curr; 
				curr = curr->next;
			} 
		}
// comes before at head of list
		if (before == NULL) { 
			new->next = curr;
			new->prev = NULL;
			curr->prev = new;
			list->head = new;
			list->length++;
			return true;
		} 
// at end of list
		else if (curr == NULL) {  
			new->prev = before;
			new->next = NULL;
			list->tail = new;
			before->next = new;
			list->length++;
			return true;
		}
// inserting in middle
		else {  
			new->prev = before;
			new->next = curr;
			before->next = new;
			curr->prev = new;
			list->length++;
			return true;
		}
	}
	return false;
}

// Delete node function
void deleteNode(node* node) 
{
	if (node == NULL) {
		return;
	}
	if (node->name != NULL) {
		free(node->name);
	}
	free(node);
}

// Removes a NODE! based on the user's input string.  Returns true if element found and delete, false otherwise.
bool delete_given_string(llist *list, const char *string)
{
// declare node pts to iterate through list with
	node* curr = list->head;
	node* before = curr->prev;
	
// declare temporary node ptr to point to delete node
	node* tmp = NULL;
	
// when not to remove a node...
	if (curr == NULL || string == NULL || list->length == 0) {
		return false;
	}
	
// while the current node is not equal to the user input, then traverse the list.  If it finds either case where current node is equal to NULL or where the current node name is equal to the user input, it stops.
	while (curr != NULL && strcasecmp(string, curr->name) != 0) {
		before = curr;
		curr = curr->next;
	}
// if stops at head of list...
	if (curr == list->head) {
		deleteNode(curr);
		list->length--;
		list->head = NULL;
		list->tail = NULL;
		return true;
	}
	
//	if (before != NULL && curr != NULL) {
	if (curr == list->tail) { // delete from end
		tmp = curr;
		before->next = NULL;
		list->tail = before;
		deleteNode(tmp);
		list->length--;
		return true;
	}	
// delete from middle
	else { 
		tmp = curr;
		before->next = curr->next;
		curr->next = curr->prev;
		deleteNode(tmp);
		list->length--;
		return true;
	}
	return false;	
}

// Searches a list for an element which matches the given string.  If found, the element's ID number is stored in id_number, which is passed by reference.
bool find(llist *list, const char *string, int *id_number)
{
	node* curr = list->head;
	
	if (list->head == NULL) {
		printf("theres nothing in the list to search for\n");
		return false;
	}
	
	while (curr != NULL && strcasecmp(string, curr->name) != 0) {
		curr = curr->next;
	}
	if (curr == NULL) {
		printf("didn't find that string in the list\n");
		return false;
	} 
// if string is found in list, and curr != NULL 
	else { 
		*id_number = curr->id; 
		return true;
	}
}	

// Function prints name/ids given a ptr to a linked list.
void print(llist *list)			
{
// declare a ptr to a node called curr that points to the current node to be printed
	node *curr = list->head;			
	node *before = NULL;
	printf("\n-Node List-\n"); 
// traverses the linked list, printing name and id#
	while (curr != NULL) {
		printf("Name: %s, ID #: %d\n", curr->name, curr->id);
		before = curr;
		curr = curr->next;
	}
}

// Prints all name/id pairs in the list from last to first nodes.
void print_backwards(llist *list)
{
// declare a ptr to a node called curr that points to the current node to be printed
	node *curr;		
	
	printf("\n-Node List in Reverse!-\n"); 
// traverse the linked list backwards, printing name and id#
	for ( curr = list->tail ; curr != NULL ; curr = curr->prev ) {
		printf("Name: %s, ID #: %d\n", curr->name, curr->id);
	}
}

