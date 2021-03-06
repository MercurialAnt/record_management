
#ifndef PAGE_REC_ITER_H
#define PAGE_REC_ITER_H

#include <memory>
#include "MyDB_RecordIterator.h"
#include "MyDB_PageHandle.h"
#include "MyDB_Record.h"
#include "PageOverlay.h"

using namespace std;
class MyDB_PageRecIterator;

typedef shared_ptr <MyDB_PageRecIterator> MyDB_PageRecIteratorPtr;

class MyDB_PageRecIterator : public MyDB_RecordIterator {

public:
        // put the contents of the next record in the file/page into the iterator record
	// this should be called BEFORE the iterator record is first examined
	void getNext () {
                if (hasNext()) {
                        char *bytes = pageOverlay->getBytes();

                        void *next = recordPtr->fromBinary(&(bytes[offsetToNextRec]));
                	recordPtr->recordContentHasChanged();

                        this->offsetToNextRec = (char *) next - &(bytes[0]); 
 

                } else {
			// cout << "MyDB_PageRecIterator: no more rec's left\n"; 
		}
                
        };

	// return true iff there is another record in the file/page
	bool hasNext () {
                return this->offsetToNextRec < pageOverlay->getOffset();

        }

	// destructor and contructor
	MyDB_PageRecIterator (MyDB_RecordPtr recordPtr, MyDB_PageReaderWriter *pageReaderWriter) {
                this->pageReaderWriter = pageReaderWriter;
                this->recordPtr = recordPtr;
                this->offsetToNextRec = 0;
                this->pageOverlay = pageReaderWriter->pageOverlay; //get it from page reader writer
        };

	~MyDB_PageRecIterator () {};


private:
        MyDB_PageReaderWriter *pageReaderWriter;
        MyDB_RecordPtr recordPtr;
        PageOverlay *pageOverlay;
        unsigned int offsetToNextRec;

};
#endif
