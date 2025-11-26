#include <bits/stdc++.h>
#include "color.hpp"
using namespace std;

#define left 0
#define right 1


template <typename T1, typename T2>
class Node
{
public:
    T1 key;
    int color;
    T2 info;
    Node *child[2];

    Node(T1 key, int color, T2 info)
    {
        this->key= key;
        this->color = color;
        this->child[left] = NULL;
        this->child[right] = NULL;
        this->info = info;
    }
    Node(T1 key,T2 info)
    {
        this->key= key;
        this->color = 0;///Assuming default color is RED
        this->child[left] = NULL;
        this->child[right] = NULL;
        this->info = info;
    }

};

#define RED 0
#define BLACK 1

template<typename t1,typename t2>
class MAP
{
    Node<t1,t2> *root;

    bool red(Node<t1,t2> *node)
    {
        if (node == NULL)
            return false;

        return node->color == RED;
    }

    void colorFlip(Node<t1,t2> *node)
    {
        node->color = node->color ^ 1;
        node->child[left]->color = node->child[left]->color ^ 1;
        node->child[right]->color = node->child[right]->color ^ 1;
    }


    Node<t1,t2> *rotate(Node<t1,t2> *node, bool direction)
    {
        Node<t1,t2> *temp = node->child[!direction];
        node->child[!direction] = temp->child[direction];
        temp->child[direction] = node;

        temp->color = node->color;
        node->color = RED;

        return temp;
    }

    Node<t1,t2> *doubleRotate(Node<t1,t2> *node, bool direction)
    {
        node->child[!direction] = rotate(node->child[!direction], !direction);
        return rotate(node, direction);
    }


    bool findHelp(t1 key, Node<t1,t2> *root)
    {
        if (root == NULL)
            return false;
        if (key == root->key)
            return true;
        else if (key < root->key)
            return findHelp(key, root->child[left]);
        else
            return findHelp(key, root->child[right]);
    }



    Node<t1,t2>* findHelp2(t1 key, Node<t1,t2> *root)
    {
        if(root==NULL) return root;
        if (key == root->key)
            return root;
        else if (key < root->key)
            return findHelp2(key, root->child[left]);
        else
            return findHelp2(key, root->child[right]);
    }



    Node<t1,t2> *INSERT_FIX_UP(Node<t1,t2> *node, bool direction)
    {
        if (red(node->child[direction]))
        {
            if (red(node->child[!direction]))
            {
                if (red(node->child[direction]->child[direction]) || red(node->child[direction]->child[!direction]))
                {
                    colorFlip(node);
                }
            }
            else
            {
                if (red(node->child[direction]->child[direction]))
                {
                    /// LL RR
                    node = rotate(node, !direction);
                }
                else if (red(node->child[direction]->child[!direction]))
                {
                    /// Align And Rotate LR RL
                    node = doubleRotate(node, !direction);
                }
            }
        }
        return node;
    }


    Node<t1,t2> *__insert(Node<t1,t2> *node, t1 key,t2 info)
    {
        if (node == NULL)
            return new Node<t1,t2>(key, RED, info);
        Node<t1,t2> *temp=findHelp2(key,node);
        if(temp)
        {
            temp->info=info;
            return node;
        }

        bool direction = key > node->key;

        node->child[direction] = __insert(node->child[direction], key, info);

        return INSERT_FIX_UP(node, direction);
    }

    int nodeCountHelp(Node<t1,t2> *root)
    {
        if (root == NULL)
            return 0;
        return 1 + (nodeCountHelp(root->child[left]) + nodeCountHelp(root->child[right]));
    }

    void inOrderTraversalHelp(Node<t1,t2> *root, ofstream &out)
    {
        if (root == NULL) return;
        inOrderTraversalHelp(root->child[left], out);


        if(root->color==RED) out << dye::red(root->key) << dye::red(" \u21D2 ") << dye::red(root->info) << "\n";
        else out << root->key << " \u21D2 " << root->info << "\n";
        if(root->color==RED) cout << dye::red(root->key) << dye::red(" => ") << dye::red(root->info) << "\n";
        else cout << root->key << " => " << root->info << "\n";


        inOrderTraversalHelp(root->child[right], out);
    }

    void ClearHelp(Node<t1,t2> *root)
    {
        if (root == NULL) return;
        ClearHelp(root->child[left]);
        ClearHelp(root->child[right]);
        free(root);
   }

    Node<t1,t2>* getMax(Node<t1,t2> *node)
    {
        Node<t1,t2> *temp=node;
        while (temp->child[right] != NULL)
            temp = temp->child[right];
        return temp;
    }


