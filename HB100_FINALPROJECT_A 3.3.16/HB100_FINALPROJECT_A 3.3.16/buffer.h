#ifndef BUFFER_H_
#define BUFFER_H_

void Buffer_EnQueue(unsigned char data);
unsigned char Buffer_DeQueue(void);
unsigned char Buffer_IsEmpty(void);
unsigned char Buffer_IsFull(void);

#endif
