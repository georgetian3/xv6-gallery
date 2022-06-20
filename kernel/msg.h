#ifndef MSG_H
#define MSG_H

#define NMSG 128 // max number of buffered messages

// increase/decrease the front/back of the queue
#define decrease(x) x = ((x + 127) % NMSG)
#define increase(x) x = ((x + 1  ) % NMSG)

extern struct spinlock msglock;
extern char msgqueue[NMSG];
extern int front, back;

#endif