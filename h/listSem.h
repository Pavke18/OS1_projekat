/*
 * listKSem.h
 *
 *  Created on: Aug 20, 2020
 *      Author: OS1
 */

#ifndef LISTSEM_H_
#define LISTSEM_H_

#include"kerSem.h"
#include "semaphor.h"
#include"System.h"

class ListKernelSem {
private:
	struct Elem{
		KernelSem* kSem;
		Elem* next;
		Elem(KernelSem* kSem){
			this->kSem=kSem;
			this->next=0;
		}
	};
	Elem *head, *tail;
	unsigned int size;
protected:
	friend class System;
	friend class KernelSem;
public:
	ListKernelSem();
	~ListKernelSem();
	void insert(KernelSem*kSem);
	void remove(KernelSem*kSem);

	void timePass(); //za niti blokirane ograniceno na nekom semaforu
};

#endif /* LISTSEM_H_ */
