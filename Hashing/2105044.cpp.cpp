#include<bits/stdc++.h>
using namespace std;

#define int long long int
#define MAXSIZE 500000
#define debug(i) cout<<i<<"eeecdi"<<"\n"
vector<string> randomWordContainer;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());///seed on the basis of present time wrt chrono clock time

int constSeedArray[3]= {2105044,2102098,2106102};

enum status : int
{
    SOFT_DELETED=-699,
    ALIVE=-69,
    NOT_FOUND=-404,
};

enum collisionResolutionMethod : int
{
    SEPARATE_CHAINING = -263,
    DOUBLE_HASHING = -141,
    CUSTOM_PROBING = -916,
};

enum hashType: int
{
    POLYNOMIAL=-111,
    BITWISE=-222,
    AUXILARY=-333,
};



///The Sieve of Eratosthenes , running time n*ln(ln(sqrt(n))) + o(n)
vector<int>binary_search_on_prime_array;

void sieveOfEratosthenes(int n)
{
    binary_search_on_prime_array.clear();
    vector<bool> is_prime(n+10, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= n; i++)
    {
        if (is_prime[i])
        {
            for (int j = i * i; j <= n; j += i)
                is_prime[j] = false;
        }
    }

    for(int i=1; i<=n; i++)
    {
        if(is_prime[i])
        {
            binary_search_on_prime_array.push_back(i);
        }
    }
}

///lower bound return smallest number which is greater than or equal to a given number
int nearestPrime(int n)
{
    if(n<2) return 2;
    else return *lower_bound(binary_search_on_prime_array.begin(),binary_search_on_prime_array.end(),n);///log(n)
}






string randomWordGenerator(int minimumLength, int maximumLength)
{

    int length = uniform_int_distribution<int>(minimumLength,maximumLength)(rng);///https://cplusplus.com/reference/random/uniform_int_distribution/#google_vignette

    string randomWord="";
    string characters = "abcdefghijklmnopqrstuvwxyz";

    for(int j=0; j<length; j++)
    {
        int index = uniform_int_distribution<int>(0,25)(rng);
        randomWord+=characters[index];
    }

    return randomWord;
}






vector<string> generateRandomWords(int n,int minL,int maxL)
{
    unordered_set<string> s;
    vector<string> v;
    //int count=0;
    while(s.size()!=n)
    {
        s.insert(randomWordGenerator(minL,maxL));
        //count++;
        //cout<<count<<"\n";
    }
    for(auto x:s) v.push_back(x);
    //for(auto x:s) cout<<x<<"\n";
    return v;
}





class Cell
{
public:
    string key;
    int value;
    Cell*next;
    int status;
    Cell (string key, int value,Cell*next=NULL,int status=ALIVE)
    {

        this->key=key;
        this->value=value;
        this->next=next;
        this->status=status;

    }
};



class HashTable
{
    vector<Cell*>table;
    int collisionResolutionMethod;
    int hashType;
    int c1,c2;
    int tableSize;
    int numberOfKey;
    int numberOfCollision;
    int numberOfProbe;

    int insertCount;
    int removeCount;
    int allowedChainLen;
    int presentMaxChainLen;
public:


    HashTable(int tableSize,int collisionResolutionMethod,int hashType,int c1,int c2,int allowedChainLen)
    {
        this->tableSize=nearestPrime(tableSize);
        table.resize(this->tableSize);
        this->collisionResolutionMethod=collisionResolutionMethod;
        this->hashType=hashType;
        this->c1=c1;
        this->c2=c2;
        this->numberOfKey=0;
        this->numberOfCollision=0;
        this->numberOfProbe=0;

        this->insertCount=0;
        this->removeCount=0;
        this->allowedChainLen=allowedChainLen;
        this->presentMaxChainLen=0;
    }
    ~HashTable()
    {
        for(auto x:table)
        {
            while(x!=NULL)
            {
                Cell* temp=x;
                x=x->next;
                delete temp;
            }

        }
        table.clear();
    }

