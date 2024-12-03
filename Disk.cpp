#include "Disk.h"
#include <iostream>
#include <fstream>
#include <sstream>
// int Disk::pidx = 0; // Definition and initialization

void Disk::flushPage(string filename, Page &p, int &pidx) {
    ofstream outfile;
    if (pidx == 0)
        outfile.open(filename);
    else
        outfile.open(filename, std::ios::app);
    pidx++;
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }
    for (const Row &row : p.rows) {
        std::ostringstream oss;
        for (size_t i = 0; i < row.record.size(); ++i)
        {
            oss << row.record[i];
            if (i < row.record.size() - 1)
            {
                oss << " "; 
            }
        }
        outfile << oss.str() << "\n"; 
    }

    outfile.close();
    p.rows = vector<Row>();
    p.rowCount = 0;
}


Page Disk::readPage(string filename, int pidx) {
    ifstream infile(filename);
    Page p;
    if (!infile.is_open()) {
        std::cerr << "Error: Could not open file initial_run for reading.\n";
        throw exception();
    }

    std::string line;
    int startRow = pidx * PAGE_SIZE;
    int currentRow = 0;
    while (currentRow < startRow && std::getline(infile, line)) {
        ++currentRow; 
    }
    while (p.rows.size() < PAGE_SIZE && std::getline(infile, line)) {
        std::istringstream iss(line);
        Row row;
        std::string value;

        while (std::getline(iss, value, ' ')) {
            row.record.push_back(std::stoi(value));
        }
        p.rows.push_back(row);
    }
    infile.close();
    return p;
}