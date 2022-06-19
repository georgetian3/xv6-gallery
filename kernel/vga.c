// Source: https://github.com/keshavgupta21/xv6-vga/blob/vga_mod/kernel/vga.c


//
// empty VGA driver.
// qemu ... -device VGA -display cocoa
//
// pci.c maps the VGA framebuffer at 0x40000000 and
// passes that address to vga_init().
//
// vm.c maps the VGA "IO ports" at 0x3000000.
//
// we're talking to hw/display/vga.c in the qemu source.
// you can modify that file to generate debugging output.
//
// http://www.osdever.net/FreeVGA/home.htm
// https://wiki.osdev.org/VGA_Hardware

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
//#include "vga.h"
//#include "font.h"

static volatile uint8 * const VGA_BASE = (uint8*) 0x3000000L;
volatile uint8 __attribute__((unused)) discard; // write to this to discard



#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF
/*			COLOR emulation		MONO emulation */
#define VGA_CRTC_INDEX		0x3D4		/* 0x3B4 */
#define VGA_CRTC_DATA		0x3D5		/* 0x3B5 */
#define	VGA_INSTAT_READ		0x3DA

#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21

// Source: https://files.osdev.org/mirrors/geezer/osd/graphics/modes.c
unsigned char g_320x200x256[] =
{
    /* MISC */
	0x63,
    /* SEQ */
	0x03, 0x01, 0x0F, 0x00, 0x0E,
    /* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
	0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
	0xFF,
    /* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
	0xFF,
    /* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x41, 0x00, 0x0F, 0x00,	0x00
};

void write_regs(unsigned char *regs)
{
	unsigned i;
  /* write MISCELLANEOUS reg */
	VGA_BASE[VGA_MISC_WRITE] = *regs;
	regs++;
  /* write SEQUENCER regs */
	for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
	{
		VGA_BASE[VGA_SEQ_INDEX] = i;
		VGA_BASE[VGA_SEQ_DATA] = *regs;
		regs++;
	}
  /* unlock CRTC registers */
	VGA_BASE[VGA_CRTC_INDEX] = 0x03;
	VGA_BASE[VGA_CRTC_DATA] = VGA_BASE[VGA_CRTC_DATA] | 0x80;
	VGA_BASE[VGA_CRTC_INDEX] = 0x11;
	VGA_BASE[VGA_CRTC_DATA] = VGA_BASE[VGA_CRTC_DATA] & ~0x80;
  /* make sure they remain unlocked */
	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;
  /* write CRTC regs */
	for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
	{
		VGA_BASE[VGA_CRTC_INDEX] = i;
		VGA_BASE[VGA_CRTC_DATA] = *regs;
		regs++;
	}
  /* write GRAPHICS CONTROLLER regs */
	for(i = 0; i < VGA_NUM_GC_REGS; i++)
	{
		VGA_BASE[VGA_GC_INDEX] = i;
		VGA_BASE[VGA_GC_DATA] = *regs;
		regs++;
	}
  /* write ATTRIBUTE CONTROLLER regs */
	for(i = 0; i < VGA_NUM_AC_REGS; i++)
	{
		(void)VGA_BASE[VGA_INSTAT_READ];
		VGA_BASE[VGA_AC_INDEX] = i;
		VGA_BASE[VGA_AC_WRITE] = *regs;
		regs++;
	}
    /* lock 16-color palette and unblank display */
	(void)VGA_BASE[VGA_INSTAT_READ];
	VGA_BASE[VGA_AC_INDEX] = 0x20;
}

void
vgainit(void)
{
  printf("initializing VGA..\n");

  write_regs(g_320x200x256);
  VGA_BASE[0x3c8] = 0;
  for (int r = 0; r < 64; r += 8) {
    for (int g = 0; g < 64; g += 8) {
      for (int b = 0; b < 64; b += 16) {
        VGA_BASE[0x3c9] = r;
        VGA_BASE[0x3c9] = g;
        VGA_BASE[0x3c9] = b;
      }
    }
  }

  printf("completed VGA initialization.\n");
  //dump_vga_config();

}