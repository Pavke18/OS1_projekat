/*
 * kernelEv.h
 *
 *  Created on: Aug 25, 2020
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

#include"event.h"
#include"pcb.h"
#include"ivtEntry.h"

class KernelEv {
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();
	void wait();
	void signal();
private:
	PCB*owner;//vlasnik dogadjaja
	PCB* blocked; //pokazivac na PCB blokirane niti na ovom dogadjaju
	IVTNo ivtNo; //broj ulaza u IVT za ovaj dogadjaj
};

#endif /* KERNELEV_H_ */
