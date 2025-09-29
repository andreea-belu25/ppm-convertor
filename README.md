# Quadtree Image Compression

A C implementation of quadtree-based image compression and decompression for PPM format images.

## Project Structure

### task1.c - Compression Tree Analysis
Core compression tree building and analysis functions:
- `check()`: Determines if a node should be a leaf based on color variance threshold
- `arb_compresie()`: Constructs the compression tree using recursive quadtree subdivision
- `Min()`: Finds minimum value among four inputs for level calculations
- `niv_min_frunza()`: Determines the minimum level containing any leaf node
- `Max()`: Finds maximum value among four inputs for size calculations
- `info_nod_max()`: Retrieves maximum leaf dimension from the minimum level
- `afisare()`: Debugging function for tree visualization
- `NrNiveluri()`: Calculates total number of levels in the tree
- `Numara_Frunze()`: Counts total number of leaf nodes in the tree
- **Output**: Text file containing tree statistics (levels, leaf count, maximum undivided block size)

### task2.c - Binary Tree Serialization
Queue-based breadth-first tree traversal for binary output:
- `InitQ()`: Initializes empty queue for tree traversal
- `InsQ()`: Enqueues tree nodes for processing
- `ExtQ()`: Dequeues nodes during traversal
- `DistrQ()`: Deallocates queue memory when finished
- **Process**: Builds compression tree and serializes it in breadth-first order to binary file

### task3.c - Image Decompression
Complete decompression pipeline from binary file to PPM image:

**Step 1: Pixel queue formation**
- `InitQ_pixel()`: Initializes pixel queue from compressed file
- `InsQ_pixel()`: Enqueues pixels during file reading
- `ExtQ_pixel()`: Dequeues pixels during tree construction

**Step 2: Tree reconstruction**
- `construieste_coada_pixeli()`: Reads compressed file and builds pixel queue
- `construieste_arbore()`: Reconstructs tree structure from pixel queue

**Step 3: Grid matrix formation**
- `Parcurge_Arb()`: Traverses tree to fill pixel grid matrix

**Step 4: Image output**
- `afisare_imagine()`: Writes reconstructed image to PPM file

**Step 5: Memory cleanup**
- `distruge()`: Deallocates tree memory
- `DistrugereQ()`: Deallocates pixel queue memory

### main.c - Command Line Interface
Program entry point handling three operation modes:

**Common Operations:**
- PPM image file reading and validation
- Command line argument parsing and factor conversion
- File I/O management and error handling

**Operation Modes:**
- `-c1 <factor> <input.ppm> <output.txt>`: Compress and analyze (text output)
- `-c2 <factor> <input.ppm> <output.bin>`: Compress to binary format
- `-d <input.bin> <output.ppm>`: Decompress binary file to image
