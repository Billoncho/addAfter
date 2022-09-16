/*
Author: Billy Gene Ridgeway
Date: Sept. 16th. 2022
Purpose: Learn how to add an element into a list in C.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This what is going to be in the queue.
typedef struct listitem {
	struct listitem *next;			// Pointer to the next item.
	struct listitem *prev;			// Pointer to the previous item.
	int data;						// Some integer data.
} LISTITEM;

// This is the queue 'header'.
typedef struct {
	struct listitem *first;			// Pointer to the next item.
	struct listitem *last;			// Pointer to the previous item.
} LISTHDR;

LISTHDR head;						// The queue.

									// This puts an item in at the end of a queue.
void enqueue(LISTHDR *queue, LISTITEM *item) {
	LISTITEM *temp;

	temp = queue->last;				// Get the 'last' item in the queue and keep hold of it.
	queue->last = item;				// Put the item in the queue at the end.
	item->prev = temp;				// Link back to old 'last' item.
	item->next = (LISTITEM*)queue;	// Set the forward link of the new item.
	temp->next = item;				// Finally, set the forward link of the old 'last' item to the new one.
}

// This removes an item from the front of a queue - returns the item or NULL if there are no more items.
LISTITEM *dequeue(LISTHDR *queue) {
	LISTITEM *temp;

	temp = queue->first;				// Get the 'first' item.
	if (temp == (LISTITEM*)queue) {		// If the head of the queue points to itself,
		temp = NULL;					// then the queue is empty.
	} else {
		queue->first = temp->next;		// Set the queue header to point to the 'second' item.
		queue->first->prev = (LISTITEM*)queue;
	}
	return temp;
}

// Returns the number of items in a queue.
int queue_length(LISTHDR *queue) {
	LISTITEM *temp;
	int length;

	temp = queue->first;			// Get the 'first' item.
	length = 0;						// Initialize the length.
	do {
		// Check for an empty queue or if we've gone through the whole queue.
		if (temp == (LISTITEM*)queue) {
			temp = NULL;			// This will break out of the do ... while loop.
			break;
		}
		temp = temp->next;			// Get the next item in the queue.
		length = length + 1;
	} while (temp != NULL);

	return length;
}

// This adds an item after 'position' in a queue - returns the item or NULL if the position does not exist.
// NOTE: This uses a similar queue iteration technique to that used in 'queue_length'.
LISTITEM *add_after(LISTHDR *queue, int position, LISTITEM *item) {
	LISTITEM *temp;
	int i;

	// NOTE: We don't deal with negative positions in this example, but we could use a negative position to
	// indicate that we want to use the backward pointers as the position.
	if (position < 0) {
		return NULL;
	}

	temp = queue->first;			// Get the 'first' item.
	i = 0;
	do {
		// Check for an empty queue or if we've gone through the whole queue.
		if (temp == (LISTITEM*)queue) {
			temp = NULL;			// This will break out of the do ... while loop.
			break;
		}
		if (i == position) {
			// This is where we link the new item into the queue.
			item->next = temp->next;
			temp->next = item;
			// This inserts the new item *after* the position in the queue using the 'prev' pointers.
			item->prev = temp;
			item->next->prev = item;
			break;
		}
		temp = temp->next;			// Get the next item in the queue,
		i = i + 1;					// and increment the corresponding index position.
	} while (temp != NULL);

	return temp;
}



int main() {
	LISTITEM *temp;
	int requested_index;
	char input[50];

	// First, make an empty queue.
	// Which is a queue where the header points to itself and there are no items in it.
	printf("\nAddress of head = %p\n", &head);
	head.first = (LISTITEM*)&head;
	head.last = (LISTITEM*)&head;

	for (int i = 0; i < 5; i++) {			// Populate the queue.
		temp = malloc(sizeof(LISTITEM));	// Allocate some memory for the new queue item.
		// Uncomment this (for debugging).
		// printf("address of new item = %p\n", temp);
		temp->data = i;						// Set the item's data to the loop count so that we can see where it is in the queue,
		enqueue(&head, temp);				// and put it in the queue.
	}

	printf("The length of the queue is %d.\n", queue_length(&head));
	// Add item at a user entered index.
	printf("\nEnter the index of the queue entry ... ");
	requested_index = atoi(gets(input));
	temp = malloc(sizeof(LISTITEM));
	// Uncomment this (for debugging).
	// printf("address of new item = %p\n", temp);
	temp->data = -requested_index;			// Set payload to a -ve number so we can see where it is when we print out the queue.
	if (add_after(&head, requested_index, temp) == NULL) {	// If we can't do it we will get NULL returned.
		printf("Cannot add item at %d\n", requested_index);
		free(temp);				// Call 'free' to clean up.
	} else {
		printf("Added item at %d.\n", requested_index);
	}

	// Uncomment this if you want to check the addresses (for debugging).
	/*
	// Print out the queue fully forwards.
	temp = head.first;
	printf("forwards ...\n");
	do {
		printf("addr=%p; data=%2d; next=%p; prev=%p; addr=%p\n", temp, temp->data, temp->next, temp->prev, temp);
		temp = temp->next;
	} while (temp != head.first);

	// Print out the queue fully backwards.
	printf("backwards ...\n");
	temp = head.last;
	do {
		printf("addr=%p; data=%2d; next=%p; prev=%p; addr=%p\n", temp, temp->data, temp->next, temp->prev, temp);
		temp = temp->prev;
	} while (temp != head.last);
	*/

	// See what we've got.
	printf("\nThe length of the queue is now %d.\n", queue_length(&head));
	do {							// Keep going until the queue is empty.
		temp = dequeue(&head);		// If the queue is empty we will get NULL returned.
		if (temp != NULL) {
			printf("data: %d\n", temp->data);
			free(temp);				// Call 'free' to clean up.
		}
	} while (temp != NULL);

	return 0;
}
