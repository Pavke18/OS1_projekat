/*
 * event.cpp
 *
 *  Created on: Aug 25, 2020
 *      Author: OS1
 */

#include "event.h"
#include"System.h"
#include"stdio.h"

Event::Event(IVTNo ivtNo){
	System::lock();
	this->myImpl=new KernelEv(ivtNo);
	System::unlock();
}

Event::~Event(){
	System::lock();
	delete myImpl;
	myImpl=0;
	System::unlock();
}

void Event::wait(){
	//printf("EVENT WAIT\n");
	myImpl->wait();
}

void Event::signal(){
	//printf("EVENT SIGNAL\n");
	myImpl->signal();
}
