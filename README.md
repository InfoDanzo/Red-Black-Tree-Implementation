# Data Structures: Red-Black Tree in C

This repository contains, production-ready implementation of a **Red-Black Tree (RBT)** written in C.

A Red-Black Tree is a self-balancing binary search tree where each node stores an extra bit for denoting the color (**RED** or **BLACK**). These colors are used to ensure the tree remains approximately balanced during insertions and deletions.

##  Key Features

- **Sentinel NIL Node:** Implements a single, globally shared sentinel node (`NIL`) to represent all leaves, optimizing memory overhead and simplifying boundary condition checks during rotations.
- **Dynamic Balancing:** Full implementation of `leftrotate` and `rightrotate` mechanics alongside the 3-case corrective `fixup` loop.
- **Comprehensive Benchmarking:** The execution includes both a visual proof-of-concept module and a test module with randomized elements. the number of elements is editable by user (edit in: #define elementi xxx).


---

##  Properties of a Red-Black Tree

The algorithm guarantees that no path from the root to a leaf is more than twice as long as any other, keeping the height bounded at $h \le 2 \log_2(n + 1)$. This is achieved by enforcing 5 strict rules:

1. Every node is either **RED** or **BLACK**.
2. The **Root** is always **BLACK**.
3. Every leaf (`NIL`) is **BLACK**.
4. If a node is **RED**, both its children must be **BLACK** (No consecutive RED nodes allowed).
5. For each node, all simple paths from the node to descendant leaves contain the same number of Black nodes (*Black-Height*).

---

##  Time & Space Complexity

Thanks to the self-balancing properties, operations that would degenerate to $O(n)$ in a standard Binary Search Tree (BST) are kept strictly log-time.

| Operation | Average Case | Worst Case | Space Complexity |
| :--- | :--- | :--- | :--- |
| **Insertion** | $O(\log n)$ | $O(\log n)$ | $O(n)$ |
| **Search** | $O(\log n)$ | $O(\log n)$ | $O(1)$ |
| **Deletion** | $O(\log n)$ | $O(\log n)$ | $O(1)$ |

### Performance Results (500,000 randomized elements)
- **Time elapsed:** ~0.02 - 0.04 seconds (depending on host CPU).

---

##  Compilation and Execution

To run the simulation, compile the code using any standard C compiler (e.g., `gcc`):

```bash
gcc -O3 rbt.c -o rbt
./rbt