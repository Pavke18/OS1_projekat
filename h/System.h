/*
 * system.h
 *
 *  Created on: Aug 7, 2020
 *      Author: OS1
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include"thread.h"
#include"pcb.h"
#include"listPCB.h"
#include "listSem.h"
#include"kernelEv.h"

typedef void interrupt(*pInterrupt)(...);

class System {
private:
	static volatile int lockFlag; //fleg za zabranu promene konteksta
protected:
	friend class ListPCB;
	friend class PCB;
	friend class Thread;
	friend class ListKernelSem;
	friend class KernelSem;
	friend class Semaphore;
public:
	static volatile int requiredContextSwitch;
	static volatile PCB* running; //pokazivac na PCB niti koja se trenutno izvrsava u sistemu
	static Thread *starting, *idle; //pokazivac na pocetnu i besposlenu niti
	static volatile int counter;
	static ListPCB* allThreads;

	static ListKernelSem *allSemaphores;
	static void timePassSem();//umanjujemo vreme cekanja niti na semaforu

	static KernelEv* KernelEvents[256]; //tabela svih dogadjaja na mestu njihovih ulaza

	static void unlock(); //dozvoljava promenu konteksta
	static void lock(); //onemogucuje promenu konteksta

	static void interrupt timer(...);
	static pInterrupt oldRoutine;

	static void init();
	static void restore();
};

#endif /* SYSTEM_H_ */
