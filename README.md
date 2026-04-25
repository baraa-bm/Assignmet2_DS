# 2–3–4 Tree (B-Tree)

## Student Information
Applied Data Structures  
Assignment 2

Name : Baraa Ben Mansour

ID: 900244128
 

---

# Project Description

This project implements a **2–3–4 Tree**, which is a special case of a **B-Tree of order 4**.

The tree supports:

- Insertion
- Search
- Level-order traversal
- File-based operations
- Tree logging
- Split counting
- Save/Restore persistence
- Reverse reconstruction analysis

The implementation follows the assignment requirements and demonstrates how balanced trees maintain logarithmic performance through node splitting.

---

# Project Structure

```txt
Assignment2_DS/
│
├──main.cpp
├── input.txt
├── output.txt
├── log.txt
├── snapshot.dat
├── README.md
```

---

# Compilation and Execution

Compile:

```bash
g++ src/main.cpp -o btree
```

Run:

```bash
./btree
```

---

# Input File

The program reads operations from:

```txt
input.txt
```

Example:

```txt
I 10
I 20
I 5
S 20
D 10
SAVE
RESTORE
```

Supported commands:


Insertion

Deletion

Search

Save (Save tree to snapshot.dat)

Restore (Restore tree from snapshot.dat)



Invalid lines are ignored.

---

# Data Structure Overview

A 2–3–4 Tree is a balanced multi-way search tree where each node may contain:

- 1 key → 2 children
- 2 keys → 3 children
- 3 keys → 4 children

The implementation uses:

```txt
Minimum degree t = 2
```

This means:

- Maximum keys per node = 3
- Maximum children per node = 4

---

# Implemented Functions

## Core Functions

### insert(k)
Inserts a value into the tree.

### search(k)
Searches for a value.

### insertNonFull()
Handles insertion into nodes that are not full.

### splitChild()
Splits a full node during insertion.

### traverse()
Prints tree in sorted order.

### levelOrder()
Displays the tree level-by-level.

---

# Tree Visualization

After every operation, the tree is displayed.

Example:

```txt
Insert 6:
Level 0: [10]
Level 1: [5 6] [20]
```

---

# Split Counter

The program tracks how many splits occur.

Example:

```txt
Total splits = 2
```

This helps analyze tree balancing behavior.

---

# Height Tracking

Tree height is calculated after every operation.

Example:

```txt
Height = 2
```

---

# Logging System

The program creates:

```txt
log.txt
```

The log file stores:

- Tree state after each operation
- Split count
- Height
- Search results
- Save/Restore operations

This helps debugging and structural analysis.

---

# Save / Restore Persistence

The program supports persistence using:

```txt
snapshot.dat
```

## SAVE
Stores the tree structure.

## RESTORE
Rebuilds the tree from the saved snapshot.

Pointers are not stored directly because memory addresses change between executions.

Instead, the tree is serialized using:

- node type (leaf or non-leaf)
- number of keys
- key values

---

# Reverse Reconstruction


Steps:

1. Store inserted values
2. Build tree normally
3. Rebuild using reverse insertion order
4. Compare structure

Compared metrics:

- Height
- Splits
- Tree layout

---

# Complexity Analysis

- Search , O(log n) 
- Insert , O(log n) 
- Level Order Print , O(n) 
- Save , O(n) 
- Restore , O(n) 

Because the tree remains balanced, operations scale logarithmically.

---

# Edge Cases Handled

## Duplicate Insertions

Duplicate values are ignored.

Example:

```txt
I 10
I 10
```

Second insertion is skipped.

---

## Full Root Split

When the root becomes full:

- Root splits
- Height increases
- Middle key moves upward

Example:

Before:

```txt
[5 10 20]
```

After split:

```txt
      [10]
     /    \
   [5]    [20]
```

---






