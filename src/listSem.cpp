/*
 * listKSem.cpp
 *
 *  Created on: Aug 20, 2020
 *      Author: OS1
 */

#include "listSem.h"
#include"stdio.h"

ListKernelSem::ListKernelSem(){
	head=0;
	tail=0;
	size=0;
}

ListKernelSem::~ListKernelSem(){
	while(head!=0){
		Elem*tmp=head;
		head=head->next;
		delete tmp;
	}
	head=tail=0;
	size=0;
}

void ListKernelSem::insert(KernelSem*kSem){
	Elem*newEl=new Elem(kSem);
	if(head!=0)
		tail->next=newEl;
	else
		head=newEl;
	tail=newEl;
	size++;
}

void ListKernelSem::remove(KernelSem*kSem){
	Elem *curr=head,*prev=0;
	while(curr!=0){
		if(curr->kSem==kSem){
			if(prev==0){
				head=head->next;
				if(head==0)
					tail=0;
			}
			else
				prev->next=curr->next;
			if(prev->next==0)
				tail=prev;
			delete curr;
			size--;
			break;
		}
		prev=curr;
		curr=curr->next;
	}
}

void ListKernelSem::timePass(){
	Elem*tmp=head;
	while(tmp!=0){ //prolazimo kroz listu svih semafora i na svakom od njih prodjemo kroz listu blokiranih niti na ograniceno vreme
		int val=tmp->kSem->waitSignalOrTime->time();
		tmp->kSem->value+=val; //uvecavam vrednost semafora za broj odblokiranih niti istekom max vremena blokiranosti na ovom semaforu
		tmp=tmp->next;
	}
}
