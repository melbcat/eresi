#include "libasm.h"

int
asm_sparc_fbpfcc(asm_instr * ins, u_char * buf, u_int len,
		 asm_processor * proc)
{
  struct s_decode_pbranch opcodep;
  struct s_asm_proc_sparc *inter;
  sparc_convert_pbranch(&opcodep, buf, proc);

  inter = proc->internals;

  ins->instr = inter->fbcc_table[opcodep.cond];
  ins->type = ASM_TYPE_CONDBRANCH;
  ins->nb_op = 2;
  ins->op1.type = ASM_SP_OTYPE_DISPLACEMENT;
  ins->op1.imm = opcodep.imm;
  ins->op2.type = ASM_SP_OTYPE_CC;
  ins->op2.base_reg = opcodep.cc;
  ins->annul = opcodep.a;
  ins->prediction = opcodep.p;
  
  return 4;
}
