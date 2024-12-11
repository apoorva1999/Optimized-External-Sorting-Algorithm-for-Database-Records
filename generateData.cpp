#include <bits/stdc++.h>
#include "Iterator.h"
#include <fstream>
#include <string.h>
#include "iostream"
using namespace std;
int main()
{
    int min = 0;
    int max = 100;
    int range = max - min + 1;
    RowCount count = 1000;
    int cols = 10;
    ofstream output_file("table.txt");
    ostream_iterator<string> output_iterator(output_file, " ");
    for (int i = 0; i < count; i++)
    {
        Row row = Row();
        vector<string> arr;
        for (int j = 0; j < cols; j++)
        {
            int num = rand() % range + min;
            row.record.push_back(num);
            arr.push_back(to_string(num));
        }
        copy(arr.begin(), arr.end(), output_iterator);
        if (i != count - 1)
            output_file << endl;
    }
}
