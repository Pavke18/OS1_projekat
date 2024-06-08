/*
 * kerSem.cpp
 *
 *  Created on: Aug 19, 2020
 *      Author: OS1
 */

#include "kerSem.h"
#include"System.h"
#include"thread.h"
#include"listPCB.h"
#include"SCHEDULE.H"
#include"stdio.h"

KernelSem::KernelSem(int init){
	this->value=init;
	waitSignalOrTime=new ListPCB();
	waitSignal=new ListPCB();
	System::allSemaphores->insert(this);
}

KernelSem::~KernelSem(){
	while(waitSignalOrTime->getSize()>0 || waitSignal->getSize()>0)
		deblock();
	delete waitSignalOrTime;
	waitSignalOrTime=0;
	delete waitSignal;
	waitSignal=0;
	System::allSemaphores->remove(this);
}

int KernelSem::wait(Time t){
	if(--value<0){//blokiramo pozivajucu nit i izvrsimo promenu konteksta
		System::lock();
		System::running->state=BLOCKED;
		if(t==0){ //standardni wait ceka na signal da bi se nit odblokirala
			//printf("Standardni wait\n");
			this->waitSignal->insert((PCB*)System::running);
		}
		else{ //ceka da istekne t vreme kako bi se nit odblokirala ili ceka signal
			System::running->timeWaitOnSem=t;
			//printf("Max vreme cekanja je %d\n",System::running->timeWaitOnSem);
			this->waitSignalOrTime->insert((PCB*)System::running);
		}
		System::unlock();
		dispatch();
	}
	//nit se odblokirala ili nije ni bila blokirana
	System::lock();
	int ret=1;
	if(System::running->flagTime){ //ako se nit odblokirala istekom vremena
		ret=0;
		System::running->flagTime=0;
	}
	//printf("Wait niti id=%d  ret=%d\n",System::running->id,ret);
	System::unlock();
	return ret;
}

int KernelSem::signal(int n){
	System::lock();
	int ret;
	if(n==0){ //standardni signal
		//printf("Standardni signal\n");
		ret=0;
		value++;
		deblock();
		System::unlock();
		return ret;
	}
	else if(n>0){ //mora da odblokira n niti
		//printf("Signal odblokiraj %d niti",n);
		for(ret=0; ret<n && ((waitSignal->getSize()>0)||(waitSignalOrTime->getSize()>0));ret++)
			deblock();
		value+=n;
		System::unlock();
		return ret; //vraca broj odblokiranih niti
	}
	System::unlock();
	return n;//prihvacen je negativan broj n pa je signal bez efekta
}

void KernelSem::deblock(){ //odblokira jednu nit blokiranu na semafooru
	System::lock();
	//da li prvo prolazim kroz listu waitSignalOrTime ili waitSignal???
	PCB*tmp=this->waitSignalOrTime->deleteFirst();
	if(tmp==0)
		tmp=this->waitSignal->deleteFirst();
	if(tmp!=0 && tmp->state!=FINISHED){
		//printf("Odblokirana je nit id=%d\n",tmp->id);
		tmp->state=READY;
		Scheduler::put(tmp);
	}
	System::unlock();
}

