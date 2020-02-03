
#ifndef PAGE_REC_ITER_H
#define PAGE_REC_ITER_H

#include <memory>
#include "MyDB_RecordIterator.h"
#include "MyDB_PageHandle.h"
#include "MyDB_Record.h"
#include "PageOverlay.h"

using namespace std;

class MyDB_PageRecIterator;
class MyDB_PageRecIterator : public MyDB_RecordIterator {

public:
        // put the contents of the next record in the file/page into the iterator record
	// this should be called BEFORE the iterator record is first examined
	void getNext () {
                if (hasNext()) {
                        PageOverlay *myPage = (PageOverlay *)this->pageHandle->getBytes();
                        void *next = recordPtr->fromBinary(&(myPage->bytes[this->offsetToNextRec]));
                        this->offsetToNextRec = (char *) next - &(myPage->bytes[0]);        
                }
                
        };

	// return true iff there is another record in the file/page
	bool hasNext () {
                PageOverlay *myPage = (PageOverlay *)this->pageHandle->getBytes();
                char *nextSlot = myPage->bytes + offsetToNextRec + recordPtr->getBinarySize();
                char *end = (char *)myPage + this->pageSize;
                return nextSlot <= end;
        }

	// destructor and contructor
	MyDB_PageRecIterator (MyDB_RecordPtr recordPtr, MyDB_PageHandle pageHandle) {
                this->pageHandle = pageHandle;
                this->recordPtr = recordPtr;
                this->offsetToNextRec = 0;
        };

	~MyDB_PageRecIterator () {};


private:
        MyDB_PageHandle pageHandle;
        MyDB_RecordPtr recordPtr;
        unsigned int offsetToNextRec;
};
#endif
