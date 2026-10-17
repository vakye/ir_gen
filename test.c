
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

    {
        irg_inst_id A = irg_BitwiseAnd(irg_Integer(0x10101), irg_Integer(0x0101));
        irg_inst_id B = irg_BitwiseAnd(irg_Integer(1239892), irg_Integer(0xFFF));
        irg_inst_id C = irg_BitwiseAnd(irg_Integer(2348712), irg_Integer(0x0));

        PerformTestCase(A, 0x0101);
        PerformTestCase(B, 2900);
        PerformTestCase(C, 0);
    }

    {
        irg_inst_id A = irg_BitwiseOr(irg_Integer(0x10), irg_Integer(0x100));
        irg_inst_id B = irg_BitwiseOr(irg_Integer(0x2314), irg_Integer(0x47291));
        irg_inst_id C = irg_BitwiseOr(irg_Integer(2387), irg_Integer(0));

        PerformTestCase(A, 0x110);
        PerformTestCase(B, 0x47395);
        PerformTestCase(C, 2387);
    }

    {
        irg_inst_id A = irg_BitwiseXor(irg_Integer(0x142), irg_Integer(0x142));
        irg_inst_id B = irg_BitwiseXor(irg_Integer(0x1234), irg_Integer(0xdead));
        irg_inst_id C = irg_BitwiseXor(irg_Integer(1234), irg_Integer(0));

        PerformTestCase(A, 0x0000);
        PerformTestCase(B, 0xcc99);
        PerformTestCase(C, 1234);
    }

    {
        irg_inst_id A = irg_BitwiseNot(irg_Integer(0xffff0000ffff0000));
        irg_inst_id B = irg_BitwiseNot(irg_Integer(0x1010101010101010));
        irg_inst_id C = irg_BitwiseNot(irg_Integer(0x0000000000000000));

        PerformTestCase(A, 0x0000ffff0000ffff);
        PerformTestCase(B, 0xefefefefefefefef);
        PerformTestCase(C, 0xffffffffffffffff);
    }

    {
        irg_inst_id Block = irg_BeginBlock();
        {
            irg_inst_id A = irg_Declare();
            irg_inst_id B = irg_Declare();

            irg_inst_id C = irg_Store(irg_AddressOf(A), irg_Integer(10));
            irg_inst_id D = irg_Store(irg_AddressOf(B), irg_Integer(20));

            irg_inst_id E = irg_Load(irg_AddressOf(A));
            irg_inst_id F = irg_Load(irg_AddressOf(B));

            irg_inst_id G = irg_Add(E, F);

            irg_PushBlock(A); irg_PushBlock(B);
            irg_PushBlock(C); irg_PushBlock(D);
            irg_PushBlock(E); irg_PushBlock(F);
            irg_PushBlock(G);
        }
        irg_EndBlock();

        PerformTestCase(Block, 30);
    }
}

#include "linux_base.c"

