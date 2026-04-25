#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class BTreeNode {
public:
    int *keys;
    int t;
    BTreeNode **children;
    int n;
    bool leaf;

    BTreeNode(int _t, bool _leaf) {
        t = _t;
        leaf = _leaf;
        keys = new int[2 * t - 1];
        children = new BTreeNode*[2 * t];
        n = 0;

        for (int i = 0; i < 2 * t; i++)
            children[i] = nullptr;
    }

    bool search(int k) {
        int i = 0;

        while (i < n && k > keys[i])
            i++;

        if (i < n && keys[i] == k)
            return true;

        if (leaf)
            return false;

        return children[i]->search(k);
    }

    void traverse(ostream &out) {
        int i;
        for (i = 0; i < n; i++) {
            if (!leaf)
                children[i]->traverse(out);

            out << keys[i] << " ";
        }

        if (!leaf)
            children[i]->traverse(out);
    }

    void insertNonFull(int k, int &splitCount) {
        int i = n - 1;

        if (leaf) {
            while (i >= 0 && keys[i] > k) {
                keys[i + 1] = keys[i];
                i--;
            }

            keys[i + 1] = k;
            n++;
        } else {
            while (i >= 0 && keys[i] > k)
                i--;

            i++;

            if (children[i]->n == 2 * t - 1) {
                splitChild(i, children[i], splitCount);

                if (keys[i] < k)
                    i++;
            }

            children[i]->insertNonFull(k, splitCount);
        }
    }

    void splitChild(int i, BTreeNode *y, int &splitCount) {
        BTreeNode *z = new BTreeNode(y->t, y->leaf);
        z->n = t - 1;

        for (int j = 0; j < t - 1; j++)
            z->keys[j] = y->keys[j + t];

        if (!y->leaf) {
            for (int j = 0; j < t; j++)
                z->children[j] = y->children[j + t];
        }

        y->n = t - 1;

        for (int j = n; j >= i + 1; j--)
            children[j + 1] = children[j];

        children[i + 1] = z;

        for (int j = n - 1; j >= i; j--)
            keys[j + 1] = keys[j];

        keys[i] = y->keys[t - 1];
        n++;

        splitCount++;
    }

    void save(ofstream &out) {
        out << leaf << " " << n << " ";

        for (int i = 0; i < n; i++)
            out << keys[i] << " ";

        out << endl;

        if (!leaf) {
            for (int i = 0; i <= n; i++)
                children[i]->save(out);
        }
    }

    static BTreeNode* restore(ifstream &in, int t) {
        int leafInt, keyCount;

        if (!(in >> leafInt >> keyCount))
            return nullptr;

        BTreeNode *node = new BTreeNode(t, leafInt == 1);
        node->n = keyCount;

        for (int i = 0; i < keyCount; i++)
            in >> node->keys[i];

        if (!node->leaf) {
            for (int i = 0; i <= keyCount; i++)
                node->children[i] = restore(in, t);
        }

        return node;
    }
};

class BTree {
private:
    BTreeNode *root;
    int t;
    int splitCount;

public:
    BTree(int _t = 2) {
        root = nullptr;
        t = _t;
        splitCount = 0;
    }

    int getSplits() {
        return splitCount;
    }

    bool search(int k) {
        if (root == nullptr)
            return false;

        return root->search(k);
    }

    void insert(int k) {
        if (search(k)) {
            cout << "Duplicate ignored: " << k << endl;
            return;
        }

        if (root == nullptr) {
            root = new BTreeNode(t, true);
            root->keys[0] = k;
            root->n = 1;
        } else {
            if (root->n == 2 * t - 1) {
                BTreeNode *s = new BTreeNode(t, false);

                s->children[0] = root;
                s->splitChild(0, root, splitCount);

                int i = 0;
                if (s->keys[0] < k)
                    i++;

                s->children[i]->insertNonFull(k, splitCount);

                root = s;
            } else {
                root->insertNonFull(k, splitCount);
            }
        }
    }

    void traverse(ostream &out) {
        if (root == nullptr) {
            out << "Tree is empty";
            return;
        }

        root->traverse(out);
    }

