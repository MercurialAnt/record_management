
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
			while (!(this->pageRecIterator->hasNext())) {
				MyDB_PageReaderWriter pageRW = (*(this->tableReaderWriter))[count++];
				this->pageRecIterator = pageRW.getIterator(this->recordPtr);
			}
			this->pageRecIterator->getNext();
		} else {
			// cout << "MyDB_TableRecIterator: no more rec's left\n"; 
		}
	};

	bool hasNext () {
		if (this->pageRecIterator == nullptr) {
			MyDB_PageReaderWriter pageRW = (*(this->tableReaderWriter))[count++]; // count is 0
			this->pageRecIterator = pageRW.getIterator(this->recordPtr);
		}

		if (this->pageRecIterator->hasNext()) {
			return true;
		}

		/* Need to use a while loop to check every page in case of clears */
		int temp = count;
		int lastPage = tableReaderWriter->tablePtr->lastPage() + 1;
		while (temp < lastPage) {
			MyDB_PageReaderWriter pageRW = (*(this->tableReaderWriter))[temp];
			MyDB_RecordIteratorPtr iter = pageRW.getIterator(this->recordPtr);
			if (iter->hasNext()) {// next page exists
				return true;
			}
			temp++;
		}
		return false;

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
	MyDB_RecordIteratorPtr pageRecIterator; // ! this is actually a MyDB_PageRecordIterator
	MyDB_RecordPtr recordPtr;
	int count;
};
#endif