    int getTableSize()
    {
        return tableSize;
    }

    int getKeyCount()
    {
        return numberOfKey;
    }

    int getNumberOfCollision()
    {
        return numberOfCollision;
    }

    int getNumberOfProbe()
    {
        return numberOfProbe;
    }
    void setTableSize(int newSize)
    {
        tableSize = newSize;
    }

    void setKeyCount(int newCount)
    {
        numberOfKey = newCount;
    }

    void setNumberOfCollision(int newCollision)
    {
        numberOfCollision = newCollision;
    }

    void setNumberOfProbe(int newProbe)
    {
        numberOfProbe = newProbe;
    }
    ///polynomial
    int Hash1(string s,int m,int p=31)
    {

        int hash_value = 0;
        int pow = 1;
        for (char c : s)
        {
            hash_value = (hash_value + (c - 'a' + 1) * pow) % m;
            pow = (pow * p) % m;
        }
        return hash_value%m;
    }
    ///bitwise
    int Hash2(const string s,int m, int p=999999937)
    {
        int n = s.length();
        int hash_value = p;
        for (int i = 0; i < n; i++)
        {
            hash_value ^= (hash_value << 6) + (hash_value >> 11) + s[i];
        }
        hash_value &= 0x7FFFFFFF;///making positive
        return hash_value % m;
    }
    /// auxilary hash
    int Hash3(string s)
    {
        int n = s.length();
        return (67-((67+999999929*(s[0] - 'a') +s[n-1] - 'a'))%67);
    }
    void insert(string key,int value)
    {
        int index;
        if(hashType==POLYNOMIAL)
        {
            index=Hash1(key,tableSize);
        }
        else if(hashType==BITWISE)
        {
            index=Hash2(key,tableSize);
        }
        if(collisionResolutionMethod==SEPARATE_CHAINING)
        {
            Cell* current=table[index];
            int length=0;
            if(current!=NULL)numberOfCollision++;
            while(current!=NULL)
            {
               /// numberOfCollision++;
                if(current->key==key)
                {
                    current->value=value;
                    return;
                }
                current=current->next;
                length++;
            }
            presentMaxChainLen=max(length,presentMaxChainLen);
            table[index]=new Cell(key,value,table[index]);
            numberOfKey++;
            insertCount++;
            if(insertCount==100)
            {
                this->insertCount=0;


                if(presentMaxChainLen>allowedChainLen)
                {
                    for(int i=0; i<0.1*numberOfKey; i++)
                    {
                        find(randomWordContainer[uniform_int_distribution<int>(0,numberOfKey)(rng)]);
                    }
                    cout<<"\n\nBefore Rehashing:\n";
                    cout<<"avarage probe count : "<<numberOfProbe*(1.0)/(0.1*numberOfKey)<<endl;
                    cout<<"load factor : "<<numberOfKey*1.0/tableSize<<endl;
                    cout<<"maximum chain length : "<<presentMaxChainLen<<"\n\n";

                    this->numberOfCollision=0;
                    this->presentMaxChainLen=0;


                    int newSize=nearestPrime(2*this->tableSize);
                    vector<Cell*> newTable(newSize, nullptr);
                    for (int i = 0; i < tableSize; ++i)
                    {
                        Cell* current = table[i];
                        int l=0;
                        if(current!=NULL)numberOfCollision++;
                        while (current != nullptr)
                        {
                            ///numberOfCollision++;
                            Cell* nextNode = current->next;
                            int newIndex;
                            if(hashType==POLYNOMIAL)
                            {
                                newIndex=Hash1(current->key,newSize);
                            }
                            else if(hashType==BITWISE)
                            {
                                newIndex=Hash2(current->key,newSize);
                            }
                            current->next = newTable[newIndex];
                            newTable[newIndex] = current;
                            current = nextNode;
                            l++;
                        }
                        presentMaxChainLen=max(l,presentMaxChainLen);
                    }
                    tableSize = newSize;
                    table = newTable;
                    numberOfProbe=0;

                    for(int i=0; i<0.1*numberOfKey; i++)
                    {
                        find(randomWordContainer[uniform_int_distribution<int>(0,numberOfKey)(rng)]);
                    }
                    cout<<"\n\nAfter Rehashing:\n";
                    cout<<"avarage probe count : "<<numberOfProbe*(1.0)/(0.1*numberOfKey)<<endl;
                    cout<<"load factor : "<<numberOfKey*1.0/tableSize<<endl;
                    cout<<"maximum chain length : "<<presentMaxChainLen<<"\n\n";

                }
            }

        }
        else if(collisionResolutionMethod==DOUBLE_HASHING)
        {

            if(numberOfKey<tableSize)
            {
                int auxHash=Hash3(key)%tableSize;

                if(table[index]!=NULL) numberOfCollision++;
                while(table[index]!=NULL and table[index]->status!=SOFT_DELETED)
                {
                    ///numberOfCollision++;
                    if(table[index]->key==key)
                    {
                        table[index]->value=value;
                        return;
                    }
                    index=(index+auxHash)%tableSize;
                }

                table[index]=new Cell(key,value,table[index]);
                numberOfKey++;


            }

        }
        else if(collisionResolutionMethod==CUSTOM_PROBING)
        {
            if(numberOfKey<tableSize)
            {
                int auxHash=Hash3(key)%tableSize;
                int it=0;
                int balance=1;

                if(table[index]!=NULL) numberOfCollision++;
                while(table[index]!=NULL  and it<tableSize and table[index]->status!=SOFT_DELETED)
                {

                   /// numberOfCollision++;
                    if(table[index]->key==key)
                    {
                        table[index]->value=value;
                        return;
                    }

                    index=(index+c1*auxHash+c2*balance)%tableSize;
                    balance+=2;
                    //index=(index+auxHash)%tableSize;
                    ///mathematics   1,4,9,16 -> 1+3,1+3+5,1+3+5+7
                    it++;
                }

                table[index]=new Cell(key,value,table[index]);
                numberOfKey++;


            }

        }
        numberOfProbe=0;
    }


