#include<bits/stdc++.h>
using namespace std;

#define inf 100000000
#define int long long int
int z;

template<typename K,typename V>
class fibNode
{
public:
    int degree;///number of child
    bool mark;///if it lost a child or not
    K key;///key denotes priority,different values can have same priority
    V value;///value should be unique
    fibNode * parent;
    fibNode * child;
    fibNode * leftSibling;
    fibNode * rightSibling;

    fibNode(K key,V value)
    {
        this->key=key;
        this->value=value;
        this->parent=NULL;
        this->child=NULL;
        this->leftSibling=this;
        this->rightSibling=this;
        this->degree=0;
        this->mark=false;
    }
};


fibNode<int,int> **a;
int *b;
int *c;


template<typename k,typename v>
class fibonacciHeap
{
    fibNode<k,v> *maxKeyNode;///root


    ///unordered_map<v,fibNode<k,v>*> findNodeByValue;///helps increases key by value, not only int value,any type of value
    int numberOfNode;
    vector<fibNode<k,v>*> pointer_to_specific_degree;




    void printHelp(fibNode<k,v> *root,ofstream & out)
    {
        cout<<"("<<root->key<<","<<root->value<<")";
        out<<"("<<root->key<<","<<root->value<<")";
        if(root->child==NULL)
        {
            cout<<"\n";
            out<<"\n";
        }
        else
        {
            cout<<"->";
            out<<"->";
            fibNode<k,v> *current=root->child;

            int child_count=root->degree;
            for(int i=0; i<child_count; i++)
            {
                if(i==child_count-1)
                {
                    cout<<"("<<current->key<<","<<current->value<<")\n";
                    out<<"("<<current->key<<","<<current->value<<")\n";
                }
                else
                {
                    cout<<"("<<current->key<<","<<current->value<<"),";
                    out<<"("<<current->key<<","<<current->value<<"),";
                }


                current=current->rightSibling;
            }
            current=root->child;
            for(int i=0; i<child_count; i++)
            {
                if(current->degree!=0)printHelp(current,out);
                current=current->rightSibling;
            }
        }
    }
    void addToRootList(fibNode<k,v> *targetNode)
    {
        fibNode<k,v>* maxNode=this->maxKeyNode;
        targetNode->parent=NULL;
        targetNode->mark=false;
        if(maxNode==NULL)
        {
            this->maxKeyNode=targetNode;
            targetNode->rightSibling=targetNode;
            targetNode->leftSibling=targetNode;

        }
        else
        {
            fibNode<k,v>* maxLeft=maxNode->leftSibling;
            maxNode->leftSibling=targetNode;
            targetNode->rightSibling=maxNode;
            targetNode->leftSibling=maxLeft;
            maxLeft->rightSibling=targetNode;
            if(maxNode->key<targetNode->key)
            {
                this->maxKeyNode=targetNode;
            }
        }
    }
    void removeNodeFromRootList(fibNode<k,v> *target)
    {
        if(target->rightSibling!=target)
        {
            target->rightSibling->leftSibling=target->leftSibling;
            target->leftSibling->rightSibling=target->rightSibling;
        }

        if(target->parent!=NULL)
        {
            ///one child,just delete it
            if(target->parent->degree==1)
            {
                target->parent->child=NULL;
            }
            ///multiple child,delete target and chaange child pointer of parent
            else
            {
                target->parent->child=target->rightSibling;
            }
            target->parent->degree--;
        }
    }
    void cut(fibNode<k,v> *target)
    {
        ///cutting from childlist and adding to rootlist
        removeNodeFromRootList(target);
        addToRootList(target);
    }
    void cascadingCut(fibNode<k,v> *target)
    {
        fibNode<k,v> *parent=target->parent;
        if(parent!=NULL)
        {
            if(target->mark)
            {
                cut(target);
                cascadingCut(parent);
            }
            else target->mark=true;
        }
    }
    void consolidateAddHelper(fibNode<k,v>* node1,fibNode<k,v>* node2)
    {
        fibNode<k,v>* parent=node1;
        fibNode<k,v>* addchild=node2;
        if(parent->key<addchild->key) swap(parent,addchild);
        if(parent->degree==0)
        {
            parent->child=addchild;
            addchild->rightSibling=addchild;
            addchild->leftSibling=addchild;
            addchild->parent=parent;
            parent->degree++;
        }
        else
        {
            fibNode<k,v> *tempchild=parent->child;
            fibNode<k,v> *childleft=tempchild->leftSibling;
            tempchild->leftSibling=addchild;
            addchild->rightSibling=tempchild;
            addchild->leftSibling=childleft;
            childleft->rightSibling=addchild;
            addchild->parent=parent;
            parent->degree++;
        }
    }
    void fib_heap_link(fibNode<k,v>* low,fibNode<k,v>* high)
    {
        removeNodeFromRootList(low);

        consolidateAddHelper(high,low);
        low->mark=false;
    }
    void consolidate()
    {
        int degree,root_count=0;
        if(numberOfNode>1)
        {
            this->pointer_to_specific_degree.clear();
            fibNode<k,v>* current=this->maxKeyNode;
            fibNode<k,v>* tempMax=this->maxKeyNode;
            fibNode<k,v>* it=this->maxKeyNode;
            fibNode<k,v>* consolidateTarget;
            fibNode<k,v>* currentdegreeNode;
            do
            {
                root_count++;
                it=it->rightSibling;
            }
            while(it!=tempMax);

            for(int i=0; i<root_count; i++)
            {
                consolidateTarget=current;
                current=current->rightSibling;
                degree=consolidateTarget->degree;

                ///dynamically allocate pointer array to detect same dergree node
                while(true)
                {
                    while(degree>=this->pointer_to_specific_degree.size()) this->pointer_to_specific_degree.push_back(NULL);

                    if(this->pointer_to_specific_degree[degree]==NULL)
                    {
                        this->pointer_to_specific_degree[degree]=consolidateTarget;
                        break;
                    }

                    else
                    {
                        currentdegreeNode=this->pointer_to_specific_degree[degree];
                        if(consolidateTarget->key<currentdegreeNode->key) swap(consolidateTarget,currentdegreeNode);
                        if(consolidateTarget==currentdegreeNode) break;
                        fib_heap_link(currentdegreeNode,consolidateTarget);
                        this->pointer_to_specific_degree[degree]=NULL;
                        degree++;


                    }
                }
            }
            this->maxKeyNode=NULL;
            for(int i=0; i<this->pointer_to_specific_degree.size(); i++)
            {
                if(this->pointer_to_specific_degree[i]!=NULL)
                {
                    addToRootList(this->pointer_to_specific_degree[i]);
                }
            }
        }
    }


