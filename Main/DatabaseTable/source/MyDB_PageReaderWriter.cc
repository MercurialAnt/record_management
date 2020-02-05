
#ifndef PAGE_RW_C
#define PAGE_RW_C

#include "MyDB_PageReaderWriter.h"
#include "MyDB_PageRecIterator.h"
#include "PageOverlay.h"

void MyDB_PageReaderWriter :: clear () {
	cout << "MyDB_PageReaderWriter clear called\n"; 
	PageOverlay *pageOverLay = (PageOverlay *)bytes;
	pageOverLay->offsetToNextUnwritten = 0;

	this->setType(MyDB_PageType::RegularPage);
}

MyDB_PageType MyDB_PageReaderWriter :: getType () {
	cout << "MyDB_PageReaderWriter getType called\n"; 
	PageOverlay *pageOverLay = (PageOverlay *)bytes;
	return pageOverLay->pageType;
}

MyDB_RecordIteratorPtr MyDB_PageReaderWriter :: getIterator (MyDB_RecordPtr recordPtr) {
	cout << "MyDB_PageReaderWriter getIterator called\n"; 
	return make_shared<MyDB_PageRecIterator>(recordPtr, this);
}

void MyDB_PageReaderWriter :: setType (MyDB_PageType pageType) {
	cout << "MyDB_PageReaderWriter setType called\n"; 
	PageOverlay *pageOverLay = (PageOverlay *)bytes;
	pageOverLay->pageType = pageType;
}

bool MyDB_PageReaderWriter :: append (MyDB_RecordPtr recordPtr) {
	cout << "MyDB_PageReaderWriter append called\n"; 

	PageOverlay *myPage = (PageOverlay *)this->bytes;
	char *nextSlot = myPage->bytes + myPage->offsetToNextUnwritten + recordPtr->getBinarySize();
	char *end = (char *)this->bytes + this->pageSize;
	if (nextSlot > end) {
		return false;
	}

    	void *next = recordPtr->toBinary (&(myPage->bytes[myPage->offsetToNextUnwritten]));
    	myPage->offsetToNextUnwritten = (char *) next - &(myPage->bytes[0]);
	
	return true;
}

MyDB_PageReaderWriter :: MyDB_PageReaderWriter(void *bytes, size_t pageSize) {
	this->bytes = bytes;
	this->pageSize = pageSize;
}


#endif