    int find(string key)
    {
        int index;
        if(hashType==POLYNOMIAL)
        {
            index=Hash1(key,tableSize);
        }
        else if(hashType==BITWISE)
        {
            index=Hash2(key,tableSize);
        }
        if(collisionResolutionMethod==SEPARATE_CHAINING)
        {
            Cell* current=table[index];

            while(current!=NULL)
            {
                numberOfProbe++;
                if(current->key==key)
                {
                    return current->value;
                }
                current=current->next;
            }
            return NOT_FOUND;
        }
        else if(collisionResolutionMethod==DOUBLE_HASHING)
        {

            int auxHash=Hash3(key);
            int it=0;
            while(table[index]!=NULL and table[index]->status!=SOFT_DELETED)
            {
                numberOfProbe++;
                if(table[index]->key==key)
                {
                    return table[index]->value;
                }
                index=(index+auxHash)%tableSize;
                it++;
            }
            return NOT_FOUND;
        }
        else if(collisionResolutionMethod==CUSTOM_PROBING)
        {
            int auxHash=Hash3(key);
            int it=0;
            int balance=1;
            while(table[index]!=NULL and it<tableSize and table[index]->status!=SOFT_DELETED)
            {
                numberOfProbe++;
                if(table[index]->key==key)
                {

                    return table[index]->value;
                }
                index=(index+this->c1*auxHash+this->c2*balance)%tableSize;
                balance+=2;

                ///mathematics   1,4,9,16 -> 1+3,1+3+5,1+3+5+7
                it++;
            }
            return NOT_FOUND;
        }
        return NOT_FOUND;
    }