    fibNode<k,v>* read(int index)
    {
        if(isInitialized(index)) return a[index];
        else return NULL;
    }

    void write(int index,fibNode<k,v>* node)
    {
        if(isInitialized(index))
        {
            a[index]=node;
        }
        else
        {
            z++;
            a[index]=node;
            b[index]=z;
            c[z]=index;
        }
    }
    bool isInitialized(int index)
    {
        return (b[index]>=1 and b[index]<=z and c[b[index]]==index);
    }

public:

    fibonacciHeap()
    {
        this->maxKeyNode=NULL;
        this->numberOfNode=0;
        this->pointer_to_specific_degree= {};

    }
    int size()
    {
        return numberOfNode;
    }
    bool is_empty()
    {
        return numberOfNode==0;
    }

    void printHeap(ofstream & out)
    {
        int root_count=0;
        fibNode<k,v> *current=this->maxKeyNode;
        fibNode<k,v> *temp=this->maxKeyNode;
        do
        {
            root_count++;
            current=current->rightSibling;
        }
        while(current!=temp);
        current=this->maxKeyNode;
        for(int i=1; i<=root_count; i++)
        {
            cout<<"Tree "<<i<<": ";
            out<<"Tree "<<i<<": ";
            printHelp(current,out);
            current=current->rightSibling;
        }

    }
    fibNode<k,v>* extract_max()
    {
        fibNode<k,v> *ret=this->maxKeyNode;
        if(ret!=NULL)
        {
            fibNode<k,v> *childIt=ret->child;
            fibNode<k,v> *removeChild;
            for(int i=0; i<ret->degree; i++)
            {
                removeChild=childIt;
                childIt=childIt->rightSibling;
                addToRootList(removeChild);
            }
            removeNodeFromRootList(ret);
            this->numberOfNode--;
            if(this->numberOfNode==0) this->maxKeyNode=NULL;
            else
            {
                this->maxKeyNode=maxKeyNode->rightSibling;
                fibNode<k,v>* maxLeft=maxKeyNode->leftSibling;
                this->maxKeyNode->leftSibling=maxLeft;
                maxLeft->rightSibling=this->maxKeyNode;
                consolidate();
            }
        }
        return ret;

    }
    /*
    ref: cormen ; min fibonacciheap
    FIB-HEAP-INSERT.H; x/
    1 x:degree D 0
    2 x:p D NIL
    3 x:child D NIL
    4 x:mark D FALSE
    5 if H:min == NIL
    6 create a root list for H containing just x
    7 H:min D x
    8 else insert x into H’s root list
    9 if x:key < H:min:key
    10 H:min D x
    11 n D H:H: n C 1
    */
    bool _delete(int value)
    {
        if (read(value)==nullptr) return false;
        fibNode<k,v> *tempNode=read((int)value);
        increase_key(value,inf);
        extract_max();
        return true;
    }
    void insert(k key,int value)
    {
        /* if(findNodeByValue.find(value)!=findNodeByValue.end()) return;
         findNodeByValue[value]=new fibNode<k,v>(key,value);
         fibNode<k,v> *tempNode=findNodeByValue[value]; */


        if(read((int)value)!=NULL) return;
        write((int)value,new fibNode<k,v>(key,value));
        fibNode<k,v> *tempNode=read((int)value);
        fibNode<k,v> *maxNode=this->maxKeyNode;

        if(maxNode!=NULL)
        {
            fibNode<k,v> *maxLeft=maxNode->leftSibling;
            maxNode->leftSibling=tempNode;
            tempNode->rightSibling=maxNode;
            tempNode->leftSibling=maxLeft;
            maxLeft->rightSibling=tempNode;
        }
        if(maxNode==NULL or maxNode->key<tempNode->key)
        {
            this->maxKeyNode=tempNode;
        }
        this->numberOfNode++;
        ///cout<<"insert done\n";
    }

