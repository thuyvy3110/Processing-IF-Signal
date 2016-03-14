#include "include.h"
// Globals

//Buffer is initialize for UART
#define BUFFER_SIZE 255
volatile unsigned char BUFFER[BUFFER_SIZE];

volatile int iBufferStart = 0; // Valid data at beginning of queue
volatile int iBufferEnd = 0; // Blank space to put data

void Buffer_EnQueue(unsigned char data) {
	BUFFER[iBufferEnd] = data;
	iBufferEnd++;
	iBufferEnd %= BUFFER_SIZE; // Circular buffer
}
unsigned char Buffer_DeQueue() {
	unsigned char temp;
	if (!Buffer_IsEmpty()) {
		temp = BUFFER[iBufferStart];
		iBufferStart++;
		iBufferStart %= BUFFER_SIZE; // Circular buffer
		return temp;
	} else {
// program should check if buffer is empty before calling this function!
		return 0;
	}
}
unsigned char Buffer_IsEmpty() {
	if (iBufferStart == iBufferEnd)
		return 1;
	else
		return 0;
}
unsigned char Buffer_IsFull() {
	if (((iBufferEnd - 1) % BUFFER_SIZE) == iBufferStart)
		return 1;
	else
		return 0;
}
//------------------------------------CIRCILE BUFFER (DSP APPLICATION) -----------------------------------------------//
void wrap(uint32_t* w, uint32_t** p, int M){

}


//-----------------------------------------------------------------------------------//
//void ArrayInit(uint32_t* input, int n) {
//	input = (uint32_t*)calloc(n, sizeof(uint32_t));
//}
//void ArrayInput(uint32_t* input, int n) {
//	if (input == NULL) {
//		return;
//	}
//	if (n <= 0)
//		return;
//	ArrayInit(input, n);
//	//if(input->members != NULL)
//}
//void ArrayFree(uint32_t* input) {
//	if (input != 0)
//		if (input != NULL) {
//			free(input);
//		}
//}
//


