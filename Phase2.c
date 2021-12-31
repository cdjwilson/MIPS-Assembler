#include "Phase2.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//map for each label to its pc
struct MapPC {
	char label[64];
	int PC;
	struct MapPC *next;
};
// frees all malloced MapPCs
void freeMapPC(struct MapPC *head) {
	if(head->next != NULL) {
		freeMapPC(head->next);
	}
	free(head);
}


// calculates the immediate for branch and jump instructions where first_pc is equal to the first instructinos address
void resolve_addresses(struct ArrayList *unresolved, uint32_t first_pc, struct ArrayList *resolved) {
	struct MapPC *head = NULL;
	struct MapPC *temp =NULL;
	struct Instruction new;
	for(int i = 0; i < unresolved->size; i++) { // first pass finding where the labels are located
		if(strcmp(unresolved->elements[i].label, "") != 0) { // if there is a label for the location of the instruction then the PC of that label is PC = i*4+first_pc
			if(head == NULL) { // if the head of MapPC is is NULL then set head to map to the first new label
				head = malloc(sizeof(struct MapPC));
				strcpy(head->label, unresolved->elements[i].label);
				head->PC = (first_pc+(i*4));
				head->next = NULL;
				temp = head;
			} else while(temp->next != NULL) { // if head isnt NULL then while heads next isnt NULL fing the MapPC that has a head.next that is NULL
				temp = temp->next;
			} // once node with next equal to NULL creat a new map for the new label
			temp->next = malloc(sizeof(struct MapPC));
			strcpy(temp->label, unresolved->elements[i].label);
			temp->PC = (first_pc+(i*4));
			temp = temp->next;
			temp->next = NULL;
		}
	} // end of first pass which ends with head equal to the start of the map of addresses
	int imm;
	int pcCur;
	for(int i = 0; i < unresolved->size; i++) { // second pass to calculate the immediates of branch and jump instructions
		temp = head;
		if(strcmp(unresolved->elements[i].branch_label, "") != 0) { // if it has a branch label then calculate the immediate and create an instruction for resolved
			while(strcmp(unresolved->elements[i].branch_label, temp->label) != 0) { // find the MapPC with the same label as the branch label and calculate imm
				temp = temp->next;
			}
			pcCur = (first_pc+(i*4));
			imm = (temp->PC-pcCur-4)/4;
			new = newInstruction(unresolved->elements[i].instruction_id, unresolved->elements[i].rd, unresolved->elements[i].rs, unresolved->elements[i].rt, imm, unresolved->elements[i].jump_address, unresolved->elements[i].shift_amount, unresolved->elements[i].label, unresolved->elements[i].branch_label);
			addLast(resolved, new);
		} else { // it doesn't have a branch label
			new = newInstruction(unresolved->elements[i].instruction_id, unresolved->elements[i].rd, unresolved->elements[i].rs, unresolved->elements[i].rt, unresolved->elements[i].immediate, unresolved->elements[i].jump_address, unresolved->elements[i].shift_amount, unresolved->elements[i].label, unresolved->elements[i].branch_label);
			addLast(resolved, new);
		}
	} // end of second pass should have added all needed things intno resolved with calculated imm
		// free all mallocs
	if(head != NULL) {
		freeMapPC(head);
	}
}
