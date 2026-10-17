
#define IR_GEN_IMPLEMENTATION
#include "ir_gen.h"

static irg_ssize CallMachineCode(irg_code Code);

void PerformTestCase(irg_inst_id InstID, irg_ssize Expected)
{
    irg_code Code = irg_GenerateX64(InstID);
    irg_ssize Result = CallMachineCode(Code);

    IRG_ASSERT(Result == Expected);
}

void Test(void)
{
    {
        irg_inst_id A = irg_Integer(10);
        irg_inst_id B = irg_Add(irg_Integer(10),    irg_Integer(20));
        irg_inst_id C = irg_Sub(irg_Integer(5),     irg_Integer(13));
        irg_inst_id D = irg_Mul(irg_Integer(20),    irg_Integer(7));
        irg_inst_id E = irg_Div(irg_Integer(500),   irg_Integer(23));
        irg_inst_id F = irg_Mod(irg_Integer(348),   irg_Integer(23));
        irg_inst_id G = irg_Negate(irg_Integer(23));

        PerformTestCase(A, 10);
        PerformTestCase(B, 30);
        PerformTestCase(C, -8);
        PerformTestCase(D, 140);
        PerformTestCase(E, 21);
        PerformTestCase(F, 3);
        PerformTestCase(G, -23);
    }

    {
        irg_inst_id A = irg_Equal   (irg_Integer(10), irg_Integer(10));
        irg_inst_id B = irg_NotEqual(irg_Integer(7), irg_Integer(3));
        irg_inst_id C = irg_Equal   (irg_Integer(23), irg_Integer(7));
        irg_inst_id D = irg_NotEqual(irg_Integer(573), irg_Integer(573));

        PerformTestCase(A, 1);
        PerformTestCase(B, 1);
        PerformTestCase(C, 0);
        PerformTestCase(D, 0);
    }

    {
        irg_inst_id A = irg_Less        (irg_Integer(10), irg_Integer(23));
        irg_inst_id B = irg_Less        (irg_Integer(23), irg_Integer(23));
        irg_inst_id C = irg_LessEqual   (irg_Integer(10), irg_Integer(23));
        irg_inst_id D = irg_LessEqual   (irg_Integer(23), irg_Integer(23));
        irg_inst_id E = irg_Less        (irg_Integer(23), irg_Integer(10));
        irg_inst_id G = irg_LessEqual   (irg_Integer(23), irg_Integer(10));

        PerformTestCase(A, 1);
        PerformTestCase(B, 0);
        PerformTestCase(C, 1);
        PerformTestCase(D, 1);
        PerformTestCase(E, 0);
        PerformTestCase(G, 0);
    }

    {
        irg_inst_id A = irg_Greater         (irg_Integer(10), irg_Integer(23));
        irg_inst_id B = irg_Greater         (irg_Integer(23), irg_Integer(23));
        irg_inst_id C = irg_GreaterEqual    (irg_Integer(10), irg_Integer(23));
        irg_inst_id D = irg_GreaterEqual    (irg_Integer(23), irg_Integer(23));
        irg_inst_id E = irg_Greater         (irg_Integer(23), irg_Integer(10));
        irg_inst_id G = irg_GreaterEqual    (irg_Integer(23), irg_Integer(10));

        PerformTestCase(A, 0);
        PerformTestCase(B, 0);
        PerformTestCase(C, 0);
        PerformTestCase(D, 1);
        PerformTestCase(E, 1);
        PerformTestCase(G, 1);
    }

    {
        irg_inst_id A = irg_ShiftLeft   (irg_Integer(23), irg_Integer(10));
        irg_inst_id B = irg_ShiftRight  (irg_Integer(124871248), irg_Integer(13));

        PerformTestCase(A, 23552);
        PerformTestCase(B, 15243);
    }
}

#include "linux_base.c"

