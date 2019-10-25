#include "sleeplis.h"
#include "kernsem.h"
#include "SCHEDULE.H"

SleepList::SleepList(){
	head = 0;
	tail = 0;
}

SleepList::~SleepList(){

	SleepList::Elem *tek = head;
	while(head){
		head = head->next;
		delete tek;
	}

}

void SleepList::addNode(KernelSem *newsem, PCB *newpcb, Time timeToWait){

	SleepList::Elem* node = new Elem();
	node->next = 0;
	node->pcb = newpcb;
	node->sem = newsem;
	node->preostaloVreme = 0;

	SleepList::Elem *prev = 0;
	SleepList::Elem *current = KernelSem::sleepList.head;

	for(; current != 0,current->preostaloVreme <= timeToWait; current = current->next){
		timeToWait -= current->preostaloVreme;
		prev = current;
	}

	node->preostaloVreme = timeToWait;
	node->next = current;
	if (prev != 0) { prev->next = node; }
	else { KernelSem::sleepList.head = node; }
	if (current != 0) { current->preostaloVreme = current->preostaloVreme - timeToWait; }

}

void SleepList::removeNode(PCB* pcb){

    SleepList::Elem *prev = 0;
    SleepList::Elem *current = KernelSem::sleepList.head;
    while(current != 0){
    	if(current->pcb == pcb){
    		break;
    	}
    	prev = current;
    	current = current->next;
    }
    if (current == 0) { return; }
    if (current->next)
        current->next->preostaloVreme = current->next->preostaloVreme + current->preostaloVreme;
    if (prev != 0){ prev->next = current->next; }
    else{ KernelSem::sleepList.head = current->next; }
    delete current;

}

void SleepList::updateSleepList() {
	 if (KernelSem::sleepList.head == 0) { return; }
	 SleepList::Elem *temp;
	 KernelSem::sleepList.head->preostaloVreme = KernelSem::sleepList.head->preostaloVreme - 1;
	 int uslov = 1;
	 while (KernelSem::sleepList.head && uslov){
		 if(KernelSem::sleepList.head->preostaloVreme == 0){
			 temp = KernelSem::sleepList.head;
			 KernelSem::sleepList.head = KernelSem::sleepList.head->next;
		     PCB *pcb = temp->sem->blocked->get(temp->pcb);
		     temp->sem->value = temp->sem->value + 1;
		     pcb->state = PCB::READY;
		     pcb->semStatus = 0;
		     Scheduler::put(pcb);
		     delete temp;
		 }else
			 uslov = 0;
	 }
}




