#include"Disk.h"
#include <iostream>
#include <fstream>
#include <sstream>
int Disk::pidx = 0; // Definition and initialization

void Disk::flushPage(string filename, Page &p) {
    std::ofstream outfile;

    if(pidx == 0)
    outfile.open(filename);
    else 
    outfile.open(filename, std::ios::app);
    pidx++;
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }
     // Write each row's record to the file
    for (const Row &row : p.rows) {
        std::ostringstream oss;
        for (size_t i = 0; i < row.record.size(); ++i) {
            oss << row.record[i];
            if (i < row.record.size() - 1) {
                oss << " "; // Add comma separator between record values
            }
        }
        outfile << oss.str() << "\n"; // Write row to file and add a newline
    }

    outfile.close();
    p.rows = vector<Row>();
    p.rowCount = 0;
    std::cout << "Page flushed to file: " << filename << std::endl;

} 
Page Disk::readPage(string filename, int pidx) {
    Page p;
    return p;
}