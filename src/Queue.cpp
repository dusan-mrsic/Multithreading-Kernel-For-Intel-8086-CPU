#include "Queue.h"

List::List(){
	head = 0;
	tail = 0;
}

List::Elem::Elem(PCB* pcbb){
	this->pcb=pcbb;
	this->next = 0;
}

List::~List(){
	Elem* tek = 0;
	while(head != 0){
		tek=head;
		head = head-> next;
		delete tek;
	}

}

void List::insertElem(PCB* pcb){
	Elem* newElem = new Elem(pcb);
	if(this->head == 0){
		this->head = newElem;
	}else{
		this->tail->next = newElem;
	}
	this->tail = newElem;
}

void List::removeElem(PCB* pcbb){
	Elem* previous = 0;
	Elem* current = head;
	while(current != 0){
		if(current->pcb == pcbb) break;
		previous = current;
		current = current->next;
	}
	if(current == 0) return;
	if(previous != 0){
		previous->next = current->next;
	}else{
		this->head = current->next;
	}
	if(current->next == 0){
		this->tail = previous;
	}
}

int List::size(){
	int number = 0;
	if(head == 0) return number;
	Elem* temp = head;
	while(temp != 0){
		number++;
		temp = temp->next;
	}
	return number;
}

PCB* List::getElem(){
	if(head == 0) return 0;
	Elem* temp = this->head;
	this->head = this->head->next;
	PCB* result = temp->pcb;
	if(this->head == 0) this->tail = 0;
	delete temp;
	return result;

}
