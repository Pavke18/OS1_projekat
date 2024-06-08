/*
 * ivtEntry.cpp
 *
 *  Created on: Aug 25, 2020
 *      Author: OS1
 */

#include "ivtEntry.h"
#include"dos.h"
#include"System.h"

IVTEntry* IVTEntry::IVT[256]={0};

IVTEntry::IVTEntry(IVTNo num,pInterrupt intRoutine){
	ivtNum=num;
	oldRoutine=0;
#ifndef BCC_BLOCK_IGNORE
	asm pushf;
	asm cli;
	oldRoutine=getvect(ivtNum);
	setvect(ivtNum,intRoutine);
	asm popf;
#endif
	IVT[ivtNum]=this;
}

IVTEntry::~IVTEntry(){
#ifndef BCC_BLOCK_IGNORE
	asm pushf;
	asm cli;
	setvect(ivtNum,oldRoutine);
	asm popf;
#endif
	IVT[ivtNum]=0;
}

void IVTEntry::signal(){
	System::KernelEvents[ivtNum]->signal();
}

