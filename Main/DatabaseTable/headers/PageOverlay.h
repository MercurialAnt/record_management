#ifndef PAGE_O_H
#define PAGE_O_H

#include "MyDB_PageType.h"

typedef struct PageBytesStruct PageOverlay;

class PageBytesStruct {
	public:
        unsigned int offsetToNextUnwritten;
        MyDB_PageType pageType; 
        char bytes[0]; /* this is where the data will be */
};

#endif