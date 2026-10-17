
void* memset(void* DestInit, irg_s32 Byte, irg_usize Size)
{
    irg_u8* Dest = (irg_u8*)DestInit;

    while (Size--)
        *Dest++ = (irg_u8)Byte;

    return (DestInit);
}

void* memcpy(void* DestInit, const void* SourceInit, irg_usize Size)
{
    irg_u8* Dest = (irg_u8*)DestInit;
    irg_u8* Source = (irg_u8*)SourceInit;

    while (Size--)
        *Dest++ = *Source++;

    return (DestInit);
}

typedef irg_ssize linux_machine_code_entry(void);

static irg_ssize CallMachineCode(irg_code Code)
{
    irg_ssize ExecutableMemory = 0;

    register irg_usize R10 __asm__("r10") = 0x22; // NOTE(vak): MAP_PRIVATE | MAP_ANON
    register irg_ssize R8  __asm__("r8")  = -1;
    register irg_usize R9  __asm__("r9")  = 0;

    __asm__ volatile (
        "syscall" :
        "=a"(ExecutableMemory) :
        "a"(9),             // NOTE(vak): __NR_mmap
        "D"(0),
        "S"(Code.Size),
        "d"(0x7),           // NOTE(vak): PROT_READ|PROT_WRITE|PROT_EXEC
        "r"(R10),
        "r"(R8),
        "r"(R9) :
        "memory", "rcx", "r11"
    );

    IRG_ASSERT(ExecutableMemory > 0);

    memcpy((void*)ExecutableMemory, Code.Base, Code.Size);

    linux_machine_code_entry* CodeEntry = (linux_machine_code_entry*)
        ExecutableMemory;

    irg_ssize Result = CodeEntry();

    __asm__ volatile (
        "syscall" ::
        "a"(11),            // NOTE(vak): __NR_munmap
        "D"(ExecutableMemory),
        "S"(Code.Size) :
        "memory", "rcx", "r11"
    );

    return (Result);
}

void LinuxBootstrap(
    irg_s32 ArgCount,
    char* Args[],
    char* Envp[]
)
{
    Test();

    __asm__ volatile (
        "syscall" ::
        "a"(60), "D"(0)
    );
}

__attribute__((naked))
void LinuxEntry(void)
{
    __asm__ volatile (
        "mov 0(%rsp),           %edi\n"
        "lea 8(%rsp),           %rsi\n"
        "lea 16(%rsp, %rdi, 8), %rdx\n"
        "call LinuxBootstrap\n"
    );
}