    void printTree(Node<t1,t2> *root, ofstream &out)
    {
        if (root == NULL)
        {
            return;
        }
        if (root->child[left] == NULL and root->child[right] == NULL)
        {
            if(root->color==RED) out <<dye::red(root->key)<<dye::red("_")<<dye::red(root->info);
            else out <<root->key<<"_"<<root->info;
            if(root->color==RED) cout <<dye::red(root->key)<<dye::red("_")<<dye::red(root->info);
            else cout <<root->key<<"_"<<root->info;
        }
        else if (root->child[right] == NULL)
        {
            if(root->color==RED) out <<dye::red(root->key)<<dye::red("_")<<dye::red(root->info);
            else out <<root->key<<"_"<<root->info;
            if(root->color==RED) cout <<dye::red(root->key)<<dye::red("_")<<dye::red(root->info);
            else cout <<root->key<<"_"<<root->info;
            out << "(";
            cout << "(";
            printTree(root->child[left], out);
            out << ",)";
            cout << ",)";
        }
        else if (root->child[left] == NULL)
        {
            if(root->color==RED) out <<dye::red(root->key)<<dye::red("_")<<dye::red(root->info);
            else out <<root->key<<"_"<<root->info;
            if(root->color==RED) cout <<dye::red(root->key)<<dye::red("_")<<dye::red(root->info);
            else cout <<root->key<<"_"<<root->info;
            out << "(,";
            cout << "(,";
            printTree(root->child[right], out);
            out << ")";
            cout << ")";
        }
        else
        {
            if(root->color==RED) out <<dye::red(root->key)<<dye::red("_")<<dye::red(root->info);
            else out <<root->key<<"_"<<root->info;
            if(root->color==RED) cout <<dye::red(root->key)<<dye::red("_")<<dye::red(root->info);
            else cout <<root->key<<"_"<<root->info;
            out << "(";
            cout << "(";
            printTree(root->child[left], out);
            out << ",";
            cout << ",";
            printTree(root->child[right], out);
            out << ")";
            cout << ")";
        }
    }



    Node<t1,t2> *__delete(Node<t1,t2> *node, t1 key, bool &ok,ofstream &out)
    {
        if (node == NULL)
        {
            ok = true;
            out<<key<<" not found\n";
            return node;
        }

        if (node->key == key)
        {
            if (node->child[left] == NULL || node->child[right] == NULL)
            {
                Node<t1,t2> *temp = NULL;
                if (node->child[left])
                    temp = node->child[left];
                if (node->child[right])
                    temp = node->child[right];

                if (red(node))
                {
                    free(node);
                    ok = true;
                }
                else if (red(temp))
                {
                    temp->color = BLACK;
                    free(node);
                    ok = true;
                }

                return temp;
            }
            else
            {
                Node<t1,t2> *temp = getMax(node->child[left]);
                node->key = temp->key;
                node->info =temp->info;
                key = temp->key;
            }
        }

        bool direction = key > node->key;

        node->child[direction] = __delete(node->child[direction],key, ok,out);
        if(ok) return node;
        else return  DELETE_FIX_UP(node, direction, ok);
    }

    Node<t1,t2> *DELETE_FIX_UP(Node<t1,t2> *node, bool direction, bool &ok)
    {
        Node<t1,t2>  *parent = node;
        Node<t1,t2>  *sibling = node->child[!direction];

        if (red(sibling))
        {
            node = rotate(node, direction);
            sibling = parent->child[!direction];
        }

        if (sibling != NULL)
        {
            /// Black Sibling Case , Part 1 : Black Sibling with only black children
            if (!red(sibling->child[left]) && !red(sibling->child[right]))
            {
                if (red(parent))
                    ok = true; /// will color it black and sibling subtree will not have imbalance

                parent->color = BLACK; /// if not ok , we proceed further
                sibling->color = RED;
            }
            else /// Black Sibling Case , Part 2 : Black Sibling with not red children
            {

                int parent_color = parent->color;
                bool RedSiblingReduction = !(node == parent);

                if (red(sibling->child[!direction])) /// RR , LL
                {
                    parent = rotate(parent, direction); /// single rotation
                }
                else
                {
                    parent = doubleRotate(parent, direction); /// align and rotate
                }

                parent->color = parent_color; /// color will be the same as initial parent
                parent->child[left]->color = BLACK;
                parent->child[right]->color = BLACK;

                if (RedSiblingReduction)
                {
                    node->child[direction] = parent; /// fixing the child for proper bottom up fixing later
                }
                else
                {
                    node = parent; /// usual black case
                }

                ok = true;
            }
        }

        return node;
    }


public:
    MAP()
    {
        root = NULL;
    }

    int size()
    {
        return nodeCountHelp(root);
    }

    bool search(int key)
    {
        return findHelp(key, root);
    }

    void iterate(ofstream &out)
    {
        inOrderTraversalHelp(root, out);
    }

    void output(ofstream &out)
    {
        printTree(root, out);
        out<<"\n";
        cout<<"\n";
    }

    void insert(t1 key,t2 info)
    {
        root = __insert(root, key, info);
        root->color = BLACK;
    }

    void delete_( t1 key,ofstream &out)
    {
        bool ok = false;
        root = __delete(root, key, ok,out);
        if (root != NULL)
            root->color = BLACK;
    }

    bool empty()
    {
        return root == NULL;
    }

    bool clear2()
    {
        if (empty())
            return false;
        else
        {
            root = NULL;
            return true;
        }
    }

    bool clear()
    {
        if (empty())
            return false;
        else
        {
             ClearHelp(root);
             root=NULL;
             return true;
        }
    }

};