    void levelOrder(ostream &out) {
        if (root == nullptr) {
            out << "Tree is empty" << endl;
            return;
        }

        BTreeNode **queue = new BTreeNode*[10000];
        int *levels = new int[10000];

        int front = 0;
        int rear = 0;

        queue[rear] = root;
        levels[rear] = 0;
        rear++;

        int currentLevel = -1;

        while (front < rear) {
            BTreeNode *node = queue[front];
            int level = levels[front];
            front++;

            if (level != currentLevel) {
                if (currentLevel != -1)
                    out << endl;

                currentLevel = level;
                out << "Level " << level << ": ";
            }

            out << "[";
            for (int i = 0; i < node->n; i++) {
                out << node->keys[i];
                if (i != node->n - 1)
                    out << " ";
            }
            out << "] ";

            if (!node->leaf) {
                for (int i = 0; i <= node->n; i++) {
                    queue[rear] = node->children[i];
                    levels[rear] = level + 1;
                    rear++;
                }
            }
        }

        out << endl;

        delete[] queue;
        delete[] levels;
    }

    int height() {
        int h = 0;
        BTreeNode *curr = root;

        while (curr != nullptr && !curr->leaf) {
            h++;
            curr = curr->children[0];
        }

        if (root == nullptr)
            return 0;

        return h + 1;
    }

    void saveToFile(string filename) {
        ofstream out(filename);

        if (!out) {
            cout << "Could not open snapshot file." << endl;
            return;
        }

        if (root == nullptr) {
            out << "EMPTY" << endl;
        } else {
            out << "TREE" << endl;
            root->save(out);
        }

        out.close();
    }

    void restoreFromFile(string filename) {
        ifstream in(filename);

        if (!in) {
            cout << "Could not open snapshot file." << endl;
            return;
        }

        string status;
        in >> status;

        if (status == "EMPTY") {
            root = nullptr;
        } else if (status == "TREE") {
            root = BTreeNode::restore(in, t);
        }

        in.close();
    }
};

void printState(BTree &tree, ostream &out, string operation) {
    out << operation << ":" << endl;
    tree.levelOrder(out);
    out << "Total splits = " << tree.getSplits() << endl;
    out << "Height = " << tree.height() << endl;
    out << "------------------------" << endl;
}

int main() {
    BTree tree(2);

    ifstream input("input.txt");
    ofstream output("output.txt");
    ofstream log("log.txt");

    if (!input) {
        cout << "input.txt could not be opened." << endl;
        return 0;
    }

    string op;
    int x;

    int inserted[10000];
    int insertCount = 0;

    while (input >> op) {
        if (op == "I") {
            if (input >> x) {
                tree.insert(x);
                inserted[insertCount++] = x;

                printState(tree, cout, "Insert " + to_string(x));
                printState(output, "Insert " + to_string(x));
                printState(log, "Insert " + to_string(x));
            }
        } else if (op == "S") {
            if (input >> x) {
                bool found = tree.search(x);

                cout << "Search " << x << ": ";
                output << "Search " << x << ": ";
                log << "Search " << x << ": ";

                if (found) {
                    cout << "Found" << endl;
                    output << "Found" << endl;
                    log << "Found" << endl;
                } else {
                    cout << "Not Found" << endl;
                    output << "Not Found" << endl;
                    log << "Not Found" << endl;
                }

                printState(tree, cout, "After Search");
                printState(output, "After Search");
                printState(log, "After Search");
            }
        } else if (op == "D") {
            if (input >> x) {
                cout << "Delete " << x << ": Delete is optional and not implemented." << endl;
                output << "Delete " << x << ": Delete is optional and not implemented." << endl;
                log << "Delete " << x << ": Delete is optional and not implemented." << endl;

                printState(tree, cout, "After Delete Attempt");
                printState(output, "After Delete Attempt");
                printState(log, "After Delete Attempt");
            }
        } else if (op == "SAVE") {
            tree.saveToFile("snapshot.dat");

            cout << "Tree saved to snapshot.dat" << endl;
            output << "Tree saved to snapshot.dat" << endl;
            log << "Tree saved to snapshot.dat" << endl;
        } else if (op == "RESTORE") {
            tree.restoreFromFile("snapshot.dat");

            cout << "Tree restored from snapshot.dat" << endl;
            output << "Tree restored from snapshot.dat" << endl;
            log << "Tree restored from snapshot.dat" << endl;

            printState(tree, cout, "After Restore");
            printState(output, "After Restore");
            printState(log, "After Restore");
        } else {
            string restOfLine;
            getline(input, restOfLine);
        }
    }

    cout << endl << "Reverse Reconstruction Test" << endl;
    output << endl << "Reverse Reconstruction Test" << endl;
    log << endl << "Reverse Reconstruction Test" << endl;

    BTree reverseTree(2);

    for (int i = insertCount - 1; i >= 0; i--)
        reverseTree.insert(inserted[i]);

    printState(reverseTree, cout, "Reverse Tree");
    printState(reverseTree, output, "Reverse Tree");
    printState(reverseTree, log, "Reverse Tree");

    input.close();
    output.close();
    log.close();

    return 0;
}