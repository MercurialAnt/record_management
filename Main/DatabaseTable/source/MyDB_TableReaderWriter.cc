
#ifndef TABLE_RW_C
#define TABLE_RW_C

#include <fstream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "MyDB_PageReaderWriter.h"
#include "MyDB_TableReaderWriter.h"
#include "MyDB_TableRecIterator.h"
#include "MyDB_PageHandle.h"


class MyDB_Page;
using namespace std;

MyDB_TableReaderWriter :: MyDB_TableReaderWriter (MyDB_TablePtr tablePtr, MyDB_BufferManagerPtr bufferMgr) {
	this->tablePtr = tablePtr;
	this->bufferMgr = bufferMgr;
	this->recordBuffPtr = make_shared <MyDB_Record>(this->tablePtr->getSchema());
	this->numPages = 0;
	if (this->tablePtr->lastPage() < 0) {
		this->tablePtr->setLastPage(0);
	}
}

MyDB_PageReaderWriter MyDB_TableReaderWriter :: operator [] (size_t size) {
	MyDB_PageHandle pageHandle = this->bufferMgr->getPage(tablePtr, size);
	MyDB_PageReaderWriter *pageRW = new MyDB_PageReaderWriter(pageHandle, bufferMgr->getPageSize());

	return *pageRW;

	// return *pageRWs[size];	
}

MyDB_RecordPtr MyDB_TableReaderWriter :: getEmptyRecord () {
	return make_shared <MyDB_Record>(this->tablePtr->getSchema());
}

MyDB_PageReaderWriter MyDB_TableReaderWriter :: last () {
	int lastIdx = this->tablePtr->lastPage(); // the index to the page
	return (*this)[lastIdx];
}

void MyDB_TableReaderWriter :: append (MyDB_RecordPtr recordPtr) {
	if (this->numPages == 0) {
		addPageRW();
	}
	cout << "PageRW BACK: " << pageRWs.back() << "\n";

	if (!(pageRWs.back()->append(recordPtr))) {
		addPageRW();
		append(recordPtr);
	}
	cout << "Appended new record rw \n";

}

void MyDB_TableReaderWriter :: addPageRW () {

	// MyDB_PagePtr newPage = make_shared<MyDB_Page>(this->tablePtr, this->numPages++, *(this->bufferMgr));
	
	MyDB_PageHandle pageHandle = this->bufferMgr->getPage(this->tablePtr, this->tablePtr->lastPage() + 1);
	
	MyDB_PageReaderWriter *pageRW = new MyDB_PageReaderWriter(pageHandle, this->bufferMgr->getPageSize());


	pageRWs.push_back(pageRW); //! DO we still need this vector? not sure , maybe last page suffices
	this->tablePtr->setLastPage(this->numPages - 1);

}

void MyDB_TableReaderWriter :: loadFromTextFile (string text) {
	cout << "Reading \n";

	int fd = open (text.c_str (), O_CREAT | O_RDWR, 0666);
	void *buf = malloc(this->recordBuffPtr->getBinarySize()); 
	int i = 0;
	while (read(fd, buf, this->recordBuffPtr->getBinarySize()) > 0) {
		this->recordBuffPtr->fromBinary(buf);
		append(this->recordBuffPtr);
		cout << "Reading: " << i++ << "\n";
	}
	

}

MyDB_RecordIteratorPtr MyDB_TableReaderWriter :: getIterator (MyDB_RecordPtr recordPtr) {
	return make_shared<MyDB_TableRecIterator>(recordPtr, this);
}

void MyDB_TableReaderWriter :: writeIntoTextFile (string text) {
	// ofstream myFile (fName, ofstream::out | ofstream::trunc);
	// if (myFile.is_open()) {
	// 	for (auto const &ent : myData) {
	// 		myFile << "|" << ent.first << "|" << ent.second << "|\n";
	// 	}
	// }
	ofstream outFile (text, ofstream::out | ofstream::trunc); 

}

#endif

