#include "Iterator.h"
class Disk {
    public:
    static int pidx;
    static void flushPage(string filename, Page &p); //clear the page too
    static Page readPage(string filename, int pidx);
};

