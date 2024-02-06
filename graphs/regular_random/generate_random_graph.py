import sys
import networkx as nx

if len(sys.argv) < 4 or len(sys.argv) > 5:
    print("Usage: python generate_random_graph.py <num_vertices> <degree> <output_file> [matrix_market]")
    sys.exit(1)

num_vertices = int(sys.argv[1])
degree = int(sys.argv[2])
output_file = sys.argv[3]

matrix_market_format = False
if len(sys.argv) == 5 and sys.argv[4].lower() == "matrix_market":
    matrix_market_format = True

# Generate a regular random graph
G = nx.random_regular_graph(degree, num_vertices, seed=123)

# Get the edge list as pairs of vertices
edges = G.edges()

# Write the edge list to the original output file in the original format
with open(output_file, "w") as file:
    # Write the header lines for the original format
    file.write(f"vertices {num_vertices}\n")
    file.write(f"edges {len(edges)}\n")
    
    # Write each edge with the "edge" prefix, adding 1 to each vertex
    for edge in edges:
        file.write(f"edge {edge[0] + 1} {edge[1] + 1}\n")

print(f"Random graph with {num_vertices} vertices and {len(edges)} edges (vertices incremented by 1) written to {output_file}.")

# If matrix_market_format is True, write the Matrix Market format to a separate file
if matrix_market_format:
    matrix_market_file = output_file + ".mtx"
    with open(matrix_market_file, "w") as mm_file:
        mm_file.write("%%MatrixMarket matrix coordinate pattern symmetric\n")
        mm_file.write(f"{num_vertices} {num_vertices} {len(edges)}\n")
        for edge in edges:
            mm_file.write(f"{edge[0] + 1} {edge[1] + 1}\n")

    print(f"Matrix Market format written to {matrix_market_file}.")

