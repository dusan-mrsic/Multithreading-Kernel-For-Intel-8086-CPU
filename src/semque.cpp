// file: semque.cpp

#include "semque.h"
#include <stdio.h>
#include "lock.h"

SemQueue::SemQueue(){
	head = 0;
	tail = 0;
}

SemQueue::Elem::Elem(PCB* pcbb, char waitt){
	this->next = 0;
	this->pcb = pcbb;
	this->WaitFlag = waitt;
}

SemQueue::~SemQueue(){
	Elem* current = 0;
	while(head){
		current=head;
		head = head->next;
		delete current;
	}

}

void SemQueue::insert(PCB *pcbb, char wait) {
	Elem* newElem = new Elem(pcbb,wait);
	if(this->head == 0){
		this->head = newElem;
	}else{
		this->tail->next = newElem;
	}
	this->tail = newElem;
}

PCB* SemQueue::get(PCB *pcbb) {
	Elem* previous = 0;
	Elem* current = head;
	while(current != 0){
		if(current->pcb == pcbb) break;
		previous = current;
		current = current->next;
	}
	if(current == 0) return 0;
	if(previous != 0){
		previous->next = current->next;
	}else{
		this->head = current->next;
	}
	if(current->next == 0){
		this->tail = previous;
	}
    PCB *ret = current->pcb;
    delete current;
    return ret;
}

PCB *SemQueue::get(char *wait) {
    if (head == 0) return 0;
    Elem *first = this->head;
    this->head = this->head->next;
    if (head == 0) tail = 0;
    PCB *ret = first->pcb;
    *wait = first->WaitFlag;
    delete first;
    return ret;
}

int SemQueue::size(){
	int number = 0;
	if(head == 0) return number;
	Elem* temp = head;
	while(temp != 0){
		number++;
		temp = temp->next;
	}
	return number;
}





