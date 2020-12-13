#include <cstdio>
#include <cstring>
#include <string>
#include <vector>


typedef struct node_t{
  char *name;
  double blen;//branch length
  node_t *up;
  int nbranches;
  int mbranches;
  node_t **branches;
  int serial;
}node_t;

int serial =0;

node_t *alloc(){

  node_t *nd = new node_t;
  nd->name =NULL;
  nd->blen=-1;
  nd->mbranches = 2;
  nd->nbranches = 0;
  nd->up =NULL;
  nd->branches =(node_t **) calloc(nd->mbranches,sizeof(node_t*));
  nd->serial = serial++;
  return nd;
}

std::vector<std::string> mysplit2(char *str){
  std::vector<std::string> vec;
  int i=0;
  char buf[strlen(str)];//THIS IS ENOUGH
  memset(buf,0,strlen(str));
  int at=0;
  for(;i<strlen(str);i++){
    if(str[i]=='\t'||str[i]==' '||str[i]==';')
      continue;
      
    if(str[i]=='('||str[i]==')'||str[i]==':'||str[i]==','){
      if(strlen(buf)>0){
	vec.push_back(buf);
	memset(buf,0,strlen(str));
	at=0;
      }
      vec.push_back(std::string()+str[i]);
      continue;
    }else{
      buf[at++]=str[i];
    }
  }
  if(strlen(buf)>0){
    vec.push_back(buf);
    memset(buf,0,strlen(str));
    at=0;
  }
  
  return vec;
}

int atter=0;//dag

//production rule: prod := (prod,prod)nam:len
node_t *parse(std::vector<std::string> &vec){

  node_t *nd = alloc();
  int seri = nd->serial;
  if(atter>=vec.size())
    return nd;
  //catch leaflike case
  if(vec[atter]!="("){
    if(vec[atter]!=":"){
      nd->name = strdup(vec[atter].c_str());
      atter++;
    }
    if(vec[atter]==":"){
      nd->blen = atof(vec[atter+1].c_str());
      atter+=2;
    }
    return nd;
  }

  //catch recursive cases;
  if(vec[atter]=="("){
    while(1){
      atter++;
      node_t *anode =parse(vec);
      anode->up = nd;
      if(nd->nbranches==nd->mbranches){
	nd->mbranches = 2*nd->mbranches;
	nd->branches =(node_t**) realloc(nd->branches,sizeof(node_t*)*nd->mbranches);
	fprintf(stderr,"reallocing\n");
      }
      nd->branches[nd->nbranches++] = anode;
      if(vec[atter]==",")
	continue;
      else
	break;
    }
  }
  if(vec[atter]==")"){
    atter++;
    if(vec[atter]==")")//catch the case when there is no name or branch length
      return nd;
  }

  if(vec[atter]!=":"){
    nd->name = strdup(vec[atter].c_str());
    atter++;
  }
  if(vec[atter]==":"){
    nd->blen = atof(vec[atter+1].c_str());
    atter+=2;
  }
  return nd;
}



void print_node(FILE *fp,node_t *nd){
  fprintf(fp,"----------\nnd->serial: %d this: %p\n",nd->serial,nd);
  //  fprintf(fp,"nd->left: %p nd->right: %p nd->up: %p\n",nd->left,nd->right,nd->up);
  fprintf(fp,"nd->name: %s nd->blen: %f nd->up:%p\n----------\n",nd->name,nd->blen,nd->up);
  
}

void serialize(node_t *nd,node_t **lst){
  assert(nd);
  lst[nd->serial] = nd;
  for(int i=0;i<nd->nbranches;i++)
    if(nd->branches[i])
      serialize(nd->branches[i],lst);
  
}


int main(){
  //char * newick = "(A:0.1,B:0.2,(C:0.3,D:0.4):0.5)ialt;";//"(A:0.1,(C:0.3,D:0.4):0.5);";
  //char * newick =   "(ding,(be,ca:1.1)dd)hans:22.2";
  //char * newick = "(ding,(be,(a,ca:1.1))):22.2;";
  //char * newick = "(be,(a,ca:1.1))banan:22.2;";
  //char * newick =   "(A:0.1,B:0.2,(C:0.3,D:0.4):0.5);";
  char * newick =   "(A:0.1,B:0.2,(C:0.3,D:0.4)E:0.5)F;";
  fprintf(stderr,"newick: %s\n",newick);
  std::vector<std::string> vec=mysplit2(strdup(newick));
  for(int i=0;0&&i<vec.size();i++)
    fprintf(stderr,"%d) %s\n",i,vec[i].c_str());

  
  node_t *nd = parse(vec);
  node_t **lst = new node_t*[serial];
  serialize(nd,lst);
  
  fprintf(stderr,"serial: %d\n",serial);
  for(int i=0;i<serial;i++)
    print_node(stderr,lst[i]);

  
  return 0;
}
