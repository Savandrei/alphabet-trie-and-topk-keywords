# File Search and Inverted Indexing System

An in-memory file indexing and keyword retrieval engine written in C. The application indexes documents using a multi-way retrieval tree (trie), maintains document records in a doubly linked list, and evaluates ranked queries via a binary max-heap.

## Overview

The system associates individual files—defined by unique string identifiers and relevance scores—with collections of keywords. It supports dynamic insertions, removals, exact match lookups, top-$k$ ranked searches, full index serialization in lexicographical order, and prefix-based subtree searches.

### Core Data Structures

1. **Doubly Linked List (`TStart_End_List`, `TNextPrevCell`)**:
   * Stores complete file metadata (`ID`, `score`, and the dynamic array of keywords).
   * Maintains chronological insertion order while providing direct references to head and tail elements.

2. **Multi-Way Retrieval Tree / Trie (`TTCell`, `Ttree`)**:
   * Implements a first-child (`down`), right-sibling (`right`) representation for keyword storage.
   * Maintains sibling nodes in lexicographical order.
   * Terminal character nodes contain a singly linked list (`TPCell`, `TPList`) storing direct pointers to records in the doubly linked list.

3. **Max-Heap (`THeap`)**:
   * Implements a binary heap to process ranked queries (`TOPK`).
   * Primary priority criterion: relevance score in descending order.
   * Secondary tie-breaker: file identifier in lexicographical ascending order.

## Command Specifications and Implementation Details

### File and Keyword Insertion

* **`ADD <id> <score> <count> <keyword_1> ... <keyword_n>`**:
  * Scans the doubly linked list to ensure the identifier is unique. If an identical identifier is encountered, the operation outputs `EXISTS` and frees temporary buffers.
  * If unique, the record is appended to the tail of the list. Distinct keywords are added to the multi-way retrieval tree, and file pointers are appended to the corresponding terminal nodes.

* **`ADDKW <id> <keyword>`**:
  * Locates the record matching the specified file identifier. If the record does not exist, the operation prints `NOT FOUND`.
  * If the keyword is already associated with the file, the system prints `OK` with no state modifications.
  * Otherwise, the keyword array is reallocated, the keyword is inserted into the trie, and the file pointer is linked to the trie's terminal node.

### Deletion and Pruning

* **`DEL <id>`**:
  * Locates the file record in the list. Returns `NOT FOUND` if absent.
  * Traverses each associated keyword in the retrieval tree, unlinking the corresponding file pointer.
  * Prunes stale nodes using `remove_empty_branches`.
  * Unlinks the file node from the doubly linked list and deallocates all associated dynamic memory.

* **`DELKW <id> <keyword>`**:
  * Verifies the existence of the file identifier and keyword. If the file is missing, prints `NOT FOUND`. If the keyword is not associated with the file, prints `OK` without modifying internal state.
  * Unlinks the file reference from the keyword's terminal node, prunes dead branches, shifts remaining keyword references, and decrements the keyword count.

* **Branch Pruning (`remove_empty_branches`)**:
  * Traverses post-order through child and sibling pointers.
  * Nodes that are not terminal for any keyword and possess no active child branches are removed and freed recursively.

### Retrieval and Ranking

* **`FIND <keyword>`**:
  * Traverses down matching character branches.
  * If the keyword terminates at a valid terminal node, outputs the number of matches followed by file identifiers sorted in lexicographical order. If not found, prints `EMPTY`.

* **`TOPK <keyword> <k>`**:
  * Traverses to the terminal node of the keyword.
  * Inserts all matching file references into a max-heap.
  * Extracts up to $\min(k, \text{total})$ elements based on score and identifier criteria. Prints `EMPTY` if no file matches exist.

### Index Traversal

* **`PRINT`**:
  * Performs a depth-first search (DFS) over the trie using `down` prior to `right`.
  * Reconstructs candidate strings using an internal traversal buffer.
  * Outputs each unique keyword followed by the count and identifiers of associated files. Because siblings are inserted in sorted order, output entries naturally follow lexicographical ordering.

### Prefix Search

* **`PREFIX <prefix>`**:
  * Navigates to the node matching the final character of the given prefix.
  * Recursively traverses all subtrees rooted at that node, collecting file references from encountered terminal nodes into an auxiliary list.
  * Deduplicates matching file records and sorts them lexicographically. Outputs `EMPTY` if no matching keywords exist.

## Build and Execution

The program expects inputs from `indexare.in` and writes output directly to `indexare.out`.

### Compilation Commands

```
# Compile search_index binary
make build

# Execute binary
make run

# Run memory diagnostics with Valgrind
make valgrind

# Clean object files and executables
make clean

# Generate project archive
make pack
```

## Directory Structure

```
.
|-- Makefile        # Compilation and utility directives
|-- tema2.c         # Source implementation of data structures and algorithms
|-- indexare.in     # Input file
|-- indexare.out    # Output destination
`-- README.md       # Project documentation
```

## Architectural Notes

* **Unified Source File**: All data structures and routines reside within `tema2.c` to streamline compilation and targeted symbolic debugging.
* **Pointer Decoupling**: Trie nodes store non-owning references to entries in the primary doubly linked list, avoiding duplicate allocations of file metadata.
* **Resource Cleanup**: Explicit deallocation functions (`destroy_tree`, `destroy_NextPrevList`, `destroy_plist`, `Destroy_Heap`) ensure complete memory reclamation prior to termination.
