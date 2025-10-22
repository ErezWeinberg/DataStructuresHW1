# DataStructuresHW1
Data Structures Course - Homework Assignment 1 (Spring 2025)

## Project Overview

This project implements **DSpotify**, a music streaming service management system, as part of the Data Structures course (234218) at Technion. The implementation uses AVL trees to efficiently manage songs and playlists with various operations.

## Features

The DSpotify system supports the following operations:

- **Playlist Management**
  - Add new playlists
  - Delete empty playlists
  - Unite two playlists into one

- **Song Management**
  - Add songs with play counts
  - Delete songs (only if not in any playlist)
  - Track song play counts
  - Add songs to playlists
  - Remove songs from playlists

- **Query Operations**
  - Get play count for a specific song
  - Get number of songs in a playlist
  - Find song with closest play count to a given value

## Data Structures

The project implements and uses the following data structures:

1. **AVL Tree** (`AvLTree.h`)
   - Self-balancing binary search tree
   - Guarantees O(log n) operations for insert, delete, and search
   - Template-based implementation with custom comparators

2. **Song** (`song.h`, `song.cpp`)
   - Stores song ID and play count
   - Tracks which playlists contain the song
   - Supports comparison by ID and by play count

3. **Playlist** (`PlayList.h`, `PlayList.cpp`)
   - Maintains two AVL trees:
     - Songs sorted by ID for fast lookup
     - Songs sorted by play count for range queries
   - Supports merging playlists efficiently

4. **DSpotify** (`dspotify25b1.h`, `dspotify25b1.cpp`)
   - Main system class
   - Manages collections of songs and playlists
   - Implements all required operations

## File Structure

```
.
├── AvLTree.h              # AVL tree template implementation
├── song.h / song.cpp      # Song class definition and implementation
├── PlayList.h / PlayList.cpp  # Playlist class definition and implementation
├── dspotify25b1.h / dspotify25b1.cpp  # Main DSpotify system
├── main25b1.cpp           # Main program with command-line interface
├── wet1util.h             # Utility types (StatusType, output_t)
├── CMakeLists.txt         # CMake build configuration
├── run_tests.py           # Test runner script
└── tests/                 # Test input and expected output files
```

## Building the Project

### Prerequisites

- C++ compiler with C++14 support (e.g., g++ 5.0 or later)
- CMake 3.29 or later (for CMake build)
- Python 3.x (for running tests)

### Compilation Methods

#### Method 1: Using the test runner (Recommended)

```bash
python3 run_tests.py
```

This will automatically compile the project and run all tests.

#### Method 2: Manual compilation

```bash
g++ -std=c++14 -DNDEBUG -Wall -o main.out *.cpp
```

#### Method 3: Using CMake

```bash
mkdir build
cd build
cmake ..
make
```

## Running Tests

The project includes a comprehensive test suite located in the `tests/` directory.

### Run all tests:

```bash
python3 run_tests.py
```

### Run specific tests:

```bash
python3 run_tests.py -t 0 10 20
```

### Clean test result files:

```bash
python3 run_tests.py --clean
```

### Test options:

- `--tests_dir`: Path to test directory (default: `./tests/`)
- `--code_dir`: Path to source code directory (default: `./`)
- `--compiler_path`: Path to g++ compiler (default: `g++`)
- `--abort_on_fail`: Stop on first test failure
- `-t, --tests`: List of specific test IDs to run

## Running the Program

The program reads commands from standard input and outputs results to standard output.

```bash
./main.out < input_file.in > output_file.out
```

### Supported Commands

- `add_playlist <playlistId>`
- `delete_playlist <playlistId>`
- `add_song <songId> <plays>`
- `add_to_playlist <playlistId> <songId>`
- `delete_song <songId>`
- `remove_from_playlist <playlistId> <songId>`
- `get_plays <songId>`
- `get_num_songs <playlistId>`
- `get_by_plays <playlistId> <plays>`
- `unite_playlists <playlistId1> <playlistId2>`

### Output Format

Each command produces output in the format:
```
<command>: <STATUS> [, <result>]
```

Where STATUS is one of:
- `SUCCESS`
- `FAILURE`
- `INVALID_INPUT`
- `ALLOCATION_ERROR`

## Complexity Analysis

All operations are designed to run efficiently:

- **add_song**: O(log n) - Binary search tree insertion
- **add_playlist**: O(log m) - Binary search tree insertion
- **add_to_playlist**: O(log m + log n_playlist) - Find playlist + insert song
- **delete_song**: O(log n) - Binary search tree deletion
- **delete_playlist**: O(log m) - Binary search tree deletion
- **remove_from_playlist**: O(log m + log n_playlist) - Find playlist + remove song
- **get_plays**: O(log n) - Binary search tree lookup
- **get_num_songs**: O(log m) - Find playlist + constant time access
- **get_by_plays**: O(log m + log n_playlist) - Find playlist + binary search
- **unite_playlists**: O(n1 + n2) - Merge two playlists

Where:
- n = total number of songs
- m = total number of playlists
- n_playlist = number of songs in a specific playlist

## Implementation Notes

- The implementation uses **AVL trees** to maintain balanced trees and guarantee logarithmic time complexity
- Songs are stored in multiple tree structures to enable efficient queries by different criteria
- Memory management is handled explicitly with proper cleanup in destructors
- The code includes Hebrew comments from the original assignment

## Authors

Implementation by students as part of Data Structures course assignment.

## License

This is an academic project for educational purposes.