    void remove(string key)
    {
        if(find(key)==NOT_FOUND) return;
        int index;
        if(hashType==POLYNOMIAL)
        {
            index=Hash1(key,tableSize);
        }
        else if(hashType==BITWISE)
        {
            index=Hash2(key,tableSize);
        }
        if(collisionResolutionMethod==SEPARATE_CHAINING)
        {
            Cell* current = table[index];
            Cell* prev = nullptr;
            int chainLength = 0; // Variable to track chain length
            while (current != nullptr)
            {
                if (current->key == key)
                {
                    if (prev == nullptr) // If current node is the head of the list
                        table[index] = current->next;
                    else
                        prev->next = current->next;

                    delete current;
                    numberOfKey--;
                    removeCount++;
                    break; // Exit the loop once the key is deleted
                }
                prev = current;
                current = current->next;
                chainLength++;
            }
            presentMaxChainLen = max(presentMaxChainLen, chainLength); // Update max chain length after deletion
            if(removeCount==100)
            {
                this->removeCount=0;

                 numberOfProbe=0;
                if(presentMaxChainLen<0.8*allowedChainLen)
                {
                    ///debug(1);
                    for(int i=0; i<0.1*numberOfKey; i++)
                    {
                        find(randomWordContainer[uniform_int_distribution<int>(0,numberOfKey)(rng)]);
                    }
                    cout<<"\n\nBefore Rehashing:\n";
                    cout<<"avarage probe count : "<<numberOfProbe*(1.0)/(0.1*numberOfKey)<<endl;
                    cout<<"load factor : "<<numberOfKey*1.0/tableSize<<endl;
                    cout<<"maximum chain length : "<<presentMaxChainLen<<"\n\n";

                    this->numberOfCollision=0;
                    this->presentMaxChainLen=0;


                    int newSize=nearestPrime(this->tableSize*1.0/2);
                    cout<<"table size : "<<tableSize<<" new size : "<<newSize<<'\n';

                    ///if(newSize<tableSize) return;
                    vector<Cell*> newTable(newSize, nullptr);
                    for (int i = 0; i < tableSize; ++i)
                    {
                        Cell* current = table[i];
                        int l=0;
                         if(current!=NULL) numberOfCollision++;
                        while (current != nullptr)
                        {
                           /// numberOfCollision++;
                            Cell* nextNode = current->next;
                            int newIndex;
                            if(hashType==POLYNOMIAL)
                            {
                                newIndex=Hash1(current->key,newSize);
                            }
                            else if(hashType==BITWISE)
                            {
                                newIndex=Hash2(current->key,newSize);
                            }
                            current->next = newTable[newIndex];
                            newTable[newIndex] = current;
                            current = nextNode;
                            l++;
                        }
                        presentMaxChainLen=max(l,presentMaxChainLen);
                    }
                    tableSize = newSize;
                    table = newTable;
                    numberOfProbe=0;

                    for(int i=0; i<0.1*numberOfKey; i++)
                    {
                        find(randomWordContainer[uniform_int_distribution<int>(0,numberOfKey)(rng)]);
                    }
                    cout<<"\n\nAfter Rehashing:\n";
                    cout<<"avarage probe count : "<<numberOfProbe*(1.0)/(0.1*numberOfKey)<<endl;
                    cout<<"load factor : "<<numberOfKey*1.0/tableSize<<endl;
                    cout<<"maximum chain length : "<<presentMaxChainLen<<"\n\n";

                }
            }


        }
        else if(collisionResolutionMethod==DOUBLE_HASHING)
        {

            int auxHash=Hash3(key);
            int it=0;
            while(table[index]!=NULL and table[index]->status!=SOFT_DELETED)
            {
                if(table[index]->key==key)
                {
                    table[index]->status=SOFT_DELETED;
                    numberOfKey--;
                }
                index=(index+auxHash)%tableSize;
                it++;
            }

        }
        else if(collisionResolutionMethod==CUSTOM_PROBING)
        {
            int auxHash=Hash3(key);
            int it=0;
            int balance=1;
            while(table[index]!=NULL and it<tableSize and table[index]->status!=SOFT_DELETED)
            {

                if(table[index]->key==key)
                {
                    table[index]->status=SOFT_DELETED;
                    numberOfKey--;
                }
                index=(index+this->c1*auxHash+this->c2*balance)%tableSize;
                balance+=2;

                ///mathematics   1,4,9,16 -> 1+3,1+3+5,1+3+5+7
                it++;
            }

        }
        numberOfProbe=0;
    }

};



