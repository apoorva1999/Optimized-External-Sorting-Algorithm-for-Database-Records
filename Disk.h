#include "Iterator.h"
#include<filesystem>
class Disk {
    public:
    // static int pidx;
    static void flushPage(string filename, Page &p, int &pidx, bool appendOVC); //clear the page too
    static Page readPage(string filename, int pidx);
    static void deleteDirectories(const std::filesystem::path& directoryPath, const std::string& pattern);
};

