#include<bits/stdc++.h>
#include"Iterator.h"
#include <fstream>
#include<string.h>


int main() {
    int min = 0;
    int max = 100;
    int range = max - min + 1;
    RowCount count = 1000;
    int cols = 10;

    std::ofstream output_file("table.txt");
    std::ostream_iterator<std::string> output_iterator(output_file, " ");


    for(int i=0;i<count;i++) {
        Row row = Row(cols);
        for(int j=0;j<cols; j++) {
            int num = rand() % range + min;
            row.record.push_back(std::to_string(num));

        }
        std::copy(row.record.begin(), row.record.end(), output_iterator);
        if(i!=count-1)
        output_file<<std::endl;
    }   
}
