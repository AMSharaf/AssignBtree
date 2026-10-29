#include <iostream>
#include <algorithm>
#include <queue>
#include <stack>
using namespace std;




template<typename type,int ord>
class BTree
{
private:
    class Node
    {
    public:
        bool is_leaf;
        int num_keys=0;
        // type* keys=nullptr;
        type keys[ord+1];
        explicit Node(bool is_leaf=false) : is_leaf(is_leaf)
        {
            //keys = new type[ord];
        }

        ~Node()
        {
           // delete[] keys;
        }

    };

    struct LeafNode:public Node
    {
        explicit LeafNode(): Node(true)
        {}
    };
    class InternalNode:public Node
    {
    public:
        //Node **childs=new Node*[ord+1];
        Node * childs[ord+2];
        explicit InternalNode(bool isLeaf=false) : Node(isLeaf)
        {
            for (int i=0;i<ord+2;i++) childs[i]=nullptr;
        }
    };


    InternalNode *root=nullptr;
    int b=ord/2;
public:
    BTree()
    {
        root=new InternalNode(true);
    }
    pair<type, Node*>* Split(Node* y)
    {
        int mid = y->num_keys / 2;
        Node* z = nullptr;

        if (y->is_leaf != true)
        {
            z = new InternalNode();
            InternalNode* p = (InternalNode*)z;
            InternalNode* q = (InternalNode*)y;

            int rightKeys = y->num_keys - mid - 1;

            for (int j = 0; j <= rightKeys; j++)
                p->childs[j] = q->childs[mid + 1 + j];
        }
        else
            z = new LeafNode();

        int rightKeys = y->num_keys - mid - 1;

        for (int j = 0; j < rightKeys; j++)
            z->keys[j] = y->keys[mid + 1 + j];

        z->num_keys = rightKeys;

        type promotedKey = y->keys[mid];

        y->num_keys = mid;

        return new pair<type, Node*>(promotedKey, z);
    }

    pair<type, Node*>* InsertPlus(Node* x, type k)
    {
        int i = x->num_keys - 1;

        if (x->is_leaf)
        {
            while (i >= 0 && x->keys[i] > k)
            {
                x->keys[i + 1] = x->keys[i];
                i--;
            }

            x->keys[i + 1] = k;
            x->num_keys++;
        }
        else
        {
            InternalNode* p = (InternalNode*)x;

            while (i >= 0 && p->keys[i] > k)
                i--;

            i++;

            pair<type, Node*>* result = InsertPlus(p->childs[i], k);

            if (result != nullptr)
            {
                for (int j = p->num_keys; j > i; j--)
                    p->keys[j] = p->keys[j - 1];

                for (int j = p->num_keys + 1; j > i + 1; j--)
                    p->childs[j] = p->childs[j - 1];

                p->keys[i] = result->first;
                p->childs[i + 1] = result->second;
                p->num_keys++;

                delete result;
            }
        }

        if (x->num_keys > ord)
            return Split(x);

        return nullptr;
    }

    void InsertPlus(type k)
    {
        pair<type, Node*>* result = InsertPlus(root, k);

        if (result != nullptr)
        {
            InternalNode* s = new InternalNode();

            s->childs[0] = root;
            s->childs[1] = result->second;
            s->keys[0] = result->first;
            s->num_keys = 1;

            root = s;

            delete result;
        }
    }


    Node* SplitChild(InternalNode* x,int i)
    {
        if (x==nullptr||x->childs[i]==nullptr)return nullptr;
        Node *y=x->childs[i];
        Node *z=nullptr;
        if (y->is_leaf!=true)
        {
            z=new InternalNode();
            InternalNode *p=(InternalNode*)z;
            InternalNode *q=(InternalNode*)y;
            for (int j=0;j<=b;j++)
                p->childs[j]=q->childs[b+j+1];
        }
        else
            z=new LeafNode();
        for (int j=0;j<b;j++)
            z->keys[j]=y->keys[b+j+1];
        z->num_keys=y->num_keys=b;
        for (int j=x->num_keys;j>i;j--) {
            x->keys[j]=x->keys[j-1];
            x->childs[j+1]=x->childs[j];
        }
        x->childs[i+1]=z;
        x->keys[i]=y->keys[b];
        x->num_keys++;
        return x;
    }
    Node* SplitRoot() {
        InternalNode *s=new InternalNode();
        s->childs[0]=root;
        root=s;
        return SplitChild(s,0);

    }

