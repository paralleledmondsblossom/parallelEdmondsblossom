// Copyright (C) 2023 Adam Lugowski. All rights reserved.
// Use of this source code is governed by the BSD 2-clause license found in the LICENSE.txt file.
// SPDX-License-Identifier: BSD-2-Clause
//#undef NDEBUG
#include <cassert>    // for assertions
#include "FileReader.h"
#include "Graph.h"
#include "Matcher.h"
#include <chrono>
using namespace std::chrono;
#include <list>
#include <cxxopts.hpp>
#include <limits>
typedef int32_t INDEX_TYPE;

int main(int argc, char **argv) {
    cxxopts::Options options("matcher", "A multi-threaded DFS-based Edmonds Blossom solver.");
    options.add_options()
        ("file", "Input file", cxxopts::value<std::string>())
        ("iterations", "Number of iterations", cxxopts::value<int>()->default_value("1"))
        ("threads", "Number of threads", cxxopts::value<int>()->default_value("1"))
        ("execution", "Execution mode (0 for serial, 1 for parallel)", cxxopts::value<int>()->default_value("0"))  // Updated to accept an integer
        ("deferral_threshold", "Stack depth at which deferral is initiated. Default: 'INF' for infinity.", cxxopts::value<int>()->default_value(std::to_string(std::numeric_limits<int>::max())));  // Updated to accept an integer

    options.parse_positional({"file", "iterations", "threads", "execution", "deferral_threshold"});
    auto result = options.parse(argc, argv);

    // Check if 'threads' is a positive integer
    if (result["threads"].as<int>() <= 0) {
        std::cerr << "Error: 'threads' must be a positive integer." << std::endl;
        return 1;
    }

    // Check if 'execution' is 0 or 1
    int executionValue = result["execution"].as<int>();
    if (executionValue != 0 && executionValue != 1) {
        std::cerr << "Error: 'execution' must be 0 or 1." << std::endl;
        return 1;
    }

    // Check if 'deferral_threshold' is a positive integer
    int deferral_threshold = result["deferral_threshold"].as<int>();
    if (deferral_threshold < 0) {
        std::cerr << "Error: 'deferral_threshold' must be a non-negative integer." << std::endl;
        return 1;
    }

    if (!result.count("file")) {
        std::cout << "Usage: " << argv[0] << " <file>.mtx <num_iterations> <num_threads> <0 for serial, 1 for parallel> <deferral_threshold>" << '\n';
        return 0;
    } else {
        std::cout << "READING " << result["file"].as<std::string>() << '\n';
        std::cout << "Number of iterations: " << result["iterations"].as<int>() << '\n';
        std::cout << "Number of threads: " << result["threads"].as<int>() << '\n';
        std::cout << "Execution mode: " << result["execution"].as<int>() << " (0 for serial, 1 for parallel)" << '\n';

        // Check if the deferral_threshold is set by the user
        deferral_threshold = result["deferral_threshold"].as<int>();
        if (deferral_threshold == std::numeric_limits<int>::max()) {
            std::cout << "Deferral threshold: INF (do not defer)" << '\n';
        } else {
            std::cout << "Deferral threshold: " << deferral_threshold << '\n';
        }
    }

    std::filesystem::path in_path{result["file"].as<std::string>()};
    FileReader<INDEX_TYPE, std::string>  FR(in_path);
    //Graph<int64_t, std::string>  G(in_path);
    // A map is used for the frontier to limit copying N vertices.
    //std::unordered_map<int64_t, Vertex<int64_t>> vertexMap;
    // A vector is used for the frontier to allocate once all the memory ever needed.
    auto allocate_start = high_resolution_clock::now();
    Graph<INDEX_TYPE, std::string>  G(FR.indptr,FR.indices,FR.original_rows,
    FR.original_cols,FR.original_vals,FR.N,FR.M);
    auto allocate_end = high_resolution_clock::now();
    auto duration_alloc = duration_cast<milliseconds>(allocate_end - allocate_start);
    std::cout << "Matching (|V|) memory allocation time: "<< duration_alloc.count() << " milliseconds" << '\n';
    // Assign all elements in the vector to false
    // Read the number of iterations and number of threads from cxxopts results
    int num_iters = result["iterations"].as<int>();
    int num_threads = result["threads"].as<int>();
    int execution = result["execution"].as<int>();
    std::vector<double> matching_times; // To store matching times for each iteration

    for (int i = 0;i<num_iters;++i){
        for (auto& atomicBool : G.matching) {
            atomicBool.store(-1);
        }
        auto match_start = high_resolution_clock::now();
        if (execution){
            Matcher::match_wl3<INDEX_TYPE, std::string>(G,num_threads,deferral_threshold);
        } else {
            Matcher::match<INDEX_TYPE, std::string>(G);
        }
        auto match_end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(match_end - match_start);
        std::cout << "Maximum matching time: "<< (1.0*duration.count())/1000.0 << " seconds" << '\n';
        auto count = std::count_if(G.matching.begin(), G.matching.end(),[&](auto const& val){ return val > -1; });
        std::cout << "Maximum matching size: "<<  count/2 << '\n';
        matching_times.push_back(duration.count());
        //Statistics<INDEX_TYPE> stats(G.getN());
        //stats.write_file(argv[1]);
    }
    // Calculate mean and standard deviation
    double mean = std::accumulate(matching_times.begin(), matching_times.end(), 0.0) / matching_times.size();
    double stdev = 0.0;
    for (const auto& time : matching_times) {
        stdev += std::pow(time - mean, 2);
    }
    stdev = std::sqrt(stdev / matching_times.size());

    // Print mean and standard deviation
    std::cout << "Mean matching time: " << mean/1000.0 << " seconds" << '\n';
    std::cout << "Standard deviation: " << stdev/1000.0 << " seconds" << '\n';

    //Matcher::match_wl<int64_t, std::string>(G,stats);
    std::vector<INDEX_TYPE> match_count(G.getM(),0);
    // Iterate through the matching vector and update the match_count array
    for (auto const& val : G.matching) {
        if (val > -1 && static_cast<size_t>(val) < match_count.size()) {
            // Increment the count at the specified index
            match_count[val]++;
        }
    }
    // Check if each value in match_count is either 0 or 2
    for (size_t i = 0; i < match_count.size(); ++i) {
        if (match_count[i] != 0 && match_count[i] != 2) {
            throw std::runtime_error("Error: Match count is not equal to 0 or 2");
        }
    }
    std::cout << "Maximum matching is valid." << '\n';

    // Write to CSV file
    std::ofstream csvFile("results.csv", std::ios::app);  // Open the CSV file in append mode

    if (csvFile.is_open()) {
        // Check if the file is empty, if so, add a header line
        if (csvFile.tellp() == 0) {
            csvFile << "File,V,E,M,MeanMatchingTime,Stdev,Threads,Iterations,ExecutionMode,ParseGraphTime,CreateCSRTime\n";
        }
        auto count = std::count_if(G.matching.begin(), G.matching.end(), [&](auto const& val) { return val > -1; });
        // Extract the basename of the file from the path
        std::filesystem::path filePath(result["file"].as<std::string>());
        std::string fileBasename = filePath.stem().string();

        // Append values to the CSV file
        csvFile << fileBasename << ","
                << G.getN() << ","
                << G.getM() << ","
                << count / 2 << ","
                << mean/1000.0 << ","
                << stdev/1000.0 << ","
                << num_threads << ","
                << num_iters << ","
                << execution << ","
                << FR.parse_graph_duration.count() << ","
                << FR.create_csr_duration.count() << std::endl;
        csvFile.close();

    } else {
        std::cerr << "Error opening the CSV file." << std::endl;
    }

    return 0;
}
