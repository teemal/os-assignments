#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "linked_list.h"

void add_to_back(list_t* list, char* read_line);

void add_to_back(list_t* list, char* read_line){
	node_t* new_node = (node_t*)malloc(sizeof(node_t));
	new_node->line = strdup(read_line);
	new_node->next = NULL;
	if(list->head == NULL){
		list->head = new_node;
		list->tail = new_node;
	}
	else{

		list->tail->next = new_node;
		list->tail = new_node;
	}
}



int main(int argc, char const *argv[])
{
	FILE *file = NULL;
	char read_[LINE_MAX] = {'\0'};
	node_t* cur_node = NULL;
	char* read_line = NULL;
	int i;

   //=======================first loop====================
   	list_t* list = malloc(sizeof(list_t));
	list->head = NULL;
	for(i = 1; i < argc; i++){
		file = fopen(argv[i], "r");
		read_line = fgets(read_, LINE_MAX, file);
   		while(read_line != NULL){
   			add_to_back(list, read_line);
   			read_line = fgets(read_, LINE_MAX, file);
   		}

   	}
   	fclose(file);
   //=======================second loop===================

   	cur_node = list->head;
   	i = 1;
   	while(cur_node != NULL){
   		printf("%04d: %s", i, cur_node->line);
   		cur_node = cur_node->next;
   		i++;
   	}
   	printf("\n");
   //=======================third loop====================
   	while(list->head != NULL){
   		cur_node = list->head;
   		list->head = list->head->next;
   		free(cur_node->line);
   		free(cur_node);
   	}
   	free(list);
	return 0;
}
