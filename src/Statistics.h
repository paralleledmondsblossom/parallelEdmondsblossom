#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#ifndef STATISTICS_H
#define STATISTICS_H
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

// Declaration of the template class
template <typename T>
class Statistics {
private:
    std::vector<T> tree;
    std::vector<T> path;
    std::vector<std::chrono::microseconds> time;
    size_t top;

public:
    // Constructor
    Statistics(size_t n);

    // Method to write entry into vectors and increment top
    void write_entry(T tree_val, T path_val, std::chrono::microseconds time_val);

    // Method to write data to a file in xmgrace ready format
    void write_file(const std::string& filename = "");
};

// Definition of the template class constructor
template <typename T>
Statistics<T>::Statistics(size_t n) : top(0) {
    // Resize vectors to size n and initialize to 0
    tree.resize(n, 0);
    path.resize(n, 0);
    time.resize(n, std::chrono::microseconds(0));
}

// Definition of the template class write_entry method
template <typename T>
void Statistics<T>::write_entry(T tree_val, T path_val, std::chrono::microseconds time_val) {
    if (top < tree.size()) {
        tree[top] = tree_val;
        path[top] = path_val;
        time[top] = time_val;
        top++;
    } else {
        std::cerr << "Vectors are full. Cannot add more entries." << std::endl;
    }
}

// Definition of the template class write_file method
template <typename T>
void Statistics<T>::write_file(const std::string& filename) {
    // Use the provided filename or create a default one
    std::string outputFilename = (filename.empty() ? "output.stats" : filename);

    // If an extension is present, drop it and replace with ".stats"
    size_t dotPosition = outputFilename.find_last_of(".");
    if (dotPosition != std::string::npos) {
        outputFilename = outputFilename.substr(0, dotPosition); // drop the extension
    }
    outputFilename += ".stats";

    std::ofstream file(outputFilename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << outputFilename << std::endl;
        return;
    }

    // Write header
    //file << "Sequential\tTree\tPath\tTime\n";

    // Write data
    for (size_t i = 0; i < top; ++i) {
        file << i << "\t" << tree[i] << "\t" << path[i] << "\t" << time[i].count() << "\n";
    }

    file.close();
    std::cout << "Data written to file: " << outputFilename << std::endl;
}
#endif