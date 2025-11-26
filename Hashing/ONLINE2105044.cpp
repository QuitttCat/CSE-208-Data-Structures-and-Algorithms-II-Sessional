#include<bits/stdc++.h>
using namespace std;

#define int long long int
#define MAXSIZE 500000
#define debug(i) cout<<i<<"eeecdi"<<"\n"
vector<string> randomWordContainer;


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
        this->tableSize=tableSize;
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

        if(collisionResolutionMethod==DOUBLE_HASHING)
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

         if(collisionResolutionMethod==DOUBLE_HASHING)
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

        if(collisionResolutionMethod==DOUBLE_HASHING)
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



int32_t main()
{

    HashTable ht(203,CUSTOM_PROBING,POLYNOMIAL,37,43,3);

    int n,l;
    cin>>n>>l;
    for(int i=0; i<n; i++)
    {
        string x;
        cin>>x;
        randomWordContainer.push_back(x);
        ht.insert(x,i);
    }
   int arr[n+1]={0};
    for(int i=0; i<n; i++)
    {
        string x= randomWordContainer[i];
        string y= randomWordContainer[i];

        for(int i=0; i<x.size(); i++)
        {
            if(x[i]=='A') x[i]='T';
            else if(x[i]=='T') x[i]='A';
            else if(x[i]=='C') x[i]='G';
            else if(x[i]=='G') x[i]='C';
        }
        reverse(x.begin(),x.end());
        int temp=ht.find(x);
          if(temp!=NOT_FOUND and x!=y and arr[i]!=1 and arr[temp]!=1)
            cout<<y<<" "<<x<<endl;
          arr[i]=1;
          arr[temp]=1;
    }

}


