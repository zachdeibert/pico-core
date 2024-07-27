#include <stdint.h>
#include <hardware/flash.h>
#include <hardware/regs/addressmap.h>
#include <hardware/regs/m0plus.h>
#include <pico/bootrom.h>
#include <pico/platform.h>

extern uint32_t __boot3_data_end__;
extern uint32_t __boot3_data_source__;
extern uint32_t __boot3_data_start__;
extern uint32_t __flash_app_end__;
extern uint32_t __flash_app_start__;
extern uint32_t __flash_data_end__;
extern uint32_t __flash_data_start__;
extern void (*__logical_binary_start)();
extern char __StackTop;

static void __no_inline_not_in_flash_func(flash_erase)(uint32_t offset, size_t count, void (*boot2)(void)) {
    rom_connect_internal_flash_fn connect_internal_flash
            = (rom_connect_internal_flash_fn) rom_func_lookup_inline(ROM_FUNC_CONNECT_INTERNAL_FLASH);
    rom_flash_exit_xip_fn flash_exit_xip = (rom_flash_exit_xip_fn) rom_func_lookup_inline(ROM_FUNC_FLASH_EXIT_XIP);
    rom_flash_range_erase_fn flash_range_erase
            = (rom_flash_range_erase_fn) rom_func_lookup_inline(ROM_FUNC_FLASH_RANGE_ERASE);
    rom_flash_flush_cache_fn flash_flush_cache
            = (rom_flash_flush_cache_fn) rom_func_lookup_inline(ROM_FUNC_FLASH_FLUSH_CACHE);
    __compiler_memory_barrier();

    connect_internal_flash();
    flash_exit_xip();
    flash_range_erase(offset, count, FLASH_BLOCK_SIZE, 0xD8);
    flash_flush_cache();
    boot2();
}

static void __no_inline_not_in_flash_func(flash_write)(
        uint32_t offset, const uint8_t *data, size_t count, void (*boot2)(void)) {
    rom_connect_internal_flash_fn connect_internal_flash
            = (rom_connect_internal_flash_fn) rom_func_lookup_inline(ROM_FUNC_CONNECT_INTERNAL_FLASH);
    rom_flash_exit_xip_fn flash_exit_xip = (rom_flash_exit_xip_fn) rom_func_lookup_inline(ROM_FUNC_FLASH_EXIT_XIP);
    rom_flash_range_program_fn flash_range_program
            = (rom_flash_range_program_fn) rom_func_lookup_inline(ROM_FUNC_FLASH_RANGE_PROGRAM);
    rom_flash_flush_cache_fn flash_flush_cache
            = (rom_flash_flush_cache_fn) rom_func_lookup_inline(ROM_FUNC_FLASH_FLUSH_CACHE);
    __compiler_memory_barrier();

    connect_internal_flash();
    flash_exit_xip();
    flash_range_program(offset, data, count);
    flash_flush_cache();
    boot2();
}

static void reset_handler(void) {
    if (__flash_data_start__ != 0xFFFFFFFF) {
        rom_memcpy44_fn memcpy44 = (rom_memcpy44_fn) rom_func_lookup_inline(ROM_FUNC_MEMCPY44);
        memcpy44(&__boot3_data_start__,
                 &__boot3_data_source__,
                 (unsigned) (&__boot3_data_end__ - &__boot3_data_start__) * sizeof(__boot3_data_start__));

        uint32_t boot2_mem[256 / sizeof(uint32_t)];
        memcpy44(boot2_mem, (void *) XIP_BASE, sizeof(boot2_mem));
        void (*boot2)(void) = (void (*)(void))(((uint8_t *) boot2_mem) + 1);

        flash_erase((uint32_t) &__flash_app_start__,
                    (size_t) (&__flash_app_end__ - &__flash_app_start__) * sizeof(__flash_app_start__),
                    boot2);

        uint32_t page[FLASH_PAGE_SIZE / sizeof(uint32_t)];
        for (uint32_t *in = &__flash_data_start__, *out = &__flash_app_start__; out != &__flash_app_end__;
             in += sizeof(page) / sizeof(*in), out += sizeof(page) / sizeof(*out)) {
            memcpy44(page, in, sizeof(page));
            flash_write((uint32_t) out, (uint8_t *) page, sizeof(page), boot2);
        }

        flash_erase((uint32_t) &__flash_data_start__,
                    (size_t) (&__flash_data_end__ - &__flash_data_start__) * sizeof(__flash_data_start__),
                    boot2);
    }
    *((void (***)())(PPB_BASE + M0PLUS_VTOR_OFFSET)) = &__logical_binary_start;
    (&__logical_binary_start)[1]();
    __builtin_trap();
}

static void isr_invalid(void) {
    __builtin_trap();
}

const void *const __boot3_vector_table[]
        = { &__StackTop, reset_handler, isr_invalid, isr_invalid, isr_invalid, isr_invalid, isr_invalid, isr_invalid,
            isr_invalid, isr_invalid,   isr_invalid, isr_invalid, isr_invalid, isr_invalid, isr_invalid, isr_invalid,
            isr_invalid, isr_invalid,   isr_invalid, isr_invalid, isr_invalid, isr_invalid, isr_invalid, isr_invalid,
            isr_invalid, isr_invalid,   isr_invalid, isr_invalid, isr_invalid, isr_invalid, isr_invalid, isr_invalid,
            isr_invalid, isr_invalid,   isr_invalid, isr_invalid, isr_invalid, isr_invalid, isr_invalid, isr_invalid,
            isr_invalid, isr_invalid,   isr_invalid, isr_invalid, isr_invalid, isr_invalid, isr_invalid, isr_invalid };
