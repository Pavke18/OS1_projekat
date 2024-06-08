/*
 * thread.cpp
 *
 *  Created on: Aug 7, 2020
 *      Author: OS1
 */

#include "thread.h"
#include"pcb.h"
#include"SCHEDULE.H"
#include"stdio.h"
#include"System.h"
#include"iostream.h"

Thread::Thread(StackSize stackS, Time timeS){
	System::lock();
	//cout<<"KONSTRUKTOR THREAD"<<endl;
	myPCB=new PCB(this,stackS,timeS);
	System::unlock();
}

Thread::~Thread(){
	this->waitToComplete();//da li ovo treba? treba!
	System::lock();
	delete myPCB;
	myPCB=0;
	System::unlock();
}

void Thread::waitToComplete(){
	System::lock();
	//cout<<"Wait to complete id="<<this->myPCB->id<<endl;
	//running nit ne treba da ceka besposlenu nit, samu sebe, gotovu nit, pocetnu nit, novu nit ni nepostojecu nit
	if(this==System::idle || this->myPCB==System::running || this->myPCB->state==FINISHED
			|| this==System::starting || this->myPCB->state==NEW || this==0 ){
		//cout<<"Ne cekamo nit id="<<this->myPCB->id<<endl;
		System::unlock();
		return;
	}

	System::running->state=BLOCKED;
	this->myPCB->waitingThreads->insert((PCB*)System::running);
	//cout<<"Running je blocked id="<<System::running->id<<endl;
	System::unlock();
	dispatch();
}

void Thread::start(){
	//cout<<"START ID="<<this->myPCB->id<<endl;
	this->myPCB->start();
}

ID Thread::getId(){
	return (this!=0 && this->myPCB!=0)?this->myPCB->id:-1;
}

ID Thread::getRunningId(){
	return (System::running!=0)?System::running->id:-1;
}

Thread* Thread::getThreadById(ID id){
	System::lock();
	if(id==System::starting->myPCB->id){
		System::unlock();
		return System::starting;
	}
	if(id==System::idle->myPCB->id){
		System::unlock();
		return System::idle;
	}
	PCB*tmp=System::allThreads->find(id);
	System::unlock();
	return(tmp!=0)?tmp->myThread:0;
}


void dispatch(){ // sinhrona promena konteksta
#ifndef BCC_BLOCK_IGNORE
	asm cli;
#endif
	System::requiredContextSwitch=1;
	System::timer(); //da li ovo treba da bude posle sti???
#ifndef BCC_BLOCK_IGNORE
	asm sti;
#endif;
}
