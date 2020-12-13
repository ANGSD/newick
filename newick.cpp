#include <cstdio>
#include <cstring>
const char *newick = "(A,(C,D)E)F;";//"(A:0.1,(C:0.3,D:0.4):0.5);";

int at =0;

typedef struct node_t{
  char *name;
  double blen;//branch length
  node_t *up;
  node_t *left;
  node_t *right;
}node_t;

node_t *alloc(){
  node_t *nd = new node_t;
  nd->name =NULL;
  nd->blen=-1;
  nd->up=nd->left=nd->right=NULL;
  return nd;
}

node_t* recursive(char *str){
  char buf[strlen(str)];
  int orig = 0;
  int at = 0;
  node_t *nd = alloc();
}




int main(){
  

  int maxnode = 100;

  node_t *root = recursive(newick);
  return 0;
}
