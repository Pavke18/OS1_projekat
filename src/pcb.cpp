/*
 * pcb.cpp
 *
 *  Created on: Aug 7, 2020
 *      Author: OS1
 */

#include "pcb.h"
#include"SCHEDULE.H"
#include"thread.h"
#include"listPCB.h"
#include"System.h"
#include"stdio.h"
#include<dos.h>
#include"iostream.h"

ID PCB::last_id=0; //id=0 za pocetnu nit (main thread), id=1 za idle thread, ove dve niti pravimo na pocetku

PCB::PCB(Thread* myThr, StackSize stackS,Time timeS){
	if(stackS>maxStackSize)
		stackS=maxStackSize;
	id=last_id++;
	myThread=myThr;
	myStack=0;
	stackSize=stackS;
	timeSlice=timeS;
	state=NEW;
	waitingThreads=new ListPCB();

	timeWaitOnSem=0; //za blokirane niti na semaforu na ograniceno vreme(MORA DA SE UMANJUJE U TAJMERUU!)
	flagTime=0;//=1 kada se deblokira u tajmeru!!!!!!

	//u listu allThreads ubacujemo sve niti (osim idle i starting!) koje su stvorene u programu
	if(this->id>1){
		System::allThreads->insert(this);
	}
}

void PCB::initStack(){
	System::lock();
	unsigned int size=stackSize/sizeof(unsigned int);
	myStack=new unsigned int[size];
	unsigned int tss,tsp,cs,ip;
#ifndef BCC_BLOCK_IGNORE
	cs=FP_SEG(PCB::wrapper);
	ip=FP_OFF(PCB::wrapper);
	tss=FP_SEG(myStack+size-12);
	tsp=FP_OFF(myStack+size-12);
#endif
	myStack[size-1]=0x200; //postavljamo bit I na 1 u PSW
	myStack[size-2]=cs;
	myStack[size-3]=ip;

	//myStack[size-4]=AX
	//myStack[size-5]=BX
	//myStack[size-6]=CX
	//myStack[size-7]=DX
	//myStack[size-8]=ES
	//myStack[size-9]=DS
	//myStack[size-10]=SI
	//myStack[size-11]=DI
	//myStack[size-12]=BP

	this->sp=tsp;
	this->ss=tss;
	this->bp=sp;
	System::unlock();
}

void PCB::wrapper(){
	System::running->myThread->run();

	System::lock();
	//printf("END WRAPPER Running exit run id=%d\n", System::running->id);
	//ova running nit je zavrsila svoj posao
	System::running->state=FINISHED;
	//treba sve niti koje su cekale na zavrsetak ove running niti obeleziti kao spremne i ubaciti u rasporedjivac
	while(System::running->waitingThreads->size>0){
		PCB*tmp=System::running->waitingThreads->deleteFirst();
		//cout<<"Oslobadjamo nit id="<<tmp->id<<endl;
		if(tmp!=0 && tmp->state!=FINISHED){
			tmp->state=READY;
			//cout<<"Odblokiramo nit id="<<tmp->id<<"state="<<tmp->state<<endl;
			if( tmp->id>1){
				Scheduler::put(tmp);
				//cout<<"Nit id="<<tmp->id<<" ide u scheduler"<<endl;
			}
		}
		tmp=0;
	}
	delete System::running->waitingThreads;
	System::running->waitingThreads=0;
	System::unlock();

	dispatch();
}

void PCB::start(){
	System::lock();
	//da ne bi jedna nit bila vise puta startovana
	if(this->state==NEW){
		this->initStack();
		this->state=READY;
		if(this->id>1){
			//cout<<"START PUT IN SCHEDULER id="<<this->id<<endl;
			Scheduler::put(this);
		}
	}
	System::unlock();
}

PCB::~PCB(){
	//cout<<"DESTRUKTOR PCB id="<<this->id<<endl;
	if(myStack!=0){
		delete myStack;
		myStack=0;
	}
	if(waitingThreads!=0){
		delete waitingThreads;
		waitingThreads=0;
	}
}

