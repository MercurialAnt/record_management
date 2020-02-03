
#ifndef PAGE_RW_C
#define PAGE_RW_C

#include "MyDB_PageReaderWriter.h"
#include "MyDB_PageRecIterator.h"

void MyDB_PageReaderWriter :: clear () {
	// Need to clear

	this->setType(RegularPage);
}

MyDB_PageType MyDB_PageReaderWriter :: getType () {
	PageOverlay *pageOverLay = (PageOverlay *)pageHandle->getBytes();
	return pageOverLay->pageType;
}

MyDB_RecordIteratorPtr MyDB_PageReaderWriter :: getIterator (MyDB_RecordPtr recordPtr) {
	return make_shared<MyDB_PageRecIterator>(recordPtr, this->pageHandle);
}

void MyDB_PageReaderWriter :: setType (MyDB_PageType pageType) {
	PageOverlay *pageOverLay = (PageOverlay *)pageHandle->getBytes();
	pageOverLay->pageType = pageType;
}

bool MyDB_PageReaderWriter :: append (MyDB_RecordPtr recordPtr) {
	
	PageOverlay *myPage = (PageOverlay *)this->pageHandle->getBytes();
	char *nextSlot = myPage->bytes + myPage->offsetToNextUnwritten + recordPtr->getBinarySize();
	char *end = (char *)this->pageHandle->getBytes() + this->pageSize;
	if (nextSlot > end) {
		return false;
	}

    	void *next = recordPtr->toBinary (&(myPage->bytes[myPage->offsetToNextUnwritten]));
    	myPage->offsetToNextUnwritten = (char *) next - &(myPage->bytes[0]);
	
	return true;
}

MyDB_PageReaderWriter :: MyDB_PageReaderWriter(MyDB_PageHandle pageHandle, size_t pageSize) {
	this->pageHandle = pageHandle;
	this->pageSize = pageSize;
}


#endif
