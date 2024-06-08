/*
 * listPCB.h
 *
 *  Created on: Aug 16, 2020
 *      Author: OS1
 */

#ifndef LISTPCB_H_
#define LISTPCB_H_

#include"pcb.h"
#include"thread.h"
#include"iostream.h"
#include "System.h"
#include"kerSem.h"

class ListPCB {
private:
	struct Elem{
		PCB* pcb;
		Elem* next;
		Elem(PCB* pcb){
			this->pcb=pcb;
			this->next=0;
		}
	};
	Elem *head, *tail;
	unsigned int size;
protected:
	friend class System;
	friend class PCB;
	friend class KernelSem;
public:
	ListPCB();
	~ListPCB();
	void insert(PCB*pcb);
	PCB* deleteFirst(); //uzima prvi element i brise ga iz liste PCB-ova
	PCB* deletePCB(ID id); //uzima PCB sa zadatim id i brise ga iz liste
	PCB*find(ID id); //pronalazi PCB sa zadatim id ali ga ne brise iz liste
	int getSize();
	int time();
};

#endif /* LISTPCB_H_ */
