/*
 * listPCB.cpp
 *
 *  Created on: Aug 16, 2020
 *      Author: OS1
 */

#include "listPCB.h"
#include"stdio.h"
#include"thread.h"
#include"SCHEDULE.H"

ListPCB::ListPCB(){
	head=0;
	tail=0;
	size=0;
}

ListPCB::~ListPCB(){
	while(head!=0){
		Elem*tmp=head;
		head=head->next;
		delete tmp;
	}
	head=tail=0;
	size=0;
}

void ListPCB::insert(PCB*pcb){
	Elem*newEl=new Elem(pcb);
	if(head!=0)
		tail->next=newEl;
	else
		head=newEl;
	tail=newEl;
	size++;
}

PCB* ListPCB::deleteFirst(){
	PCB*ret=0;
	if(head!=0){
		Elem*old=head;
		head=head->next;
		size--;
		if(head==0)
			tail=0;
		ret=old->pcb;
		delete old;
	}
	return ret;
}

PCB* ListPCB::deletePCB(ID id){
	PCB*ret=0;
	Elem *curr=head,*prev=0;
	while(curr!=0){
		if((curr->pcb!=0) && (curr->pcb->id==id)){
			if(prev==0){
				head=head->next;
				if(head==0)
					tail=0;
			}
			else
				prev->next=curr->next;
			if(prev->next==0)
				tail=prev;
			ret=curr->pcb;
			delete curr;
			size--;
			break;
		}
	prev=curr;
	curr=curr->next;
	}
	return ret;
}

PCB* ListPCB::find(ID id){
	Elem*curr=head;
	while(curr!=0){
		if((curr->pcb!=0) && curr->pcb->id==id){
			return curr->pcb;
		}
		curr=curr->next;
	}
	return 0;
}

int ListPCB::time(){//vraca broj odblokiranih niti
	Elem*curr=head;
	int ret=0;
	while(curr!=0){
		Elem*next=curr->next;
		if(curr->pcb->timeWaitOnSem>0)//umanjujemo maksimalno vreme cekanja niti
			curr->pcb->timeWaitOnSem--;
		if(curr->pcb->timeWaitOnSem==0){//nit se odblokirala istekom maksimalnig vremena cekanja na semaforu
			//printf("Postavljam slag time na 1 nit id=%d\n",curr->pcb->id);
			curr->pcb->flagTime=1; //postavljamo fleg koji govori da je nit odblokirana istekom vremena
			PCB* tmpPCB=this->deletePCB(curr->pcb->id);
			if(tmpPCB!=0 && tmpPCB->state!=FINISHED){
				//printf("Odblokirana je nit id=%d istekom max vremena blokiranosti\n",tmpPCB->id);
				tmpPCB->state=READY;
				Scheduler::put(tmpPCB);
				ret++;
			}
		}
		curr=next;
	}
	return ret;
}

int ListPCB::getSize(){
	return this->size;
}
