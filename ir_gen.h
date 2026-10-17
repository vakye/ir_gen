
#ifndef __IR_GEN_H__
#define __IR_GEN_H__ 1

///////////////////////////////////////////////////////////
// NOTE(vak): Keywords
///////////////////////////////////////////////////////////

#if !defined(IRG_FUNC)
    #define IRG_FUNC static
#endif

#if !defined(IRG_INVALID_CODE_PATH)
    #define IRG_INVALID_CODE_PATH __builtin_trap()
#endif

///////////////////////////////////////////////////////////
// NOTE(vak): Types
///////////////////////////////////////////////////////////

// NOTE(vak): Integers

typedef signed char         irg_s8;
typedef signed short        irg_s16;
typedef signed int          irg_s32;
typedef signed long long    irg_s64;

typedef unsigned char       irg_u8;
typedef unsigned short      irg_u16;
typedef unsigned int        irg_u32;
typedef unsigned long long  irg_u64;

typedef irg_s64 irg_ssize;
typedef irg_u64 irg_usize;

// NOTE(vak): Booleans

typedef irg_u8  irg_b8; 
typedef irg_u32 irg_b32;

///////////////////////////////////////////////////////////
// NOTE(vak): Constants
///////////////////////////////////////////////////////////

#define IRG_TRUE    (1)
#define IRG_FALSE   (0)

///////////////////////////////////////////////////////////
// NOTE(vak): Macros
///////////////////////////////////////////////////////////

#define IRG_ARRAY_COUNT(Array) (sizeof(Array) / sizeof((Array)[0]))

#define IRG_ASSERT(Expression) if (!(Expression)) IRG_INVALID_CODE_PATH

///////////////////////////////////////////////////////////
// NOTE(vak): IR
///////////////////////////////////////////////////////////

typedef irg_u32 irg_inst_id;

#define IRG_NIL_INST_ID ((irg_inst_id)(0))

IRG_FUNC irg_inst_id irg_Integer(irg_usize Value);

IRG_FUNC irg_inst_id irg_Negate(irg_inst_id Operand);

IRG_FUNC irg_inst_id irg_Add(irg_inst_id Left, irg_inst_id Right);
IRG_FUNC irg_inst_id irg_Sub(irg_inst_id Left, irg_inst_id Right);
IRG_FUNC irg_inst_id irg_Mul(irg_inst_id Left, irg_inst_id Right);
IRG_FUNC irg_inst_id irg_Div(irg_inst_id Left, irg_inst_id Right);
IRG_FUNC irg_inst_id irg_Mod(irg_inst_id Left, irg_inst_id Right);

///////////////////////////////////////////////////////////
// NOTE(vak): Generation
///////////////////////////////////////////////////////////

typedef struct
{
    void* Base;
    irg_usize Size;
} irg_code;

IRG_FUNC irg_code irg_GenerateX64(irg_inst_id InstID);

///////////////////////////////////////////////////////////
// NOTE(vak): Implementation
///////////////////////////////////////////////////////////

#if defined(IR_GEN_IMPLEMENTATION)

///////////////////////////////////////////////////////////
// NOTE(vak): IR
///////////////////////////////////////////////////////////

typedef enum
{
    IRG_INST_KIND_NIL = 0,

    IRG_INST_KIND_INTEGER,
    IRG_INST_KIND_NEGATE,
    IRG_INST_KIND_ADD,
    IRG_INST_KIND_SUB,
    IRG_INST_KIND_MUL,
    IRG_INST_KIND_DIV,
    IRG_INST_KIND_MOD,

    IRG_INST_KIND_COUNT,
} irg_inst_kind;

typedef struct
{
    irg_inst_kind Kind;

    // NOTE(vak): Integer
        irg_usize IntegerValue;

    // NOTE(vak): Unary
        irg_inst_id Operand;

    // NOTE(vak): Binary
        irg_inst_id Left;
        irg_inst_id Right;
} irg_inst;

// TODO(vak): Custom allocator
static irg_inst irg_Insts[4096] = {0};
static irg_usize irg_InstCount = 1;

