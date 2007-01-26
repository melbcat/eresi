/*
** $Id: op_incdec_rmb.c,v 1.1 2007-01-26 14:18:37 heroine Exp $
**
*/
#include <libasm.h>
#include <libasm-int.h>

/*
  <instruction func="op_incdec_rmb" opcode="0xfe"/>
*/

int op_incdec_rmb(asm_instr *new, u_char *opcode, u_int len, asm_processor *proc) {
  struct s_modrm        *modrm;
  
  new->ptr_instr = opcode;
  modrm = (struct s_modrm *) opcode + 1;
  new->len += 1;
  switch(modrm->r) {
  case 0:
    new->instr = ASM_INC;
    break;
  case 1:
    new->instr = ASM_DEC;
    break;
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
    break;
  default:
    break;
  }
  new->op1.type = ASM_OTYPE_ENCODED;
  operand_rmb(&new->op1, opcode + 1, len - 1, proc);
  new->len += new->op1.len;
  return (new->len);
}
