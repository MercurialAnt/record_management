#ifndef PAGE_O_H
#define PAGE_O_H

#include "MyDB_PageType.h"

// typedef PageOverlay;

class PageOverlay {
	public:
                unsigned int offsetToNextUnwritten;
                MyDB_PageType pageType; 
                char *bytes; /* this is where the data will be */
};

#endif