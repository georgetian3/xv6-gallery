#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"


#include "msg.h"


char msgqueue[NMSG]; // queue using circular array
int listening = 0;
int front = 0;
int back = 0;

struct spinlock msglock;
void
msginit()
{
  initlock(&msglock, "msg");
}

void
putmsg(char c)
{
  acquire(&msglock);

  msgqueue[back] = c;
  increase(back);
  if(front == back)
  {
    increase(front);
  }

  release(&msglock);
}
