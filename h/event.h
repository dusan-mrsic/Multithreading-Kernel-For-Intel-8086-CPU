#ifndef _event_h_
#define _event_h_

typedef unsigned char IVTNo;
class KernelEv;

#define PREPAREENTRY(IVTnum, routineflag) \
void interrupt interruptRoutine##IVTnum(...);\
IVTEntry entry##IVTnum(IVTnum, interruptRoutine##IVTnum);\
void interrupt interruptRoutine##IVTnum(...) {\
    if (routineflag != 0)\
        IVTEntry::oldRoutineCall(IVTnum);\
    IVTEntry::Signal(IVTnum);\
}\


class Event {
public:

	Event (IVTNo ivtNo);

	~Event ();

	void wait();

protected:

	friend class KernelEv;
	friend class IVTEntry;

	void signal(); // can call KernelEv

private:

	KernelEv* myImpl;
};
#endif
