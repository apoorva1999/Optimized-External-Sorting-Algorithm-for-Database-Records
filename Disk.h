#include "Iterator.h"
class Disk {
    public:
    // static int pidx;
    static void flushPage(string filename, Page &p, int &pidx); //clear the page too
    static Page readPage(string filename, int pidx);
};