void printTableHeader(ofstream& outputFile)
{
    outputFile << setw(23) << left << " Collision Resolution |";
    outputFile << setw(26) << left << "           Hash1" << "|";
    outputFile << setw(26) << left << "            Hash2" << "|" << endl;
    outputFile << "                      " << setfill('-') << setw(60) << "-" << setfill(' ') << endl;
    outputFile << setw(23) << left << "       Method" << "|";
    outputFile << setw(13) << right << " Collisions" << "|";
    outputFile << setw(13) << right << "  Avg Probes" << "|";
    outputFile << setw(13) << right << "  Collisions" << "|";
    outputFile << setw(13) << right << "  Avg Probes" << "|" << endl;
    outputFile << "----------------------------------------------------------------------------------" << endl;
}

void printTableRow(ofstream& outputFile, const string& method, int coll1, double avgProbe1, int coll2, double avgProbe2)
{
    outputFile << setw(23) << left << method << "|";
    outputFile << setw(13) << right << coll1 << "|";
    outputFile << setw(13) << right << fixed << setprecision(4) << avgProbe1 << "|";
    outputFile << setw(13) << right << coll2 << "|";
    outputFile << setw(13) << right << fixed << setprecision(4) << avgProbe2 << "|" << endl;
}

vector<string> generateRandomWords2(int n,int minL,int maxL)
{
    HashTable ht(20000,CUSTOM_PROBING,POLYNOMIAL,37,43,3);
    vector<string> v;
    while(ht.getKeyCount()!=n)
    {
        string temp=randomWordGenerator(minL,maxL);
        if(ht.find(temp)==NOT_FOUND)
        {
            ht.insert(temp,1111);
            v.push_back(temp);
        }

    }
    return v;
}

void uniqueness(ofstream & out,int tsize)
{
        HashTable ht(tsize,CUSTOM_PROBING,POLYNOMIAL,37,43,3);
    set<int> s;
    for(int i=0; i<10000; i++)
    {
        s.insert(ht.Hash1(randomWordContainer[i],nearestPrime((int)tsize)));
    }
    out<<"\n\n Uniqueness of hash1 when load factor "<<1.0*10000/tsize<<" : "<<s.size()*100.0/10000<<"%\n";

    HashTable ht2(tsize,CUSTOM_PROBING,BITWISE,37,43,3);
    s.clear();
    for(int i=0; i<10000; i++)
    {
        s.insert(ht.Hash2(randomWordContainer[i],nearestPrime((int)tsize)));
    }
    out<<"Uniqueness of hash2 when load factor "<<1.0*10000/tsize<<" : "<<s.size()*100.0/10000<<"%\n";
}