    bool increase_key(v value,k newKey)
    {
        /*if(findNodeByValue.find(value)==findNodeByValue.end()) return false;
        fibNode<k,v> *temp=this->findNodeByValue[value];*/

        if (read(value)==nullptr) return false;
        fibNode<k,v> *temp=read((int)value);

        if(temp->key>newKey) return false;
        ///update
        temp->key=newKey;
        if(temp->parent!=NULL and temp->key>temp->parent->key)
        {
            fibNode<k,v> *parent=temp->parent;
            cut(temp);
            cascadingCut(parent);
        }

        if(newKey>this->maxKeyNode->key)
        {
            this->maxKeyNode=temp;
        }
        return true;
    }
    fibNode<k,v>* find_max()
    {
        return this->maxKeyNode;
    }

    void meld(fibonacciHeap<k, v> &otherHeap)
    {
        // Merge the root lists
        fibNode<k, v> *maxLeft = this->maxKeyNode->leftSibling;
        fibNode<k, v> *otherMaxLeft = otherHeap.maxKeyNode->leftSibling;

        maxLeft->rightSibling = otherHeap.maxKeyNode;
        otherHeap.maxKeyNode->leftSibling = maxLeft;

        otherMaxLeft->rightSibling = this->maxKeyNode;
        this->maxKeyNode->leftSibling = otherMaxLeft;

        // Update the maximum node
        if (otherHeap.maxKeyNode->key > this->maxKeyNode->key)
        {
            this->maxKeyNode = otherHeap.maxKeyNode;
        }

        // Update the number of nodes
        this->numberOfNode += otherHeap.numberOfNode;

        // Clear the other heap
        otherHeap.maxKeyNode = NULL;
        otherHeap.numberOfNode = 0;
    }

};
/*
int32_t main()
{

    fibonacciHeap<int,int> fh;
    fibonacciHeap<int,int> fh1;
    fibonacciHeap<int,int> fh2;
    clock_t start=clock();
    for(int i=1;i<1000000;i++)
    {
        fh.insert(i,i);
    }
    cout<<"1000000 insertions: some unit of time  "<<(double)(clock()-start)<<endl;
    cout<<"per insertion: some unit of time  "<<(double)(clock()-start)/1000000<<endl;
    start=clock();
    for(int i=1;i<3000000;i++)
    {
        fh1.insert(i,i);
    }
    cout<<"\n\n";
    cout<<"3000000 insertions: some unit of time  "<<(double)(clock()-start)<<endl;
    cout<<"per insertion: some unit of time  "<<(double)(clock()-start)/3000000<<endl;



        start=clock();
    for(int i=1;i<5000000;i++)
    {
        fh2.insert(i,i);
    }
    cout<<"\n\n";
    cout<<"500000 insertions: some unit of time  "<<(double)(clock()-start)<<endl;
    cout<<"per insertion: some unit of time  "<<(double)(clock()-start)/5000000<<endl;
    //fh.printHeap();


       start=clock();
        for(int i=1;i<1000000;i++)
    {
        fh.extract_max();
    }
    cout<<"\n\n";
    cout<<"1000000 extract max : some unit of time  "<<(double)(clock()-start)<<endl;
    cout<<"per extract max: some unit of time  "<<(double)(clock()-start)/1000000<<endl;
    start=clock();
    for(int i=1;i<3000000;i++)
    {
        fh1.extract_max();
    }
    cout<<"\n\n";
    cout<<"3000000 extract max: some unit of time  "<<(double)(clock()-start)<<endl;
    cout<<"per extract max: some unit of time  "<<(double)(clock()-start)/3000000<<endl;



        start=clock();
    for(int i=1;i<5000000;i++)
    {
        fh2.extract_max();
    }
    cout<<"\n\n";
    cout<<"5000000 extract max: some unit of time  "<<(double)(clock()-start)<<endl;
    cout<<"per extract max: some unit of time  "<<(double)(clock()-start)/5000000<<endl;
    //fh.printHeap();
}*/






