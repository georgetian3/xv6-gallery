#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

///////////////////////////////////////////////////////////////////////////////
#include "vga.h"
#include "msg.h"
///////////////////////////////////////////////////////////////////////////////


uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

///////////////////////////////////////////////////////////////////////////////
uint64
sys_setpixel(void)
{
  static int x, y, i;
  static int rgb[3];

  argint(0, &x);
  argint(1, &y);

  // pixel must be within the screen bounds
  if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
    return -1;

  for (i = 0; i < 3; i++) {
    // reading rgb values
    argint(i + 2, rgb + i);
    // checking bounds
    if (rgb[i] < 0) {
      rgb[i] = 0;
    } else if (rgb[i] > 255) {
      rgb[i] = 255;
    }
  }

  // converting RGB values to the index of the closest color
  rgb[0] -= rgb[0] % 64;
  rgb[1] /= 8;
  rgb[1] -= rgb[1] % 4;
  rgb[2] /= 64;
  *(char*)(FRAMEBUFFER + WIDTH * y + x) = rgb[0] + rgb[1] + rgb[2];
  return 0;
}


uint64
sys_getmsg()
{
  static int c;
  acquire(&msglock);
  if (front == back) {
    release(&msglock);
    return -1;
  }
  c = msgqueue[front];
  increase(front);

  release(&msglock);

  return c;

}

uint64
sys_setmsgstate(void)
{
  acquire(&msglock);
  argint(0, &msgstate);
  // empty the queue if messaging was enabled
  if (msgstate == 1) {
    front = 0;
    back = 0;
  }
  release(&msglock);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
