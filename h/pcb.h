/*
 * pcb.h
 *
 *  Created on: Aug 7, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include"thread.h"
#include"listPCB.h"
#include"System.h"
#include"kerSem.h"
#include"kernelEv.h"

typedef enum{NEW, READY, RUNNING, BLOCKED, FINISHED} State;
const StackSize maxStackSize =65535;

class PCB {
private:
	static ID last_id;
	ID id;
	volatile unsigned int ss,sp,bp;
	StackSize stackSize;
	Time timeSlice;
	State state;
	Thread* myThread;
	unsigned int* myStack;

	volatile Time timeWaitOnSem;//maksimalno vreme cekanja niti na nekom semaforu(=0 neograniceno vreme cekanja)
	int flagTime;//fleg koji nam govori da je nit odblokirana na semaforu istekom vremena
protected:
	friend class Thread;
	friend class ListPCB;
	friend class System;
	friend class KernelSem;
	friend class Semaphore;
	friend class KernelEv;
public:
	PCB(Thread* myThread, StackSize stackSize, Time timeSlice);
	~PCB();
	void initStack();
	static void wrapper();
	void start();

	ListPCB* waitingThreads; //red niti koji cekaju na zavrsetak myThread niti
};

#endif /* PCB_H_ */
