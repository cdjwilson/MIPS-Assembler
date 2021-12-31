#include <stdio.h>
#include "Phase3.h"
// translates the tal instructions into their 32 bit binary number
void translate_instructions(struct ArrayList *tals, uint32_t machineCode[]) {
	int oc;
	int func;
	int rd;
	int rs;
	int rt;
	int imm;
	int sa;
	int32_t mcode;
	int rtype;
	int itype;
	for(int i = 0; i < tals->size; i++) { // for each instruction calculate its machineCode
		int x = 65535;
		rs = tals->elements[i].rs;
		rs = rs << 21;
		rt = tals->elements[i].rt;
		rt = rt << 16;
		rd = tals->elements[i].rd;
		rd = rd << 11;
		sa = tals->elements[i].shift_amount;
		sa = sa << 6;
		imm = tals->elements[i].immediate;
		x = imm & x;
		rtype = rs | rt | rd | sa;
		itype = rs | rt | x;
		if(tals->elements[i].instruction_id == sll) { // if its an sll instruction
			func = 0;
			mcode = rtype | func;
			machineCode[i] = mcode;
		} else if(tals->elements[i].instruction_id == addu) { // if its an addu instruction
			func = 33;
			mcode = rtype | func;
			machineCode[i] = mcode;
		} else if(tals->elements[i].instruction_id == or) { // if its an or instruction
			func = 37;
			mcode = rtype | func;
			machineCode[i] = mcode;
		} else if(tals->elements[i].instruction_id == slt) { // if its an slt instruction
			func = 42;
			mcode = rtype | func;
			machineCode[i] = mcode;
		} else if(tals->elements[i].instruction_id == beq) { // if its an beq instruction
			oc = 4;
			oc = oc << 26;
			mcode = oc | itype;
			machineCode[i] = mcode;
		} else if(tals->elements[i].instruction_id == bne) { // if its an bne instruction
			oc = 5;
			oc = oc << 26;
			mcode = oc | itype;
			machineCode[i] = mcode;
		} else if(tals->elements[i].instruction_id == addi) { // if its an addi instruction
			oc = 8;
			oc = oc << 26;
			mcode = oc | itype;
			machineCode[i] = mcode;
		} else if(tals->elements[i].instruction_id == addiu) { // if its an addiu instruction
			oc = 9;
			oc = oc << 26;
			mcode = oc | itype;
			machineCode[i] = mcode;
		} else if(tals->elements[i].instruction_id == ori) { // if its an ori instruction
			oc = 13;
			oc = oc << 26;
			mcode = oc | itype;
			machineCode[i] = mcode;
		} else if(tals->elements[i].instruction_id == lui) { // if its an lui instruction
			oc = 15;
			oc = oc << 26;
			mcode = oc | itype;
			machineCode[i] = mcode;
		}
	} 
}