IRG_FUNC irg_inst* irg_GetInst(irg_inst_id InstID)
{
    irg_b32 IsInRange = (InstID < IRG_ARRAY_COUNT(irg_Insts));
    irg_inst* Inst = irg_Insts + (InstID * IsInRange);

    return (Inst);
}

IRG_FUNC irg_inst_id irg_MakeInst(irg_inst_kind Kind)
{
    IRG_ASSERT(irg_InstCount < IRG_ARRAY_COUNT(irg_Insts));

    irg_inst_id InstID = irg_InstCount++;
    irg_inst* Inst = irg_GetInst(InstID);

    *Inst = (irg_inst){0};
    Inst->Kind = Kind;

    return (InstID);
}

IRG_FUNC irg_inst_id irg_Integer(irg_usize Value)
{
    irg_inst_id InstID = irg_MakeInst(IRG_INST_KIND_INTEGER);
    irg_inst* Inst = irg_GetInst(InstID);

    Inst->IntegerValue = Value;

    return (InstID);
}

IRG_FUNC irg_inst_id irg_Negate(irg_inst_id Operand)
{
    irg_inst_id InstID = irg_MakeInst(IRG_INST_KIND_NEGATE);
    irg_inst* Inst = irg_GetInst(InstID);

    Inst->Operand = Operand;

    return (InstID);
}

IRG_FUNC irg_inst_id irg_Add(irg_inst_id Left, irg_inst_id Right)
{
    irg_inst_id InstID = irg_MakeInst(IRG_INST_KIND_ADD);
    irg_inst* Inst = irg_GetInst(InstID);

    Inst->Left = Left;
    Inst->Right = Right;

    return (InstID);
}

IRG_FUNC irg_inst_id irg_Sub(irg_inst_id Left, irg_inst_id Right)
{
    irg_inst_id InstID = irg_MakeInst(IRG_INST_KIND_SUB);
    irg_inst* Inst = irg_GetInst(InstID);

    Inst->Left = Left;
    Inst->Right = Right;

    return (InstID);
}

IRG_FUNC irg_inst_id irg_Mul(irg_inst_id Left, irg_inst_id Right)
{
    irg_inst_id InstID = irg_MakeInst(IRG_INST_KIND_MUL);
    irg_inst* Inst = irg_GetInst(InstID);

    Inst->Left = Left;
    Inst->Right = Right;

    return (InstID);
}

IRG_FUNC irg_inst_id irg_Div(irg_inst_id Left, irg_inst_id Right)
{
    irg_inst_id InstID = irg_MakeInst(IRG_INST_KIND_DIV);
    irg_inst* Inst = irg_GetInst(InstID);

    Inst->Left = Left;
    Inst->Right = Right;

    return (InstID);
}

IRG_FUNC irg_inst_id irg_Mod(irg_inst_id Left, irg_inst_id Right)
{
    irg_inst_id InstID = irg_MakeInst(IRG_INST_KIND_MOD);
    irg_inst* Inst = irg_GetInst(InstID);

    Inst->Left = Left;
    Inst->Right = Right;

    return (InstID);
}

///////////////////////////////////////////////////////////
// NOTE(vak): Generation
///////////////////////////////////////////////////////////

typedef struct
{
    irg_usize At;
    irg_u8    Buffer[65536];
} irg_writer;

static irg_writer irg_Writer = {0};

IRG_FUNC void irg_WriteBytes(void* Bytes, irg_usize Size)
{
    IRG_ASSERT(irg_Writer.At + Size <= sizeof(irg_Writer.Buffer));

    irg_u8* Dest = irg_Writer.Buffer + irg_Writer.At;
    irg_u8* Source = (irg_u8*)Bytes;

    for (irg_usize Index = 0; Index < Size; Index++)
        Dest[Index] = Source[Index];

    irg_Writer.At += Size;
}

