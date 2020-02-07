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
                        return ((PageOverlayStruct *)this->pageHandle->getBytes())->bytes;
                }
                unsigned int getOffset() {
                        return ((PageOverlayStruct *)this->pageHandle->getBytes())->offsetToNextUnwritten;
                }

                void setOffset(unsigned int newOffSet) {
                        ((PageOverlayStruct *)this->pageHandle->getBytes())->offsetToNextUnwritten = newOffSet;
                }

                MyDB_PageType getPageType() {
                        return ((PageOverlayStruct *)this->pageHandle->getBytes())->pageType;
                }

                void setPageType(MyDB_PageType pageType) {
                        ((PageOverlayStruct *)this->pageHandle->getBytes())->pageType = pageType;
                }

                PageOverlay(MyDB_PageHandle pageHandle) {
                        this->pageHandle = pageHandle;
                };

                ~PageOverlay() {};


        private:
                MyDB_PageHandle pageHandle;
                PageOverlayStruct *pageOverlay;
              
};

#endif