#include <bits/stdc++.h>
#include <fstream>
#include<iostream>

using namespace std;

int main()
{
    int min = 1;         // Minimum value for the columns
    int max = 100;       // Maximum value for the columns
    int count = 1000;    // Number of rows
    int cols = 10;       // Number of columns per row

    ofstream output_file("table.txt"); 

    vector<int> fixedRow(cols);

    // Generate a fixed row with random values
    for (int j = 0; j < cols; ++j)
    {
        fixedRow[j] = rand() % (max - min + 1) + min;
    }

    // Write the same row multiple times
    for (int i = 0; i < count; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            output_file << fixedRow[j];
            if (j != cols - 1)
                output_file << " ";
        }
        if (i != count - 1)
            output_file << endl;
    }

    output_file.close();

    return 0;
}
