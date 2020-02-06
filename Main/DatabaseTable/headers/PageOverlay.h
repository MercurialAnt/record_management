#ifndef PAGE_O_H
#define PAGE_O_H

#include "MyDB_PageType.h"
#include "MyDB_PageHandle.h"

class PageOverlay;
typedef struct PageOverlayStruct PageOverlayStruct;
struct PageOverlayStruct
{
    unsigned int offsetToNextUnwritten;
    MyDB_PageType pageType;
    char bytes[0];
};

class PageOverlay {
	public:
                char *getBytes() {
                        return this->pageOverlay->bytes;
                }
                unsigned int getOffset() {
                        return this->pageOverlay->offsetToNextUnwritten;
                }

                void setOffset(unsigned int newOffSet) {
                        this->pageOverlay->offsetToNextUnwritten = newOffSet;
                }

                MyDB_PageType getPageType() {
                        return this->pageOverlay->pageType;
                }

                void setPageType(MyDB_PageType pageType) {
                        this->pageOverlay->pageType = pageType;
                }

                PageOverlay(MyDB_PageHandle pageHandle) {
                        this->pageHandle = pageHandle;
                        this->pageOverlay = (PageOverlayStruct *)this->pageHandle->getBytes();
                        // this->pageOverlay->offsetToNextUnwritten = 0;
                };

                ~PageOverlay() {};


        private:
                MyDB_PageHandle pageHandle;
                PageOverlayStruct *pageOverlay;
              
};

#endif