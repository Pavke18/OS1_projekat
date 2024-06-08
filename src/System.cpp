/*
 * system.cpp
 *
 *  Created on: Aug 7, 2020
 *      Author: OS1
 */

#include "System.h"
#include"pcb.h"
#include"listPCB.h"
#include"thread.h"
#include"stdio.h"
#include"idleThr.h"
#include"SCHEDULE.H"
#include"dos.h"
#include"iostream.h"
#include "listSem.h"
#include "semaphor.h"
#include"kerSem.h"


volatile int System::lockFlag=0; //0-dozvoljena promena konteksta, sve ostalo-zabranjena promena konteksta
volatile int System::requiredContextSwitch=0;//1-zahtevana eksplicitna promena konteksta (preko dispatcha)
volatile int System::counter = 0;
volatile PCB* System::running = 0;
Thread* System::starting=0;
Thread* System::idle=0;
ListPCB* System::allThreads=0;
pInterrupt System::oldRoutine=0;
ListKernelSem* System::allSemaphores=new ListKernelSem();
KernelEv*System::KernelEvents[256]={0};

extern void tick();

void System::unlock(){
	lockFlag=(--lockFlag<0?0:lockFlag);
	//lockFlag=0;
}

void System::lock(){
	lockFlag++;
	//lockFlag=1;
}

void interrupt System::timer(...){
	static unsigned int tsp,tss,tbp,old;

	old=0;//old=1 treba da pozovemo staru prekidnu rutinu na 60h jer je zahtev za prekid stigao od hardvera

	//printf("Running id=%d  counter=%d\n",running->id,counter);

	if(requiredContextSwitch==0){
		if(System::counter>0)
			counter--;
		old=1; //da bi pozvali staru prekidnu rutinu
		tick();
		System::timePassSem();
	}

	//promenu konteksta vrsimo samo ako je isteklo vreme izvrsavanja running niti
	//ili ako je trazena esplicitna promena konteksta
	if((counter==0 && running->timeSlice!=0) || requiredContextSwitch){
		if(System::lockFlag==0){

			//printf("Running id=%d, running state=%d,runnig timeSlice=%d, running timeToWaitOnSem=%d\n",System::running->id,System::running->state,System::running->timeSlice,System::running->timeWaitOnSem);

			requiredContextSwitch=0;
#ifndef BCC_BLOCK_IGNORE
			asm{
				mov tsp,sp;
				mov tss,ss;
				mov tbp,bp;
			}
#endif
			System::running->sp=tsp;
			System::running->ss=tss;
			System::running->bp=tbp;

			if(running->state==RUNNING)
				running->state=READY;

			if(running->state==FINISHED){
				delete running->myStack;//oslobadjam memoriju ako se nit zavrsila
				running->myStack=0;
			}

			//printf("Promena konteksta\n");

			//schedlue
			if(running->id>1 && running->state==READY){
				Scheduler::put((PCB*)System::running);
			}
			System::running=Scheduler::get();
			if(running==0){
				if(System::starting!=0 && System::starting->myPCB->state==READY)
					running=System::starting->myPCB;
				else{
					running=System::idle->myPCB;
					//printf("Idle thread\n");
				}
			}

			running->state=RUNNING;

			tsp=running->sp;
			tss=running->ss;
			tbp=running->bp;
			counter=running->timeSlice;

#ifndef BCC_BLOCK_IGNORE
			asm{
				mov sp,tsp;
				mov ss,tss;
				mov bp,tbp;
			}
#endif

			//printf("Running id=%d, running state=%d\n",System::running->id,System::running->state);

		}
		else{
			requiredContextSwitch=1;//trazimo eksplicitnu promenu konteksta koja ce se izvrsiti kada lockFlag postavimo na 0
		}
	}
	if(old==1){
#ifndef BCC_BLOCK_IGNORE
		asm int 60h; //0x60
#endif

	}

}

void System::timePassSem(){
	System::lock();
	if(allSemaphores!=0){
		//cout<<"TIME PASS ALL SEMAPHORES"<<endl;
		allSemaphores->timePass();
	}
	System::unlock();
}

void System::init(){
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	oldRoutine=getvect(0x8);
	setvect(0x8,timer);
	setvect(0x60,oldRoutine);
#endif
	//cout<<"INIT SYSTEM"<<endl;
	allThreads=new ListPCB();
	starting=new Thread(maxStackSize,1);
	//starting->start();//da li ovo treba????
	running=starting->myPCB;
	running->state=RUNNING;
	counter=running->timeSlice;
	idle=new idleThr();
	idle->start(); //ovo ne treba? TREBAA
	//cout<<"starting id="<<starting->myPCB->id<<" idle id="<<idle->myPCB->id<<endl;
#ifndef BCC_BLOCK_IGNORE
	asm sti;
#endif

}

void System::restore(){
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	setvect(0x8,oldRoutine);
#endif
	//cout<<"RESTORE SYSTEM"<<endl;
	//da li treba da sacekamo da se sve niti zavrse??? mislim da se to radi u destruktorima i da je dovoljno
	delete idle;
	idle=0;
	delete starting;
	starting=0;
	delete allThreads;
	allThreads=0;
	delete allSemaphores;
	allSemaphores=0;
#ifndef BCC_BLOCK_IGNORE
	asm sti;
#endif
}
