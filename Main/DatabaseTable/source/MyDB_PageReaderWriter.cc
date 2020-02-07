
#ifndef PAGE_RW_C
#define PAGE_RW_C

#include "MyDB_PageReaderWriter.h"
#include "MyDB_PageRecIterator.h"
#include "PageOverlay.h"
#include "MyDB_PageHandle.h"

void MyDB_PageReaderWriter :: clear () {
	// cout << "MyDB_PageReaderWriter clear called\n"; 
	this->pageOverlay->setOffset(0);
	this->setType(MyDB_PageType::RegularPage);
}

MyDB_PageType MyDB_PageReaderWriter :: getType () {
	// cout << "MyDB_PageReaderWriter getType called\n"; 
	return this->pageOverlay->getPageType();
}

MyDB_RecordIteratorPtr MyDB_PageReaderWriter :: getIterator (MyDB_RecordPtr recordPtr) {
	// cout << "MyDB_PageReaderWriter getIterator called\n"; 
	return make_shared<MyDB_PageRecIterator>(recordPtr, this);
}

void MyDB_PageReaderWriter :: setType (MyDB_PageType pageType) {
	// cout << "MyDB_PageReaderWriter setType called\n"; 
	this->pageOverlay->setPageType(MyDB_PageType::RegularPage);
}

bool MyDB_PageReaderWriter :: append (MyDB_RecordPtr recordPtr) {


	char *bytes = pageOverlay->getBytes();
	unsigned int curOffset = pageOverlay->getOffset();
	char *nextSlot = bytes + curOffset + recordPtr->getBinarySize();
	char *end = (char *)pageHandle->getBytes() + this->pageSize; 


	// cout << "bytes: " << (void *)bytes << " curOffset: " << curOffset << endl;
	// cout << "Nextslot: " << (void *)nextSlot << " End: " << (void *)end << endl;


	if (nextSlot > end) {
		cout << "MyDB_PageReaderWriter return false\n"; 
		return false;
	}
	
	recordPtr->recordContentHasChanged();
    	void *next = recordPtr->toBinary (&(bytes[curOffset]));


	pageOverlay->setOffset((char *) next - &(bytes[0]));
	pageHandle->wroteBytes(); //does this matter? lol apparently quite matters
	cout << "MyDB_PageReaderWriter append success\n"; 
	return true;
}

MyDB_PageReaderWriter :: MyDB_PageReaderWriter(MyDB_PageHandle pageHandle, size_t pageSize, bool isLoad) {
	this->pageHandle = pageHandle;
	this->pageSize = pageSize;
	this->pageOverlay = new PageOverlay(this->pageHandle);

	if (!isLoad) //only set offset to 0 if we're not loading old pages
		this->pageOverlay->setOffset(0);
}


#endif
