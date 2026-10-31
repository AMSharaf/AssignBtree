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
    int b=(ord)/2;
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

    pair<type, Node*>* Insert(Node* x, type k)
    {
        int i = x->num_keys - 1;

        if (x->is_leaf)
        {
            for (int j=0;j<x->num_keys;j++) {
                if (x->keys[j] == k)return nullptr;
            }
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

            pair<type, Node*>* result = Insert(p->childs[i], k);

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

    void Insert(type k)
    {
        pair<type, Node*>* result = Insert(root, k);

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
        root->is_leaf=false;
        return SplitChild(s,0);

    }

    void Print()
    {
        if (root==nullptr) {
            cout<<"Empty Tree"<<endl;
            return;
        }
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
        cout<<endl;
    }
    void InOrder(Node* x) {
        InternalNode *xc=nullptr;
        if (!x->is_leaf)xc=(InternalNode*)x;
        for (int i=0;i<=x->num_keys;i++) {
            if (!x->is_leaf) {
                InOrder(xc->childs[i]);
            }
            if (i!=x->num_keys)
                cout<<x->keys[i]<<" ";
        }
    }
    void InOrder() {
        InOrder(root);
        cout<<endl;
    }
    // type Delete(type k,InternalNode*p=nullptr,Node *x) {
    //
    // }

    bool search(type k) {
         pair<Node*,bool> * result=search(root,k);
        if (result!=nullptr) {
            cout<<"it is here"<<endl;
            delete result;
            return true;
        }
        cout<<"it is not here"<<endl;
        delete result;
        return false;
    }
    pair<Node*,bool>* search(Node*x,type k) {
        if (x==nullptr) {
            return nullptr;
        }
        int i=x->num_keys-1;
        if (x->is_leaf)
        {
            while (i >= 0 && x->keys[i] > k)
            {
                i--;
            }

            if (x->keys[i] == k) {
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
    }


    void del(Node*x,InternalNode*p,type k,int cn){
        if (x==nullptr) return;
        int i=x->num_keys-1;
        if (x->is_leaf)
        {
            while (i >= 0 && x->keys[i] > k)
                i--;
            if (x->keys[i] == k) {
                for (int j=i; j<x->num_keys-1; j++) {
                    x->keys[j]=x->keys[j+1];
                }
                x->num_keys--;
                if (x->num_keys<b) {
                    return fixUnderflow(p,cn);
                }
                return ;
            }
            return ;
        }
        else
        {
            InternalNode* q = (InternalNode*)x;

            while (i >= 0 && q->keys[i] > k)
                i--;
            if (k<q->keys[i])
                 del(q->childs[i],q,k,i);
            else if (k>q->keys[i])
                 del(q->childs[i+1],q,k,i+1);
            else {
                type tmp =getSuc(q->childs[i+1]);
                q->keys[i]=tmp;
                 del(q->childs[i+1],q,tmp,i+1);
            }
            if (x->num_keys<b) {
                return fixUnderflow(p,cn);
            }
        }
        return ;

    }
    void del(type k) {

        del(root,nullptr,k,0);
        if (root->num_keys == 0) {
            if (!root->is_leaf) {
                Node *x=root->childs[0];
                InternalNode* p = new InternalNode(x->is_leaf);
                if (!x->is_leaf) {
                    InternalNode *xc=(InternalNode*)x;
                    for (int i=0;i<=x->num_keys;i++) {
                        p->childs[i]=xc->childs[i];
                    }
                }
                for (int i=0;i<x->num_keys;i++) {
                    p->keys[i]=x->keys[i];
                }

                p->num_keys=x->num_keys;
                delete x,root;
                root=p;
            }
            else {
                delete root;
                root=nullptr;
            }
        }
    }


    void merge(InternalNode *p,int cn) {
        Node *x=p->childs[cn];
        Node *y=nullptr;
        InternalNode *xc;
        InternalNode *yc;

        if (cn>0) {
            y=p->childs[cn-1];
            if (!x->is_leaf) {
                xc=(InternalNode*)x;
                yc=(InternalNode*)y;
            }

            if (!x->is_leaf) {
                for (int i=0;i<=x->num_keys;i++)
                    yc->childs[y->num_keys+i+1]=xc->childs[i];
            }
            y->keys[y->num_keys++]=p->keys[cn-1];
            for (int i=0;i<x->num_keys;i++) {
                y->keys[y->num_keys+i]=x->keys[i];
            }
            y->num_keys+=x->num_keys;
            delete x;
            for (int i=cn;i<p->num_keys;i++) {
                p->keys[i-1]=p->keys[i];
            }
            for (int i=cn;i<=p->num_keys;i++) {
                p->childs[i]=p->childs[i+1];
            }
            p->num_keys--;
        }
        else {
            y=p->childs[cn+1];
            if (!x->is_leaf) {
                xc=(InternalNode*)x;
                yc=(InternalNode*)y;
            }
            x->keys[x->num_keys++]=p->keys[cn];
            for (int i=0;i<y->num_keys;i++) {
                x->keys[x->num_keys+i]=y->keys[i];
            }
            if (!y->is_leaf) {
                for (int i=0;i<=y->num_keys;i++)
                    xc->childs[x->num_keys+i+1]=yc->childs[i];
            }
            x->num_keys+=y->num_keys;
            delete y;
            for (int i=cn;i<p->num_keys;i++) {
                p->keys[i]=p->keys[i+1];
            }
            for (int i=cn+1;i<=p->num_keys;i++) {
                p->childs[i]=p->childs[i+1];
            }
            p->num_keys--;

        }
    }

    void BorrowRight(InternalNode *p,int cn) {
        Node *x=p->childs[cn];
        Node *y=p->childs[cn+1];
        type tmp=p->keys[cn];
        x->keys[x->num_keys++]=tmp;
        p->keys[cn]=y->keys[0];
        for (int i=0;i<y->num_keys;i++) {
            y->keys[i]=y->keys[i+1];
        }
        y->num_keys--;
    }
    void BorrowLeft(InternalNode *p,int cn) {
        Node *x=p->childs[cn];
        Node *y=p->childs[cn-1];
        type tmp=y->keys[--y->num_keys];

        swap(p->keys[cn-1],tmp);
        for (int i=x->num_keys;i>=0;i--) {
            x->keys[i+1]=x->keys[i];
        }
        x->keys[0]=tmp;
        x->num_keys++;

    }

    type getSuc(Node *x) {
        if (x->is_leaf)return x->keys[0];
        InternalNode* p=(InternalNode*)x;
        return getSuc(p->childs[0]);
    }
    void fixUnderflow(InternalNode *p,int cn) {
        if (p==nullptr) return;
        if (cn!=0) {
            if (p->childs[cn-1]->num_keys>b) {
                BorrowLeft(p,cn);
                return;
            }
        }
        if (cn!=p->num_keys) {
            if (p->childs[cn+1]->num_keys>b) {
                BorrowRight(p,cn);
                return;
            }
        }
        merge(p,cn);
    }
};


int main() {
    BTree<char,4>T;
    T.Insert('f');
    T.Insert('s');
    T.Insert('q');
    T.Insert('k');
    T.Insert('c');
    T.Insert('l');
    T.Insert('h');
    T.Insert('t');
    T.Insert('v');
    T.Insert('w');
    T.Insert('m');
    T.Insert('r');
    T.Insert('n');
    T.Insert('p');
    T.Insert('a');
    T.Insert('b');
    T.Insert('x');
    T.Insert('y');
    T.Insert('d');
    T.Insert('z');
    T.Insert('e');
    T.Insert('g');
    T.Insert('g');
    T.Insert('g');

    T.Print();
    T.del('h');
    T.del('g');
    T.Print();
    T.del('n');
    T.Print();
    T.del('m');
    T.Print();
    T.del('s');
    T.Print();
    T.del('q');
    T.Print();
    T.del('t');
    T.Print();

    T.Print();
    T.del('b');
    T.del('c');
    T.del('d');
    T.Print();
    cout<<endl;
    T.del('f');
    T.del('l');
    T.del('p');
    T.Print();
    T.del('v');
    T.del('w');
    T.del('x');
    T.del('y');
    T.Print();
    T.del('a');
    T.del('e');
    T.del('r');
    T.del('s');
    T.del('t');
    T.Print();
    T.del('z');
    T.Print();
    T.del('k');
    T.Print();
    return 0;
}