    void Insert(type k) {
        Insert(root,k);
    }
    void Insert(Node* x,type k) {
        if (x->num_keys==ord) {
            x=SplitRoot();
        }
        int i=x->num_keys-1;
        if (x->is_leaf) {
            while (i>=0&&x->keys[i]>k) {
                x->keys[i+1]=x->keys[i];
                i--;
            }
            x->keys[++i]=k;
            x->num_keys++;
        }
        else {
            InternalNode *p=(InternalNode*)x;
            while (i>=0&&p->keys[i]>k) i--;
            i++;
            if (p->childs[i]->num_keys==ord) {
                SplitChild(p,i);
                if (p->keys[i]<k)i++;
            }
            Insert(p->childs[i],k);
        }

    }


    void Print() {
        queue<Node*>s;
        s.push(root);
        while (!s.empty()) {
            cout<<"[";
            InternalNode *p=(InternalNode*)s.front();
            if (!p->is_leaf||p==root) {
                for (int i=0;i<p->num_keys;i++) {
                    cout<<p->keys[i]<<' ';
                    s.push(p->childs[i]);
                }
                s.push(p->childs[p->num_keys]);
            }
            else {
                for (int i=0;i<s.front()->num_keys;i++)cout<<s.front()->keys[i]<<' ';
            }
            s.pop();
            cout<<"]"<<endl;
        }
    }
    void Print2()
    {
        queue<Node*> s;
        s.push(root);

        while (!s.empty())
        {
            int levelSize = s.size();

            while (levelSize > 0)
            {
                Node* current = s.front();
                s.pop();

                cout << "[";

                for (int i = 0; i < current->num_keys; i++)
                {
                    if (i > 0)
                        cout << " ";

                    cout << current->keys[i];
                }

                cout << "] ";

                if (!current->is_leaf)
                {
                    InternalNode* p = (InternalNode*)current;

                    for (int i = 0; i <= current->num_keys; i++)
                    {
                        if (p->childs[i] != nullptr)
                            s.push(p->childs[i]);
                    }
                }

                levelSize--;
            }

            cout << endl;
        }
    }

    // type Delete(type k,InternalNode*p=nullptr,Node *x) {
    //
    // }

    bool search(type k) {
         pair<Node*,bool> * result=search(root,k);
        if (result!=nullptr) {
            cout<<"it is here"<<endl;
            return true;
        }
        cout<<"it is not here"<<endl;
        return false;
    }
    pair<Node*,bool>* search(Node*x,type k) {
        int i=x->num_keys-1;
        if (x->is_leaf)
        {
            while (i >= 0 && x->keys[i] > k)
            {
                i--;
            }

            if (x->keys[i+1] == k) {
                pair<Node*,bool>* result = new pair<Node*,bool>(x,true);
                return result;
            }
            return nullptr;
        }
        else
        {
            InternalNode* p = (InternalNode*)x;

            while (i >= 0 && p->keys[i] > k)
                i--;
            pair<Node*,bool>* result =new pair<Node*,bool>(p,true);

            if (p->keys[i] == k)return result;
            return search(p->childs[i+1],k);
        }
        return nullptr;

    }


    void del(Node*x,InternalNode*p,type k,int cn){
        int i=x->num_keys-1;
        if (x->is_leaf)
        {
            while (i >= 0 && x->keys[i] > k)
                i--;
            if (x->keys[i] == k) {
                if (x->num_keys>b) {
                    for (int j=i; j<x->num_keys-1; j++) {
                        x->keys[j]=x->keys[j+1];
                    }
                    x->num_keys--;
                    return;
                }
                else if (x->num_keys==b) {
                    borrow(p,cn,k);
                    del(p,nullptr,k,0);
                    return;
                }

                return ;
            }
            return ;
        }
        else
        {
            InternalNode* p = (InternalNode*)x;

            while (i >= 0 && p->keys[i] > k)
                i--;
            if (k<p->keys[i])
                return del(p->childs[i],p,k,i);
            else if (k>p->keys[i])
                return del(p->childs[i+1],p,k,i+1);
            else {
                type tmp =getSuc(p->childs[i+1]);
                p->keys[i]=tmp;
                return del(p->childs[i+1],p,tmp,i+1);
            }
        }
        return ;

    }
    void del(type k) {
        del(root,nullptr,k,0);
    }