template<typename k,typename v>
class priorityQueue
{
public:
    fibonacciHeap<k,v>* make_heap()
    {
        return new fibonacciHeap<k,v>();
    }
    bool is_empty(fibonacciHeap<k,v> &heap)
    {
        return heap.is_empty();
    }
    void insert(fibonacciHeap<k,v> &heap,k key,v value)
    {
        heap.insert(key,value);
    }
    int size(fibonacciHeap<k,v>& heap)
    {
        return heap.size();
    }
    fibNode<k,v>*  extract_max(fibonacciHeap<k,v> &heap)
    {
        return heap.extract_max();
    }
    bool increase_key(fibonacciHeap<k,v> &heap,v value,k new_key)
    {
        return heap.increase_key(value,new_key);
    }
    bool _delete(fibonacciHeap<k,v> &heap,v value)
    {
        return heap._delete(value);
    }
    void meld(fibonacciHeap<k,v> &heap,fibonacciHeap<k,v> &heap2)
    {
        heap.meld(heap2);
    }
    fibNode<k,v> * find_max(fibonacciHeap<k,v> &heap)
    {
        return heap.find_max();
    }
    void print(fibonacciHeap<k,v> &heap,ofstream &out)
    {
        cout<<"\n\n";
        out<<"\n\n";
        heap.printHeap(out);
        cout<<"\n\n";
        out<<"\n\n";
    }
};

