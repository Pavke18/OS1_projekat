/*
 * kernelEv.cpp
 *
 *  Created on: Aug 25, 2020
 *      Author: OS1
 */

#include "kernelEv.h"
#include"SCHEDULE.H"
#include"ivtEntry.h"

KernelEv::KernelEv(IVTNo num){
	owner=(PCB*)System::running; //nit koja se izvrsava prilikom stavaranja dogadjaja je vlasnik
	blocked=0;
	ivtNo=num;
	System::KernelEvents[ivtNo]=this;//upisujemo ovaj dogadjaj u tabelu pod njegovim brojem ulaza ivtNo
}

KernelEv::~KernelEv(){
	if(blocked!=0){
		if(blocked->state!=FINISHED){
			blocked->state=READY;
			Scheduler::put(blocked);
		}
		blocked=0;
	}
	owner=0;
	System::KernelEvents[ivtNo]=0;
}

void KernelEv::wait(){
	if(owner==System::running && blocked==0){//pozivajuca nit se blokira ako vec nije blokirana
		System::lock();
		System::running->state=BLOCKED;
		blocked=(PCB*)System::running;
		System::unlock();
		dispatch();
	}
}

void KernelEv::signal(){//ovu fju poziva prekidna rutina koja je vezana za prekid dogadjaja!!!
	if(blocked!=0){//deblokiram nit blokiranu na dogadjaju
		System::lock();
		if(blocked->state!=FINISHED){
			blocked->state=READY;
			Scheduler::put(blocked);
		}
		blocked=0;
		System::unlock();
		//dispatch(); //jel ovo treba ???? valjda da
		//kad se dogodi prekid treba uvek vrsiti preuzimanje
	}
}
