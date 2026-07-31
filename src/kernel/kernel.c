#include <limine.h>
#include <stddef.h>
#include <stdint.h>
#include <terminal/flanterm.h>
#include <terminal/flanterm_backends/fb.h>

struct flanterm_context *global_flanterm = NULL;

volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0
};

volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

void kmain(void) {
struct limine_framebuffer *fb = framebuffer_request.response->framebuffers[0];
    static uint32_t default_bg = 0x00000000;
    static uint32_t default_fg = 0xAAAAAAAA;
    global_flanterm = flanterm_fb_init(
        NULL, 
        NULL, 
        (uint32_t *)fb->address, 
        fb->width, 
        fb->height, 
        fb->pitch,
        fb->red_mask_size, 
        fb->red_mask_shift, 
        fb->green_mask_size, 
        fb->green_mask_shift,
        fb->blue_mask_size, 
        fb->blue_mask_shift, 
        NULL, 
        NULL, 
        NULL, 
        &default_bg, 
        &default_fg,
        NULL, 
        NULL, 
        NULL, 
        0, 
        0, 
        1, 
        0, 
        0, 
        0, 
        0
    );

const char msg[] = "Hello world\n";
flanterm_write(global_flanterm, msg, sizeof(msg));

while(1){}
}