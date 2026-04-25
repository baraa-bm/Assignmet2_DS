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
├──src/main.cpp
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
1. What happens when inserting into a full root?
------------------------------------------------

When inserting into a full root, the root must be split before the new value is inserted.

Steps:
- The middle key moves upward.
- A new root is created.
- The old root is divided into two child nodes.
- The tree height increases by 1.
- The new value is inserted into the correct child node.

Example:

Before split:

    [10 20 30]

After split:

    Level 0: [20]
    Level 1: [10] [30]

The insertion then continues in the correct subtree.

----------------------------------------

2. Why is saving only keys not enough?
------------------------------------------------

Saving only keys is not enough because the exact tree structure would be lost.

A 2–3–4 Tree depends on:
- Which keys belong to the same node
- Parent-child relationships
- Whether nodes are leaf or internal nodes
- Tree hierarchy and shape

Two trees may contain identical keys but have different structures.

If only keys are saved:
- The original layout cannot be restored
- The rebuilt tree may differ from the original

----------------------------------------

3. How do you distinguish node types in serialization?
-------------------------------------------------------

Node types are distinguished by storing metadata for every node.

Each node saves:

- Leaf status
- Number of keys
- Key values


If the node is not a leaf:
- Child nodes are serialized immediately after the parent
- This preserves the exact structure during restoration

----------------------------------------

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




# Complexity Analysis

- Search , O(log n) 
- Insert , O(log n) 
- Level Order Print , O(n) 
- Save , O(n) 
- Restore , O(n) 

Because the tree remains balanced, operations scale logarithmically.

---
