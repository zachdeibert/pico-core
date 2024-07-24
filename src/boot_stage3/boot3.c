#include <hardware/regs/addressmap.h>
#include <hardware/regs/m0plus.h>

extern void (*__logical_binary_start)();
extern char __StackTop;

static void __boot3_isr_invalid(void) {
    __builtin_trap();
}

static void __boot3_reset_handler(void) {
    *((void (***)())(PPB_BASE + M0PLUS_VTOR_OFFSET)) = &__logical_binary_start;
    (&__logical_binary_start)[1]();
    __builtin_trap();
}

const void *const __boot3_vector_table[]
        = { &__StackTop,         __boot3_reset_handler, __boot3_isr_invalid, __boot3_isr_invalid, __boot3_isr_invalid,
            __boot3_isr_invalid, __boot3_isr_invalid,   __boot3_isr_invalid, __boot3_isr_invalid, __boot3_isr_invalid,
            __boot3_isr_invalid, __boot3_isr_invalid,   __boot3_isr_invalid, __boot3_isr_invalid, __boot3_isr_invalid,
            __boot3_isr_invalid, __boot3_isr_invalid,   __boot3_isr_invalid, __boot3_isr_invalid, __boot3_isr_invalid,
            __boot3_isr_invalid, __boot3_isr_invalid,   __boot3_isr_invalid, __boot3_isr_invalid, __boot3_isr_invalid,
            __boot3_isr_invalid, __boot3_isr_invalid,   __boot3_isr_invalid, __boot3_isr_invalid, __boot3_isr_invalid,
            __boot3_isr_invalid, __boot3_isr_invalid,   __boot3_isr_invalid, __boot3_isr_invalid, __boot3_isr_invalid,
            __boot3_isr_invalid, __boot3_isr_invalid,   __boot3_isr_invalid, __boot3_isr_invalid, __boot3_isr_invalid,
            __boot3_isr_invalid, __boot3_isr_invalid,   __boot3_isr_invalid, __boot3_isr_invalid, __boot3_isr_invalid,
            __boot3_isr_invalid, __boot3_isr_invalid,   __boot3_isr_invalid };
