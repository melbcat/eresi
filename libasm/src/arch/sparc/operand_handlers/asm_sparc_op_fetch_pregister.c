#include <libasm.h>
#include <libasm-sparc.h>

/**
 * Decode data for operand type ASM_SP_OTYPE_PREGISTER
 * @param operand Pointer to operand structure to fill.
 * @param opcode Pointer to opcode data.
 * @param otype Operand type.
 * @param ins Pointer to instruction structure.
 *
 * @return Operand length (currently unused in libasm-sparc)
 */

int asm_sparc_op_fetch_pregister(asm_operand *operand, u_char *opcode, 
                       				   int otype, asm_instr *ins)
{
  operand->type = ASM_SP_OTYPE_PREGISTER;

  return (0);
}