IRG_FUNC void irg_U8 (irg_u8  V) { irg_WriteBytes(&V, 1); }
IRG_FUNC void irg_U16(irg_u16 V) { irg_WriteBytes(&V, 2); }
IRG_FUNC void irg_U24(irg_u32 V) { irg_WriteBytes(&V, 3); }
IRG_FUNC void irg_U32(irg_u32 V) { irg_WriteBytes(&V, 4); }
IRG_FUNC void irg_U40(irg_u64 V) { irg_WriteBytes(&V, 5); }
IRG_FUNC void irg_U48(irg_u64 V) { irg_WriteBytes(&V, 6); }
IRG_FUNC void irg_U56(irg_u64 V) { irg_WriteBytes(&V, 7); }
IRG_FUNC void irg_U64(irg_u64 V) { irg_WriteBytes(&V, 8); }

IRG_FUNC void irg_x64_Inst(irg_inst_id InstID);

IRG_FUNC void irg_x64_Unary(irg_inst_id Operand)
{
    irg_x64_Inst(Operand);

    // NOTE(vak):
    // RAX = Operand
}

IRG_FUNC void irg_x64_Binary(irg_inst_id Left, irg_inst_id Right)
{
    irg_x64_Inst(Right);
    irg_U8(0x50); // NOTE(vak): 50 push rax
    irg_x64_Inst(Left);
    irg_U8(0x59); // NOTE(vak): 59 pop rcx

    // NOTE(vak):
    // RAX = Left
    // RCX = Right
}

IRG_FUNC void irg_x64_Inst(irg_inst_id InstID)
{
    if (InstID == 0)
        return;

    irg_inst* Inst = irg_GetInst(InstID);

    switch (Inst->Kind)
    {
        default: IRG_ASSERT(!"Unimplemented inst kind"); break;

        case IRG_INST_KIND_INTEGER:
        {
            // NOTE(vak):
            // 48 b8 [Imm64]    mov rax, Imm64

            irg_U16(0xb848);
            irg_U64(Inst->IntegerValue);
        } break;

        case IRG_INST_KIND_NEGATE:
        {
            irg_x64_Unary(Inst->Operand);

            // NOTE(vak):
            // 48 f7 d8 neg rax
            irg_U24(0xd8f748);
        } break;

        case IRG_INST_KIND_ADD:
        {
            irg_x64_Binary(Inst->Left, Inst->Right);

            // NOTE(vak):
            // 48 03 c1 add rax, rcx
            irg_U24(0xc10348);
        } break;

        case IRG_INST_KIND_SUB:
        {
            irg_x64_Binary(Inst->Left, Inst->Right);

            // NOTE(vak):
            // 48 2b c1 sub rax, rcx
            irg_U24(0xc12b48);
        } break;

        case IRG_INST_KIND_MUL:
        {
            irg_x64_Binary(Inst->Left, Inst->Right);

            // NOTE(vak):
            // 48 0f af c1 imul rax, rcx
            irg_U32(0xc1af0f48);
        } break;

        case IRG_INST_KIND_DIV:
        {
            irg_x64_Binary(Inst->Left, Inst->Right);

            // NOTE(vak):
            // 48 99        cqo
            // 48 f7 f9     idiv rcx
            irg_U40(0xf9f7489948);
        } break;

        case IRG_INST_KIND_MOD:
        {
            irg_x64_Binary(Inst->Left, Inst->Right);

            // NOTE(vak):
            // 48 99        cqo
            // 48 f7 f9     idiv rcx
            // 48 8b c2     mov rax, rdx
            irg_U64(0xc28b48f9f7489948);
        } break;
    }
}

IRG_FUNC irg_code irg_GenerateX64(irg_inst_id InstID)
{
    irg_usize BeginOffset = irg_Writer.At;

    irg_x64_Inst(InstID);

    // NOTE(vak):
    // c3 ret
    irg_U8(0xc3);

    irg_usize EndOffset = irg_Writer.At;

    irg_code Result =
    {
        .Base = irg_Writer.Buffer + BeginOffset,
        .Size = EndOffset - BeginOffset,
    };

    return (Result);
}

#endif // NOTE(vak): IR_GEN_IMPLEMENTATION

#endif // NOTE(VAK): __IR_GEN_H__

