
#ifndef TABLE_REC_ITER_H
#define TABLE_REC_ITER_H

#include <memory>
#include "MyDB_RecordIterator.h"
#include "MyDB_TableReaderWriter.h"
#include "MyDB_PageReaderWriter.h"
#include "MyDB_PageRecIterator.h"

using namespace std;

class MyDB_TableRecIterator;
class MyDB_TableRecIterator : public MyDB_RecordIterator {

public:
	void getNext () {
		// cout << "MyDB_TableRecIterator getNext called\n"; 
		if (hasNext()) {
			if (!(this->pageRecIterator->hasNext())) {
				MyDB_PageReaderWriter pageRW = (*(this->tableReaderWriter))[++count];
				this->pageRecIterator = pageRW.getIterator(this->recordPtr);
			}
			this->pageRecIterator->getNext();
		} else {
			// cout << "MyDB_TableRecIterator: no more rec's left\n"; 
		}
	};

	bool hasNext () {
		if (this->pageRecIterator == nullptr) {
			MyDB_PageReaderWriter pageRW = (*(this->tableReaderWriter))[count];
			this->pageRecIterator = pageRW.getIterator(this->recordPtr);
		}
		bool iterHasNext = this->pageRecIterator->hasNext();

		cout << "IterHasNext :" << iterHasNext << endl;
		bool isLastPage = (count == this->tableReaderWriter->tablePtr->lastPage());
		return iterHasNext || !isLastPage;
	};

	// destructor and contructor
	~MyDB_TableRecIterator () {};

	MyDB_TableRecIterator (MyDB_RecordPtr recordPtr, MyDB_TableReaderWriter *tableReaderWriter) {
		this->tableReaderWriter = tableReaderWriter;
		this->recordPtr = recordPtr;
		count = 0;
		pageRecIterator = nullptr; 
	};
private:
	MyDB_TableReaderWriter *tableReaderWriter;
	MyDB_RecordPtr recordPtr;
	int count;
	MyDB_RecordIteratorPtr pageRecIterator; // ! this is actually a MyDB_PageRecordIterator
};
#endif
