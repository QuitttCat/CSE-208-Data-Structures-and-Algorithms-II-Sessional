#include "red_black_tree.hpp"


int main()
{
    MAP<int,string> rbtree;
    ifstream in("input.txt");
    ofstream out("output.txt");
    string read;
    while(getline(in,read))
    {
        istringstream iss(read);
        string s;
        iss>>s;
        if(s=="Clr")
        {
            if(rbtree.clear())
            {
                out<<"successful\n";
                cout<<"successful\n";


            }
            else
            {
                out<<"unsuccessful\n";
                cout<<"unsuccessful\n";
            }

        }
        else if(s=="Em")
        {
            if(rbtree.empty())
            {
                out<<"yes\n";
                cout<<"yes\n";

            }


            else
            {
                out<<"no\n";
                cout<<"no\n";


            }
        }
        else if(s=="I")
        {
            int key;
            string ss;
            iss>>key>>ss;
            rbtree.insert(key,ss);
            rbtree.output(out);
        }
        else if(s=="F")
        {
            int key;
            string ss;
            iss>>key>>ss;
            if(rbtree.search(key))
            {
                out<<key<<" found\n";
                cout<<key<<" found\n";

            }

            else

            {
                out<<key<<" not found\n";
                cout<<key<<" not found\n";

            }
        }
        else if(s=="S")
        {
            out<<rbtree.size()<<"\n";
            cout<<rbtree.size()<<"\n";

        }
        else if(s=="E")
        {
            int key;
            iss>>key;
            int prev=rbtree.size();
            rbtree.delete_(key,out);
            int next=rbtree.size();
            if(next<prev)rbtree.output(out);
            else cout<<key<<" not found\n";
        }
        else if(s=="Itr")
        {
            rbtree.iterate(out);
        }
    }
    in.close();
    out.close();

    return 0;

}