void output_test()
{
    ofstream out("code_output.txt");
    a=(fibNode<int,int>**)malloc(10000000*sizeof(fibNode<int,int>*));
    b=(int *)malloc(10000000*sizeof(int));
    c=(int *)malloc(10000000*sizeof(int));
    z=0;

    ///make heap test
    priorityQueue<int,int> pq;
    fibonacciHeap<int,int>*fh1=pq.make_heap();
    fibonacciHeap<int,int>*fh2=pq.make_heap();
    fibonacciHeap<int,int>*fh3=pq.make_heap();

    ///testing is_empty
    if(pq.is_empty(*fh1))
    {
        cout<<"empty\n";
        out<<"empty\n";
    }
    else
    {
        cout<<"not empty\n";
        out<<"not empty\n";
    }
    ///testing delete on empty heap;
    pq._delete(*fh1,10);



    ///inserting test

    ///inserting 1-10 to heap 1
    for(int i=1; i<=10; i++)
    {
        pq.insert(*fh1,i,i);
    }

    ///size test;
    cout<<pq.size(*fh1)<<"\n";
    out<<pq.size(*fh1)<<"\n";

    ///invalid insertion
    pq.insert(*fh1,10,8);
    cout<<pq.size(*fh1)<<"\n";
    out<<pq.size(*fh1)<<"\n";

    ///structure testing
    pq.print(*fh1,out);




    ///extract max test
    fibNode<int,int> *temp=pq.extract_max(*fh1);
    cout<<temp->key<<" "<<temp->value<<"\n";
    out<<temp->key<<" "<<temp->value<<"\n";
    pq.print(*fh1,out);


    ///find max test
    cout<<pq.find_max(*fh1)->key<<" "<<pq.find_max(*fh1)->value<<"\n";
    out<<pq.find_max(*fh1)->key<<" "<<pq.find_max(*fh1)->value<<"\n";





    for(int i=11; i<=20; i++)
    {
        pq.insert(*fh2,i,i);
    }
    pq.extract_max(*fh2);
    for(int i=21; i<=30; i++)
    {
        pq.insert(*fh3,i,i);
    }
    pq.extract_max(*fh3);
    pq.print(*fh2,out);
    pq.print(*fh3,out);
    pq.meld(*fh2,*fh3);
    pq.print(*fh2,out);
    pq.meld(*fh1,*fh2);
    pq.print(*fh1,out);

    ///increase key
    pq.increase_key(*fh1,25,20);
    pq.print(*fh1,out);

    pq.increase_key(*fh1,29,40);
    pq.print(*fh1,out);

    pq.increase_key(*fh1,23,50);
    pq.print(*fh1,out);
    ///cascade cut
    pq.increase_key(*fh1,24,100);
    pq.print(*fh1,out);

    ///delete cases
    pq._delete(*fh1,29);
    pq.print(*fh1,out);

    pq._delete(*fh1,2);
    pq.print(*fh1,out);







    free(a);
    free(b);
    free(c);

    out.close();

    ifstream codeoutput("code_output.txt");
    ifstream expected("expected_output.txt");

    string content1((istreambuf_iterator<char>(codeoutput)), istreambuf_iterator<char>());
    string content2((istreambuf_iterator<char>(expected)), istreambuf_iterator<char>());

    if(content1==content2) cout<<"passed!!!..."<<"\n";
    else cout<<"not passed!!!..."<<"\n";

    expected.close();
    codeoutput.close();



}

void speed_test()
{
    a=(fibNode<int,int>**)malloc(10000000*sizeof(fibNode<int,int>*));
    b=(int *)malloc(10000000*sizeof(int));
    c=(int *)malloc(10000000*sizeof(int));
    z=0;
    priorityQueue<int,int> pq;
    fibonacciHeap<int,int>*fh1=pq.make_heap();
    priority_queue<pair<int,int>> PQ;

    clock_t start=clock();
    for(int i=1; i<=1000000; i++)
    {
        pq.insert(*fh1,i,i);
    }
    cout<<"time needed for 1000000 insertion in our fibonacci heap: "<<(clock()-start)<<" unit time"<<endl;
    start=clock();
    for(int i=1; i<=1000000; i++)
    {
        PQ.push({i,i});
    }
    cout<<"time needed for 1000000 insertion in stl priority queue: "<<(clock()-start)<<" unit time"<<endl;
    start=clock();
    for(int i=1; i<=1000000; i++)
    {
        pq.extract_max(*fh1);
    }
    cout<<"time needed for 1000000 extract in our fibonacci heap: "<<(clock()-start)<<" unit time"<<endl;

    start=clock();

    for(int i=1; i<=1000000; i++)
    {
        PQ.pop();
    }
    cout<<"time needed for 1000000 extract in stl priority queue: "<<(clock()-start)<<" unit time"<<endl;

    free(a);
    free(b);
    free(c);


}


void test()
{
    cout<<"\n\ntesting output ... "<<"\n\n\n";
    output_test();

    cout<<"\n\ntesting speed ... "<<"\n\n\n";
    speed_test();
}




int32_t main()
{
    test();
}