void report(ofstream & out)
{
    randomWordContainer=generateRandomWords2(10000,5,10);
    int N_bar[3]= {5000,10000,200000};
    int hashArray[2]= {POLYNOMIAL,BITWISE};
    int collisionResolutionMethodArray[3]= {SEPARATE_CHAINING,DOUBLE_HASHING,CUSTOM_PROBING};

    cout<<"------------------------------REPORT------------------------------------------------------------------"<<endl;
    for(int i=0; i<=2; i++)
    {
        cout<<"\n\nN' = "<<N_bar[i]<<"\n\n";
        out<<"\n\nN' = "<<N_bar[i]<<"\n\n";
        printTableHeader(out);
        for(int j=0; j<=2; j++)
        {
            ///data formating
            string x;
            if(j==0) x="separate chaining";
            else if(j==1) x="double hashing";
            else x="custom probing";
            HashTable ht(N_bar[i],collisionResolutionMethodArray[j],BITWISE,37,43,3);
            for(int k=0; k<10000; k++)
            {
                ht.insert(randomWordContainer[k],k+1);
            }
            for(int k=0; k<1000; k++)
            {
                ht.find(randomWordContainer[uniform_int_distribution<int>(0,min((int)10000,N_bar[i]))(rng)]);
            }

            HashTable ht2(N_bar[i],collisionResolutionMethodArray[j],POLYNOMIAL,37,43,3);
            for(int k=0; k<10000; k++)
            {
                ht2.insert(randomWordContainer[k],k+1);
            }
            for(int k=0; k<1000; k++)
            {
                ht2.find(randomWordContainer[uniform_int_distribution<int>(0,min((int)10000,N_bar[i]))(rng)]);
            }
            printTableRow(out,x,ht.getNumberOfCollision(),1.0*ht.getNumberOfProbe()/1000,ht2.getNumberOfCollision(),1.0*ht2.getNumberOfProbe()/1000);

        }
    }
    uniqueness(out,20000);
    uniqueness(out,15000);
    uniqueness(out,12000);
    uniqueness(out,10000);
    uniqueness(out,8000);
    uniqueness(out,5000);

}




void removeCheck1()
{
    cout<<"\n\n functionalities of remove: custom probing"<<"\n";
    HashTable ht(20000,CUSTOM_PROBING,POLYNOMIAL,37,43,3);
    for(int k=0; k<10000; k++)
    {
        ht.insert(randomWordContainer[k],k+1);
    }
    cout<<"\n\n before delete, size: 10000\n";
    for(int k=0; k<1000; k++)
    {
        ht.remove(randomWordContainer[uniform_int_distribution<int>(0,5000)(rng)]);
    }
    cout<<"\n\n after delete, size: "<<ht.getKeyCount()<<"\n";
}
void removeCheck2()
{
    cout<<"\n\n functionalities of remove: double hashing"<<"\n";
    HashTable ht(20000,DOUBLE_HASHING,BITWISE,37,43,3);

    for(int k=0; k<10000; k++)
    {
        ht.insert(randomWordContainer[k],k+1);
    }
    cout<<"\n\n before delete, size: 10000\n";
    for(int k=0; k<1000; k++)
    {
        ht.remove(randomWordContainer[uniform_int_distribution<int>(0,5000)(rng)]);
    }
    cout<<"\n\n after delete, size: "<<ht.getKeyCount()<<"\n";
}
void removeCheck3()
{
    cout<<"\n\n functionalities of insert: separate chainging along with rehashing"<<"\n";
    HashTable ht(1000,SEPARATE_CHAINING,POLYNOMIAL,37,43,20);

    for(int k=0; k<10000; k++)
    {
        ht.insert(randomWordContainer[k],k+1);
    }
    cout<<ht.getKeyCount()<<endl;
    cout<<"\n\n functionalities of remove: separate chainging along with rehashing"<<"\n";
    for(int k=0; k<10000; k++)
    {
        ht.remove(randomWordContainer[k]);

    }
    cout<<ht.getKeyCount()<<endl;

}

void removeCheck()
{
    removeCheck1();
    removeCheck2();
    removeCheck3();
}



int32_t main()
{
    ofstream out("Report.txt");
    sieveOfEratosthenes(MAXSIZE);
    rng.seed(2105044);///report
    ///rng.seed(2105098);
    report(out);
    removeCheck();
    out.close();

    return 0;
}
