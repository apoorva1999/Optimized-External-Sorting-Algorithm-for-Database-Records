#include <bits/stdc++.h>
#include "Iterator.h"
#include <fstream>
#include <string.h>

using namespace std;

int main()
{
    int min = 0;
    int max = 100;
    RowCount count = 1000; // Number of rows
    int cols = 10;         // Number of columns per row

    ofstream output_file("database.txt");
    ostream_iterator<string> output_iterator(output_file, " ");

    // Initialize the first row with random values
    vector<int> prevRow(cols);
    for (int j = 0; j < cols; ++j)
    {
        prevRow[j] = rand() % (max - min + 1) + min;
    }
    sort(prevRow.begin(), prevRow.end()); // Sort the first row for lexicographic order

    for (int i = 0; i < count; i++)
    {
        vector<int> currRow = prevRow;

        // Add some randomness to generate the current row
        for (int j = cols - 1; j >= 0; j--)
        {
            int increment = rand() % 3; // Random increment between 0 and 2
            currRow[j] += increment;

            // Ensure values stay within bounds
            // if (currRow[j] > max)
            //     currRow[j] = max;

            // Ensure lexicographic order by resetting values in previous columns
            if (j < cols - 1 && currRow[j] > currRow[j + 1])
                currRow[j] = currRow[j + 1];
        }

        // Write the current row to the file
        for (size_t j = 0; j < currRow.size(); ++j)
        {
            output_file << currRow[j];
            if (j != currRow.size() - 1)
                output_file << " ";
        }

        // Add a newline for all rows except the last
        if (i != count - 1)
            output_file << endl;

        // Update prevRow for the next iteration
        prevRow = currRow;
    }

    return 0;
}
