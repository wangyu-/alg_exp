#include <stdio.h>

//from https://blog.csdn.net/xiaowang627/article/details/51339959

const int RED=0;
const int BLACK=1;

struct Node{
    Node(int key){
        this->key=key;
    }
    int key=-1;
    int color=1;
    Node* parent=0;
    Node* left=0;
    Node* right=0;
};

struct Tree{
    Tree(){
    }
    Node* root=0;
    Node* nil=0;
};


void LEFT_ROTATE(Tree* &T, Node* x){
    Node* y;

    y=x->right;
    x->right=y->left;
    if(y->left!=T->nil)
        y->left->parent=x;
    y->parent=x->parent;
    if(x->parent==T->nil)
        T->root=y;
    else if(x==x->parent->left)
        x->parent->left=y;
    else
        x->parent->right=y;
    y->left=x;
    x->parent=y;
}
//节点右旋
void RIGHT_ROTATE(Tree* &T, Node* y){
    Node *x;

    x=y->left;
    y->left=x->right;
    if(x->right!=T->nil)
        x->right->parent=y;
    x->parent=y->parent;
    if(y->parent==T->nil)
        T->root=x;
    else if(y==y->parent->left)
        y->parent->left=x;
    else
        y->parent->right=x;
    x->right=y;
    y->parent=x;
}


//红黑树调整
void RB_INSERT_FIXUP(Tree* &T, Node* z){
    Node* y;

    while(z->parent->color==RED){
        if(z->parent==z->parent->parent->left){     //z节点父节点为其祖父节点的左孩子
            y=z->parent->parent->right;
            if(y->color==RED){                      //case1
                z->parent->color=BLACK;
                y->color=BLACK;
                z->parent->parent->color=RED;
                z=z->parent->parent;
            }
            else{
                if(z==z->parent->right){            //case2
                    z=z->parent;
                    LEFT_ROTATE(T,z);
                }
                z->parent->color=BLACK;             //case3
                z->parent->parent->color=RED;
                RIGHT_ROTATE(T,z->parent->parent);
            }
        }
        else{									    //z节点父节点为其祖父节点的右孩子
            y=z->parent->parent->left;
            if(y->color==RED){						//case 1
                z->parent->color=BLACK;
                y->color=BLACK;
                z->parent->parent->color=RED;
                z=z->parent->parent;
            }
            else{
                if(z==z->parent->left){            //case2
                    z=z->parent;
                    RIGHT_ROTATE(T,z);
                }
                z->parent->color=BLACK;             //case3
                z->parent->parent->color=RED;
                LEFT_ROTATE(T,z->parent->parent);
            }
        }
    }
    T->root->color=BLACK;
}

//插入节点
void RB_INSERT(Tree* &T,Node* z){
    Node* y=T->nil;
    Node* x=T->root;
    while(x!=T->nil){
        y=x;
        if(z->key < x->key)
            x=x->left;
        else
            x=x->right;
    }
    z->parent=y;
    if(y==T->nil)//插入第一个元素
        T->root=z;
    else if(z->key < y->key)
        y->left=z;
    else
        y->right=z;
    z->left=T->nil;
    z->right=T->nil;
    z->color=RED;
    RB_INSERT_FIXUP(T,z);
}
void dfs_prt(Tree *T,Node *node)
{
    if(node==T->nil) return ;
    dfs_prt(T,node->left);
    printf("<%d>",node->key);
    dfs_prt(T,node->right);
}
void dfs_prt2(Tree *T,Node *node)
{
    if(node==T->nil) return ;
    printf("<%d>",node->key);
    dfs_prt2(T,node->left);
    dfs_prt2(T,node->right);
}

#define REDD   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"


//from https://stackoverflow.com/questions/801740/c-how-to-draw-a-binary-tree-to-the-console
int max_row=0;
int _print_t(Tree *T,Node *tree, int is_left, int offset, int depth, char s[20][255])
{
    char b[20];
    int width = 2;
    int is_red=0;

    if (!tree) return 0;

    if(tree==T->nil)
        sprintf(b, " N");
    else
    {
        sprintf(b, "%02d", tree->key);
        if(b[0]=='0') b[0]=' ';
        if(tree->color==RED) is_red=1;
    }

    int left  = _print_t(T,tree->left,  1, offset,                depth + 1, s);
    int right = _print_t(T,tree->right, 0, offset + left + width, depth + 1, s);

    if(2*depth>max_row) max_row= 2*depth;

    for (int i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];

    if(is_red)
    {
        s[2 * depth][offset + left - 1] = 'R';
        s[2 * depth][offset + left + width] = 'r';
    }

    if (depth && is_left) {

        for (int i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset + left + width + right + width/2] = '+';

    } else if (depth && !is_left) {

        for (int i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset - width/2 - 1] = '+';
    }

    return left + width + right;
}

void print_t(Tree *T,Node *tree)
{
    max_row=0;
    const int max_height=100;
    char s[max_height][255];
    for (int i = 0; i < max_height; i++)
        sprintf(s[i], "%80s", " ");

    _print_t(T,tree, 0, 0, 0, s);

    //for (int i = 0; i < 20; i++)
      //  printf("%s\n", s[i]);
    for (int i = 0; i < max_row+3; i++)
        for(int j=0;;j++)
        {
            if(s[i][j]==0) {printf("\n");break;}
            if(s[i][j]=='R') printf("%s ",REDD);
            else if(s[i][j]=='r') printf("%s ",RESET);
            else if(s[i][j]=='+'||s[i][j]=='-') printf("%s%c%s",YEL,s[i][j],RESET);
            else printf("%c",s[i][j]);
        }
}

int run(int arr[],int len)
{
    static int cnt=0;
    printf("=============case %d===============\n",cnt++);
    Tree *T;
    //Node* z[MAXSIZE];
    T = new Tree();
    T->nil = new Node(0);
    T->nil->color = BLACK;
    T->root = T->nil;
    for (int i = 0; i < len; i++)
    {
        Node *z = new Node(arr[i]);
        RB_INSERT(T, z);
        //print_t(T,T->root);
    }
    print_t(T,T->root);
}

int main(){

    {
        int arr[]={};
        int len = sizeof(arr) / sizeof(arr[0]);
        run(arr,len);
    }
    {
        int arr[]={12};
        int len = sizeof(arr) / sizeof(arr[0]);
        run(arr,len);
    }

    {
        int arr[] = {12, 1, 9, 2, 0, 11, 7, 19, 4, 15, 18, 5, 14, 13, 10, 16, 6, 3, 8, 17};
        int len = sizeof(arr) / sizeof(arr[0]);
        run(arr,len);
    }

    {
        int arr[]={41,38,31,12,19,8};
        int len = sizeof(arr) / sizeof(arr[0]);
        run(arr,len);
    }
    return 0;
}