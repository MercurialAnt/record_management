
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
                // cout << "MyDB_PageRecIterator getNext called\n"; 
                if (hasNext()) {
        //                 void *next = recordPtr->toBinary (&(static_cast<char *>(bytes)[curOffset]));
	// pageOverlay->setOffset((char *) next - &(static_cast<char *>(bytes)[0]));
                        void *bytes = pageOverlay->getBytes();
                        cout << "HasNext bytes: " << bytes << endl;
                        cout << "the bull : " << ((void *)(static_cast<char *>(bytes)) )<< endl;
                        cout << "the bull : " << ((void *)(static_cast<char *>(bytes) + offsetToNextRec) )<< endl;
                        cout << "offset : " << offsetToNextRec << endl;
                        void *next = recordPtr->fromBinary((static_cast<char *>(bytes) + offsetToNextRec));
                        cout << "next " << next << endl;
                        this->offsetToNextRec = (char *) next - &(static_cast<char *>(bytes)[0]);        
                } else {
			cout << "MyDB_PageRecIterator: no more rec's left\n"; 
		}
                
        };

	// return true iff there is another record in the file/page
	bool hasNext () {
                // cout << "MyDB_PageRecIterator hasNext called\n"; 

                char *nextSlot = static_cast<char *>(pageOverlay->getBytes()) + offsetToNextRec + recordPtr->getBinarySize();
                char *end = (char *)pageReaderWriter->pageHandle->getBytes() + pageReaderWriter->pageSize; 

                return nextSlot <= end;
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
