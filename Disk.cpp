#include "Disk.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

// int Disk::pidx = 0; // Definition and initialization
Page Disk::readPage(string filename, int pidx) {
    ifstream infile(filename);
    Page p;
    if (!infile.is_open()) {
        std::cerr << "Error: Could not open file "<<filename<<" for reading"<<endl;
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
        vector<int>values;
        while (std::getline(iss, value, ' ')) {
            values.push_back(std::stoi(value));
        
        }
        if(values.size() > ROW_SIZE) {
            row.ovc = values.back();
            row.record.assign(values.begin(), values.end() - 1);
        }
        else {
            row.record = values;
        }
        p.rows.push_back(row);
    }

    infile.close();
    return p;
}

void Disk::flushPage(string filename, Page &p, int &pidx, bool appendOVC) {
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
    for (Row &row : p.rows) {
        if(appendOVC)
        row.record.push_back(row.ovc);
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


void Disk::deleteDirectories(const fs::path& directoryPath, const std::string& pattern) {
         try {
        // Iterate over all entries in the directory
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            // Check if the entry is a directory
            if (fs::is_directory(entry)) {
                // Get the directory name
                const std::string dirName = entry.path().filename().string();
                
                // Check if the directory name matches the pattern
                if (dirName.find(pattern) == 0) { // Name starts with "pass_"
                    std::cout << "Deleting directory: " << entry.path() << std::endl;
                    fs::remove_all(entry); // Recursively delete the directory
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}