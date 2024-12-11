#include <bits/stdc++.h>
#include "Iterator.h"
#include <fstream>
#include <string.h>
#include "iostream"
using namespace std;
int main(int argc, char * argv [])
{
    RowCount count = 1000;

    if(argc > 1) {
		count = stoi(argv[1]);
	} 
    int min = 0;
    int max = 100;
    int range = max - min + 1;
    int cols = 10;

    // Seed the random number generator
    srand(static_cast<unsigned>(time(0)));

    ofstream output_file("database.txt");
    ostream_iterator<string> output_iterator(output_file, " ");

     // Generate rows of random data
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
