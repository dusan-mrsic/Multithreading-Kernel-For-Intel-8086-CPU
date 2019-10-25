#include "signlist.h"
#include<stdlib.h>
#include<stdio.h>

SignalList::SignalList(){
	head=0;
	tail=0;
}

SignalList::~SignalList(){
	Elem* curr = 0;
	while(head != 0){
		curr=head;
		head = head-> next;
		delete curr;
	}
}

void SignalList::add(SignalHandler handler){

	Elem *newNode = new Elem();
	newNode->hand=handler;
	newNode->next = 0;

    if (head != 0)
        tail->next = newNode;
    else
        head = newNode;
    tail = newNode;
}

void SignalList::changePositions(SignalHandler hand1, SignalHandler hand2){
	Elem* tek = head;
	while((tek != 0) || (tek->hand != hand1)){
		tek = tek->next;
	}
	if(tek == 0) return;
	Elem* tek1 = head;
	while((tek1 != 0) || (tek1->hand != hand2)){
		tek1 = tek1->next;
	}
	if(tek1==0) return;
	SignalHandler pom = tek->hand;
	tek->hand = tek1->hand;
	tek1->hand = pom;

}

void SignalList::handlerCopy(SignalList* list){
	for(Elem* tek = list->head ;tek != 0; tek = tek->next){
		this->add(tek->hand);
	}
}


