
#ifndef TABLE_RW_C
#define TABLE_RW_C

#include <fstream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "MyDB_PageReaderWriter.h"
#include "MyDB_TableReaderWriter.h"
#include "MyDB_TableRecIterator.h"


class MyDB_Page;
using namespace std;

MyDB_TableReaderWriter :: MyDB_TableReaderWriter (MyDB_TablePtr tablePtr, MyDB_BufferManagerPtr bufferMgr) {
	this->tablePtr = tablePtr;
	this->bufferMgr = bufferMgr;
	this->recordBuffPtr = make_shared <MyDB_Record>(this->tablePtr->getSchema());
	this->numPages = 0;
}

MyDB_PageReaderWriter MyDB_TableReaderWriter :: operator [] (size_t size) {
	return *pageRWs[size];	
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
	
	if (!pageRWs.back()->append(recordPtr)) {
		addPageRW();
		append(recordPtr);
	}

}

void MyDB_TableReaderWriter :: addPageRW () {
	MyDB_PagePtr newPage = make_shared<MyDB_Page>(this->tablePtr, this->numPages++, *(this->bufferMgr));
	MyDB_PageReaderWriter *pageRW = new MyDB_PageReaderWriter(newPage->getBytes(newPage), this->bufferMgr->getPageSize());
	pageRWs.push_back(pageRW);
	this->tablePtr->setLastPage(this->numPages - 1);
}

void MyDB_TableReaderWriter :: loadFromTextFile (string text) {
	int fd = open (text.c_str (), O_CREAT | O_RDWR, 0666);
	void *buf = malloc(this->recordBuffPtr->getBinarySize()); 
	while (read(fd, buf, this->recordBuffPtr->getBinarySize()) > 0) {
		this->recordBuffPtr->fromBinary(buf);
		append(this->recordBuffPtr);
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

