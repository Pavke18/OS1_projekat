/*
 * kerSem.h
 *
 *  Created on: Aug 19, 2020
 *      Author: OS1
 */

#ifndef KERSEM_H_
#define KERSEM_H_

#include"listPCB.h"
#include "listSem.h"
#include "semaphor.h"

class KernelSem {
public:
	KernelSem(int init);
	~KernelSem();

	int wait(Time maxTimeToWait);
	int signal(int n);

	void timePassSem();//umanjujemo vreme cekanja niti na semaforu

protected:
	void deblock();//pomocna fja koja deblokira jednu nit blokiranu na semaforu

	friend class ListKernelSem;
	friend class Semaphore;
	friend class ListPCB;
private:
	int value;//vrednost semafora
	ListPCB *waitSignal,*waitSignalOrTime; //2 liste blokiranih niti
	//(obe grupe mogu biti odblokirane pomocu signal, a samo druga grupa moze se odblokirati nakon vremena maxTimeToWait bez poziva signal)
};

#endif /* KERSEM_H_ */
