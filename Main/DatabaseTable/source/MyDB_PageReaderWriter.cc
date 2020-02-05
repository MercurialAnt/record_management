
#ifndef PAGE_RW_C
#define PAGE_RW_C

#include "MyDB_PageReaderWriter.h"
#include "MyDB_PageRecIterator.h"
#include "PageOverlay.h"

void MyDB_PageReaderWriter :: clear () {
	cout << "MyDB_PageReaderWriter clear called\n"; 
	PageOverlay *pageOverLay = (PageOverlay *)pageBytes;
	pageOverLay->offsetToNextUnwritten = 0;

	this->setType(MyDB_PageType::RegularPage);
}

MyDB_PageType MyDB_PageReaderWriter :: getType () {
	cout << "MyDB_PageReaderWriter getType called\n"; 
	PageOverlay *pageOverLay = (PageOverlay *)pageBytes;
	return pageOverLay->pageType;
}

MyDB_RecordIteratorPtr MyDB_PageReaderWriter :: getIterator (MyDB_RecordPtr recordPtr) {
	cout << "MyDB_PageReaderWriter getIterator called\n"; 
	return make_shared<MyDB_PageRecIterator>(recordPtr, this);
}

void MyDB_PageReaderWriter :: setType (MyDB_PageType pageType) {
	cout << "MyDB_PageReaderWriter setType called\n"; 
	PageOverlay *pageOverLay = (PageOverlay *)pageBytes;
	pageOverLay->pageType = pageType;
}

bool MyDB_PageReaderWriter :: append (MyDB_RecordPtr recordPtr) {
	cout << "MyDB_PageReaderWriter append called\n"; 
	cout << "Bytes: " <<  this->pageBytes  << "\n";

	PageOverlay *myPage = (PageOverlay *)this->pageBytes;
	char *nextSlot = myPage->bytes + myPage->offsetToNextUnwritten + recordPtr->getBinarySize();
	
	char *end = ((char *)this->pageBytes) + this->pageSize;

	cout << "Bytes: " <<  ((char *)this->pageBytes)  << "\n";

	cout << "Mypage Bytes: " <<  myPage->bytes << "\n";
	cout << "Off: " << myPage->offsetToNextUnwritten << "\n";
	cout << "Record: " <<  recordPtr->getBinarySize() << "\n";
	cout << "Bytes: " <<  (char *)this->pageBytes  << "\n";
	cout << "PageSize: " <<  this->pageSize  << "\n";

	cout << "NextSlot: " << nextSlot << "\n";
	cout << "End: " << end << "\n";

	if (nextSlot > end) {
		cout << "MyDB_PageReaderWriter return false\n"; 
		return false;
	}

    	void *next = recordPtr->toBinary (&(myPage->bytes[myPage->offsetToNextUnwritten]));
    	myPage->offsetToNextUnwritten = (char *) next - &(myPage->bytes[0]);
	
	return true;
}

MyDB_PageReaderWriter :: MyDB_PageReaderWriter(void *pageBytes, size_t pageSize) {
	this->pageBytes = pageBytes;
	this->pageSize = pageSize;

	PageOverlay *myPage = (PageOverlay *)this->pageBytes;
	myPage->offsetToNextUnwritten = 0;
	myPage->pageType = MyDB_PageType :: RegularPage;
	myPage->bytes = (char *)this->pageBytes + 8;

}


#endif
