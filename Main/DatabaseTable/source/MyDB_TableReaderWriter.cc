
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

	int lastPage = this->tablePtr->lastPage();

	int i;
	for (i = 0; i <= lastPage; i++) {
		addPageRW(i, true);
	}

}

MyDB_PageReaderWriter MyDB_TableReaderWriter :: operator [] (size_t size) {

	// ! somethign wrong here
	// int curLastPage = tablePtr->lastPage();
	// if (size <= curLastPage) {
	// 	return *(pageRWs[size]);	
	// }

	// int i;
	// for (i = curLastPage + 1; i <= size; i++) {
	// 	addPageRW(i, false);
	// }
	// cout << "operator : " << size << "lastpage: " << tablePtr->lastPage() << endl;
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
		last().append(recordPtr);
	}

}

void MyDB_TableReaderWriter :: addPageRW (int pageNum, bool isLoad) {

	// cout << "Table Last Page: " << this->tablePtr->lastPage() << " Table Page Num: " << pageNum << endl;

 	MyDB_PageHandle pageHandle = this->bufferMgr->getPage(this->tablePtr, pageNum);
	MyDB_PageReaderWriter *pageRW = new MyDB_PageReaderWriter(pageHandle, this->bufferMgr->getPageSize(), isLoad);
	pageRWs.push_back(pageRW); 
	if (pageNum > tablePtr->lastPage()) 
		tablePtr->setLastPage(pageNum);

}

void MyDB_TableReaderWriter :: loadFromTextFile (string text) {
	cout << "Reading \n";

	ifstream file(text.c_str());
	if (file.is_open()) {
	string line;
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
	// ofstream myFile (fName, ofstream::out | ofstream::trunc);
	// if (myFile.is_open()) {
	// 	for (auto const &ent : myData) {
	// 		myFile << "|" << ent.first << "|" << ent.second << "|\n";
	// 	}
	// }
	ofstream outFile (text, ofstream::out | ofstream::trunc); 

}

#endif

