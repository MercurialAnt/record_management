
#ifndef PAGE_REC_ITER_H
#define PAGE_REC_ITER_H

#include <memory>
#include "MyDB_RecordIterator.h"
#include "MyDB_Page.h"
#include "MyDB_Record.h"


using namespace std;

class MyDB_PageRecIterator;


// do we need a shared pointer? 

class MyDB_PageRecIterator : public MyDB_RecordIterator {

public:
        // put the contents of the next record in the file/page into the iterator record
	// this should be called BEFORE the iterator record is first examined
	void getNext () {
                void *memory = this->pagePtr->getBytes(this->pagePtr);

                recordPtr->fromBinary(memory);

        };

	// return true iff there is another record in the file/page
	bool hasNext () {
                return false;
        }

	// destructor and contructor
	MyDB_PageRecIterator (MyDB_RecordPtr recordPtr, MyDB_PagePtr pagePtr) {
                this->pagePtr = pagePtr;
                this->recordPtr = recordPtr;
        };

	~MyDB_PageRecIterator () {};


private:
        MyDB_PagePtr pagePtr;
        MyDB_RecordPtr recordPtr;
};





#endif
