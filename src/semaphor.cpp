/*
 * semaphor.cpp
 *
 *  Created on: Aug 19, 2020
 *      Author: OS1
 */

#include "semaphor.h"

#include"System.h"
#include"kerSem.h"
#include"iostream.h"
#include"stdio.h"

Semaphore::Semaphore(int init){
	System::lock();
	this->myImpl=new KernelSem(init);
	System::unlock();
}

Semaphore::~Semaphore(){
	System::lock();
	delete myImpl;
	myImpl=0;
	System::unlock();
}

int Semaphore::wait(Time t){
	//printf("WAIT SEMAPHORE\n");
	return this->myImpl->wait(t);
}

int Semaphore::signal(int n){
	//printf("SIGNAL SEMAPHORE\n");
	return this->myImpl->signal(n);
}

int Semaphore::val()const{
	return this->myImpl->value;
}
