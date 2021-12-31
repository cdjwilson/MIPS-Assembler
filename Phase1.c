#include "Phase1.h"
#include "InstructionFactory.h"
#include <stdio.h>
// turning mal instructions into tal instructions
void mal_to_tal(struct ArrayList *mals, struct ArrayList *tals) {
	struct Instruction new;
	int temp = 65535;
	for(int i = 0; i < mals->size; i++) { 					// for each element in mals check if it needs to be translated to a tal instruction then add it to the talls ArrayList
		if(mals->elements[i].instruction_id == blt) { // if instruction is a branch less than then turn it into tal instructions of slt and bne
			new = CreateSlt_lab(1, mals->elements[i].rt, mals->elements[i].rs, mals->elements[i].label);
			addLast(tals, new);
			new = CreateBne(1, 0, mals->elements[i].branch_label);
			addLast(tals, new);
		} else if(mals->elements[i].instruction_id == bge) { // if instruction is a branch greater than instruction turn it into tal instructions of slt and beq
			new = CreateSlt_lab(1, mals->elements[i].rt, mals->elements[i].rs, mals->elements[i].label);
			addLast(tals, new);
			new = CreateBeq(1, 0, mals->elements[i].branch_label);
			addLast(tals, new);
		} else if(mals->elements[i].instruction_id == mov) { // if mov instructino then just change it to an addu instruction
			new = CreateAddu_lab(mals->elements[i].rd, 0,mals->elements[i].rt, mals->elements[i].label);
			addLast(tals, new);
		} else if(mals->elements[i].instruction_id == ori) { // if ori instruction then check if its immediate uses more than 16 bits
			if(mals->elements[i].immediate > 65535 || mals->elements[i].immediate < 0) {
				int x = (mals->elements[i].immediate >> 16);
				x = temp & x;
				new = CreateLui_lab(1, x, mals->elements[i].label);
				addLast(tals, new);
				x = temp & mals->elements[i].immediate;
				new = CreateOri(1, 1, x);
				addLast(tals, new);
				new = CreateOr(mals->elements[i].rt, mals->elements[i].rs, 1);
				addLast(tals, new);
			} else { // if it doesn't take more than 16 bits just copy the instruction over
				new = newInstruction(mals->elements[i].instruction_id, mals->elements[i].rd, mals->elements[i].rs, mals->elements[i].rt, mals->elements[i].immediate, mals->elements[i].jump_address, mals->elements[i].shift_amount, mals->elements[i].label, mals->elements[i].branch_label);
			addLast(tals, new);
			}
		} else if(mals->elements[i].immediate > 65535 || mals->elements[i].immediate < -32767) { // if immediate is too large then use lui and ori with addu to get what you need
			int x = (mals->elements[i].immediate >> 16);
			x = temp & x;
			new = CreateLui_lab(1, x, mals->elements[i].label);
			addLast(tals, new);
			x = temp & mals->elements[i].immediate;
			new = CreateOri(1, 1, x);
			addLast(tals, new);
			new = CreateAddu(mals->elements[i].rt, mals->elements[i].rs, 1);
			addLast(tals, new);
		} else { // just a normal tal instruction with nothing to be changed just copy it to tals
			new = newInstruction(mals->elements[i].instruction_id, mals->elements[i].rd, mals->elements[i].rs, mals->elements[i].rt, mals->elements[i].immediate, mals->elements[i].jump_address, mals->elements[i].shift_amount, mals->elements[i].label, mals->elements[i].branch_label);
			addLast(tals, new);
		}
	}
}
