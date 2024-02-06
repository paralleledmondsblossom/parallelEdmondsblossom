# Parallel Edmonds Blossom README

## Installation

To get started with Parallel Edmonds Blossom, follow these steps:

```bash
git clone https://github.com/paralleledmondsblossom/paralleledmondsblossom.git
cd paralleledmondsblossom
git submodule update --init --recursive
git submodule update --recursive --remote

## Local Compilation
mkdir build
cd build
cmake ..
make

## Docker Compilation
docker build -t benchmarking.docker .
docker run -it benchmarking 

## Run
Usage: ./matcher <file>.mtx <num_iterations> <num_threads> <0 for serial, 1 for parallel>

## Benchmarking
bash ./run_benchmarks.sh

## Input File

Matrix Market files are a standard format for representing sparse matrices. [Learn more](https://networkrepository.com/mtx-matrix-market-format.html).

## Output File

A CSV file is generated with the following columns:

- Input File
- Number of Vertices
- Number of Edges
- Matching Size
- Mean Match Time (seconds)
- Match Time Standard Deviation (seconds)
- Number of Threads
- Number of Iterations
- Execution Mode (serial/parallel)
- Parse Matrix Market File Time (milliseconds)
- Create Compressed Sparse Row Time (milliseconds)

The file is created with headers in the current working directory the executable is called from only once, and then new entries are appended to the existing file.

