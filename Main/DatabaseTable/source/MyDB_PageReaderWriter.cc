
#ifndef PAGE_RW_C
#define PAGE_RW_C

#include "MyDB_PageReaderWriter.h"
#include "MyDB_PageRecIterator.h"
#include "PageOverlay.h"
#include "MyDB_PageHandle.h"

void MyDB_PageReaderWriter :: clear () {
	cout << "MyDB_PageReaderWriter clear called\n"; 
	this->pageOverlay->setOffset(0);
	this->setType(MyDB_PageType::RegularPage);
}

MyDB_PageType MyDB_PageReaderWriter :: getType () {
	cout << "MyDB_PageReaderWriter getType called\n"; 
	return this->pageOverlay->getPageType();
}

MyDB_RecordIteratorPtr MyDB_PageReaderWriter :: getIterator (MyDB_RecordPtr recordPtr) {
	cout << "MyDB_PageReaderWriter getIterator called\n"; 
	return make_shared<MyDB_PageRecIterator>(recordPtr, this);
}

void MyDB_PageReaderWriter :: setType (MyDB_PageType pageType) {
	cout << "MyDB_PageReaderWriter setType called\n"; 
	this->pageOverlay->setPageType(MyDB_PageType::RegularPage);
}

bool MyDB_PageReaderWriter :: append (MyDB_RecordPtr recordPtr) {
	cout << "MyDB_PageReaderWriter append called\n"; 

	void *bytes = pageOverlay->getBytes();
	unsigned int curOffset = pageOverlay->getOffset();

	cout << "record size : " << recordPtr->getBinarySize() << endl;
	cout << "page size " << pageSize << endl;
	cout << "bytes " << bytes << endl;
	cout << "offset " << curOffset << endl;
	char *nextSlot = static_cast<char *>(bytes) + curOffset + recordPtr->getBinarySize();
	char *end = (char *)(pageOverlay->getPageHandleBytes()) + this->pageSize; //! is this cast correct?
	cout << "nextSlot : " << (void *)nextSlot << endl;
	cout << "end : " << (void *)end << endl;

	if (nextSlot > end) {
		cout << "MyDB_PageReaderWriter return false\n"; 
		return false;
	}

    void *next = recordPtr->toBinary (&(static_cast<char *>(bytes)[curOffset]));
	pageOverlay->setOffset((char *) next - &(static_cast<char *>(bytes)[0]));
	cout << "new PageOverlay offset :" << pageOverlay->getOffset() << endl;
	return true;
}

MyDB_PageReaderWriter :: MyDB_PageReaderWriter(MyDB_PageHandle pageHandle, size_t pageSize) {
	this->pageHandle = pageHandle;
	this->pageSize = pageSize;
	this->pageOverlay = new PageOverlay(this->pageHandle);
}


#endif
