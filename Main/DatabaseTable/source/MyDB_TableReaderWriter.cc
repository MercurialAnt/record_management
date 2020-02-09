
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
	int lastPage = this->tablePtr->lastPage();

	/* Load the pages in if they were previously there. */
	int i;
	for (i = 0; i <= lastPage; i++) {
		addPageRW(i, true);
	}

}

MyDB_TableReaderWriter :: ~MyDB_TableReaderWriter () {
	// delete pageRWs;
	for (auto page : pageRWs) {
		delete page;
	}
	pageRWs.clear();
	
}


MyDB_PageReaderWriter MyDB_TableReaderWriter :: operator [] (size_t size) {
	// ! somethign wrong here??? 
	return *(pageRWs[size]);


}

MyDB_RecordPtr MyDB_TableReaderWriter :: getEmptyRecord () {
	return make_shared <MyDB_Record>(this->tablePtr->getSchema());
}

MyDB_PageReaderWriter MyDB_TableReaderWriter :: last () {
	return *(pageRWs.back());
}

void MyDB_TableReaderWriter :: append (MyDB_RecordPtr recordPtr) {
	if (this->tablePtr->lastPage() == -1) { // there are no pages in there
		addPageRW(0, false);
	}

	if (!(last().append(recordPtr))) {
		addPageRW(tablePtr->lastPage() + 1, false);
		append(recordPtr);
	}

}

void MyDB_TableReaderWriter :: addPageRW (int pageNum, bool isLoad) {

	// cout << "Table Last Page: " << this->tablePtr->lastPage() << " Table Page Num: " << pageNum << endl;
 	MyDB_PageHandle pageHandle = this->bufferMgr->getPage(this->tablePtr, pageNum);
	MyDB_PageReaderWriter *pageRW = new MyDB_PageReaderWriter(pageHandle, this->bufferMgr->getPageSize(), isLoad);
	pageRWs.push_back(pageRW); 

	/* Set the new last page. */
	if (pageNum > tablePtr->lastPage()) 
		tablePtr->setLastPage(pageNum);

}

void MyDB_TableReaderWriter :: loadFromTextFile (string text) {
	string line;
	ifstream file(text.c_str());
	if (file.is_open()) {
		while (getline(file, line)) {
			this->recordBuffPtr->fromString(line);
			recordBuffPtr->recordContentHasChanged();
			append(this->recordBuffPtr);
		}
		file.close();
	}

}

MyDB_RecordIteratorPtr MyDB_TableReaderWriter :: getIterator (MyDB_RecordPtr recordPtr) {
	return make_shared<MyDB_TableRecIterator>(recordPtr, this);
}

void MyDB_TableReaderWriter :: writeIntoTextFile (string text) {
	ofstream myFile (text, ofstream::out | ofstream::trunc);
	MyDB_RecordIteratorPtr iter = getIterator(recordBuffPtr);
	if (myFile.is_open()) {
		while (iter->hasNext()) {
			iter->getNext();
			myFile << recordBuffPtr << endl;
		}
		myFile.close();
	}

}

#endif

