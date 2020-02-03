
#ifndef TABLE_RW_C
#define TABLE_RW_C

#include <fstream>
#include "MyDB_PageReaderWriter.h"
#include "MyDB_TableReaderWriter.h"

using namespace std;

MyDB_TableReaderWriter :: MyDB_TableReaderWriter (MyDB_TablePtr tablePtr, MyDB_BufferManagerPtr bufferMgr) {
	this->tablePtr = tablePtr;
	this->bufferMgr = bufferMgr;
}

MyDB_PageReaderWriter MyDB_TableReaderWriter :: operator [] (size_t size) {


	MyDB_PageReaderWriter temp;
	return temp;	
}

MyDB_RecordPtr MyDB_TableReaderWriter :: getEmptyRecord () {
	return nullptr;
}

MyDB_PageReaderWriter MyDB_TableReaderWriter :: last () {
	MyDB_PageReaderWriter temp;
	int lastIdx = this->tablePtr->lastPage(); // the index to the page
	return (*this)[lastIdx];
}

void MyDB_TableReaderWriter :: append (MyDB_RecordPtr recordPtr) {
}

void MyDB_TableReaderWriter :: loadFromTextFile (string text) {
	//1. Open this text file 
	//2. Read it in chunks of pages 
	//3. 

}

MyDB_RecordIteratorPtr MyDB_TableReaderWriter :: getIterator (MyDB_RecordPtr recordPtr) {
	return nullptr;
}

void MyDB_TableReaderWriter :: writeIntoTextFile (string text) {
}

#endif

