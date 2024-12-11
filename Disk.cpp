#include "Disk.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

// Reads a page from a specified file based on the page index.
// Returns a Page object containing up to PAGE_SIZE rows of data.
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
    // Skip rows until reaching the starting idx for the requested page.
    while (currentRow < startRow && std::getline(infile, line)) {
        ++currentRow; 
    }
    // Read rows from the file, parse them into Row objects, and add them to the Page.
    // Stop reading once the Page reaches PAGE_SIZE rows.
    while (p.rows.size() < PAGE_SIZE && std::getline(infile, line)) {
        std::istringstream iss(line);
        Row row;
        std::string value;
        vector<int>values;
        while (std::getline(iss, value, ' ')) {
            values.push_back(std::stoi(value));
        
        }
        // sets the ovc of the row
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

// Writes a Page object to the specified file. Appends to the file if pidx > 0.
// Resets the Page object after writing.
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
    // Write each row in the Page to the file, formatting values with spaces in between.
    // Include the ovc if appendOVC is true.
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
    // Clear the rows in the Page and reset the rowCount to 0 after writing.
    outfile.close();
    p.rows = vector<Row>();
    p.rowCount = 0;
}

// Deletes directories in the specified path that match a given name pattern.
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