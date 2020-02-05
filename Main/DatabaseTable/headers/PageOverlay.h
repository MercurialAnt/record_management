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
    void *bytes;
};

class PageOverlay {
	public:
                void *getBytes() {
                        return (void *)(static_cast<char *>(pageHandle->getBytes()) + 8);
                }

                void *getPageHandleBytes() {
                        return this->pageHandle->getBytes();
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
                        cout << "size of PageOverlay " << pageOverlay << endl;
                        cout << "size of PageOverlay bytes " << pageOverlay->bytes << endl;
                        
                        this->pageOverlay->offsetToNextUnwritten = 0;
                        this->pageOverlay->bytes = static_cast<char *>(pageHandle->getBytes()) + 8;
                        
                        cout << "size of PageOverlay bytes " << pageOverlay->bytes << endl;

                };

                ~PageOverlay() {};


        private:
                MyDB_PageHandle pageHandle;
                PageOverlayStruct *pageOverlay;
              
};

#endif