    void merge(InternalNode *p,int cn,int k) {
        Node *x=p->childs[cn];
        Node *y=nullptr;
        if (cn!=p->num_keys&&k<p->childs[cn+1]->num_keys==b){

            y=p->childs[cn+1];
            x->keys[x->num_keys]=p->keys[cn];
            x->num_keys++;
            for (int j=0;j<x->num_keys;j++) {
                x->keys[x->num_keys+j]=y->keys[j];
            }
            x->num_keys+=y->num_keys;
            swap(p->childs[cn],p->childs[cn+1]);
            delete y;
        }
        else if (cn>0&&p->childs[cn-1]->num_keys==b){
            y=p->childs[cn-1];
            y->keys[y->num_keys]=p->keys[cn-1];
            y->num_keys++;
            for (int j=0;j<y->num_keys;j++) {
                y->keys[y->num_keys+j]=x->keys[j];
            }
            y->num_keys+=x->num_keys;
            delete x;
        }
        for (int j=cn-1;j<=p->num_keys;j++)
            p->keys[j]=p->keys[j+1];
        for (int j=cn;j<=p->num_keys+1;j++)
            p->childs[j]=p->childs[j+1];
        p->childs[p->num_keys]=nullptr;
        p->num_keys--;
    }

    void borrow(InternalNode *p,int cn,int k) {
        Node *x=p->childs[cn];
        Node *y=nullptr;
        type tmp;
        if (cn!=0) {//left
            y=p->childs[cn-1];
            if (y->num_keys>b) {
                y->num_keys--;
                tmp=y->keys[y->num_keys];
                swap(tmp,p->keys[cn-1]);
                //shift(x,0,x->num_keys,true);
                x->num_keys;
                x->keys[0]=tmp;
                return;
            }
        }
        if (cn!=p->num_keys) {//right
            y=p->childs[cn+1];
            if (y->num_keys>b) {
                tmp=y->keys[0];
                shift(y,0,y->num_keys,true);
                y->num_keys--;
                swap(tmp,p->keys[cn]);
                x->keys[x->num_keys]=tmp;
                x->num_keys++;
                return;
            }
        }
        merge(p,cn,k);

    }

    void shift(Node*x,int l,int r,bool isRight) {
        if (isRight) {
            for (int i=l;i<r;i++) {
                x->keys[i]=x->keys[i+1];
            }
        }
        else {
            for (int i=r;i>l;i--) {
                x->keys[i-1]=x->keys[i];
            }
        }

    }
    type getSuc(Node *x) {
        if (x->is_leaf)return x->keys[0];
        InternalNode* p=(InternalNode*)x;
        return getSuc(p->childs[0]);
    }
};


int main() {
    BTree<char,5>T;
    T.InsertPlus('f');
    T.InsertPlus('s');
    T.InsertPlus('q');
    T.InsertPlus('k');
    T.InsertPlus('c');
    T.InsertPlus('l');
    T.InsertPlus('h');
    T.InsertPlus('t');
    T.InsertPlus('v');
    T.InsertPlus('w');
    T.InsertPlus('m');
    T.InsertPlus('r');
    T.InsertPlus('n');
    T.InsertPlus('p');
    T.InsertPlus('a');
    T.InsertPlus('b');
    T.InsertPlus('x');
    T.InsertPlus('y');
    T.InsertPlus('d');
    T.InsertPlus('z');
    T.InsertPlus('e');
    T.InsertPlus('g');


    T.Print2();
    T.del('q');
    T.Print2();

    // T.del('h');
    // T.del('g');
    // T.Print2();
    // T.del('n');
    // T.del('m');
    //
    // T.Print2();
    // T.del('b');
    // T.del('c');
    // T.del('d');
    // T.Print2();
    // cout<<endl;
    // T.del('f');
    // T.del('l');
    // T.del('p');
    // T.Print2();
    // T.del('v');
    // T.del('w');
    // T.del('x');
    // T.del('y');
    // T.Print2();
    // T.del('a');
    // T.del('e');
    // T.del('r');
    // T.del('s');
    // T.del('t');
    // T.Print2();
    // T.del('z');
    // T.Print2();



}

