/*
 * ivtEntry.h
 *
 *  Created on: Aug 25, 2020
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include"event.h"

typedef void interrupt (*pInterrupt)(...);

#define PREPAREENTRY(num,old)\
	void interrupt newRoutine##num(...);\
	IVTEntry ivtEntry##num = IVTEntry(num,newRoutine##num);\
	void interrupt newRoutine##num(...){\
		ivtEntry##num.signal();\
		if (old == 1) ivtEntry##num.oldRoutine();\
	}


class IVTEntry {
public:
	IVTEntry(IVTNo num,pInterrupt intRoutine);
	~IVTEntry();
	void signal(); //poziva signal KernelEv!
	//void oldRoutine(); //poziva staru prekidnu rutinu, ovo valjda ne treba?
	pInterrupt oldRoutine;
	static IVTEntry* IVT[256]; //interrupt vector table ima tacno 256 ulaza
private:
	IVTNo ivtNum;
};

#endif /* IVTENTRY_H_ */
