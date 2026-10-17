
#define IR_GEN_IMPLEMENTATION
#include "ir_gen.h"

static irg_ssize CallMachineCode(irg_code Code);

void PerformTestCase(irg_node_id NodeID, irg_ssize Expected)
{
    irg_code Code = irg_GenerateX64(NodeID);
    irg_ssize Result = CallMachineCode(Code);

    IRG_ASSERT(Result == Expected);
}

void Test(void)
{
    irg_node_id A = irg_Integer(10);
    irg_node_id B = irg_Add(irg_Integer(10),    irg_Integer(20));
    irg_node_id C = irg_Sub(irg_Integer(5),     irg_Integer(13));
    irg_node_id D = irg_Mul(irg_Integer(20),    irg_Integer(7));
    irg_node_id E = irg_Div(irg_Integer(500),   irg_Integer(23));
    irg_node_id F = irg_Mod(irg_Integer(348),   irg_Integer(23));
    irg_node_id G = irg_Negate(irg_Integer(23));

    PerformTestCase(A, 10);
    PerformTestCase(B, 30);
    PerformTestCase(C, -8);
    PerformTestCase(D, 140);
    PerformTestCase(E, 21);
    PerformTestCase(F, 3);
    PerformTestCase(G, -23);
}

#include "linux_base.c"

