#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node_t{
  char *name;
  double blen;//branch length
  struct node_t *up;
  int nbranches;
  int mbranches;
  struct node_t **branches;
  int serial;
}node_t;

typedef struct {
  int l;
  int m;
  char **toks;
}tokens;

void push_back_str(tokens *ret,char *buf){
  assert(ret);
  //fprintf(stderr,"ret->l: %d ret->m: %d buf:%s\n",ret->l,ret->m,buf);
  if(ret->l==ret->m){
    ret->m = 2*ret->m;
    ret->toks = realloc(ret->toks,ret->m*sizeof(char*));
  }
  ret->toks[ret->l++] = strdup(buf);
  memset(buf,0,strlen(buf));
}
void push_back_char(tokens *ret,char tmp){
  // fprintf(stderr,"tmp: %c\n",tmp);
  char buf[2] = {tmp,'\0'};
  push_back_str(ret,buf);
}


int serial =0;

node_t *alloc(){

  node_t *nd = (node_t*) calloc(1,sizeof(node_t));
  nd->name =NULL;
  nd->blen=-1;
  nd->mbranches = 2;
  nd->nbranches = 0;
  nd->up =NULL;
  nd->branches =(node_t **) calloc(nd->mbranches,sizeof(node_t*));
  nd->serial = serial++;
  return nd;
}

tokens* mysplit3(char *str){
  tokens *ret =(tokens*) calloc(1,sizeof(tokens));
  ret->l = 0;
  ret->m = 4; //4 elements per default
  ret->toks =(char **) calloc(ret->m,sizeof(char*));
  
  int i=0;
  char buf[strlen(str)];//THIS IS ENOUGH
  memset(buf,0,strlen(str));
  int at=0;
  for(;i<strlen(str);i++){
    if(str[i]=='\t'||str[i]==' '||str[i]==';')
      continue;
      
    if(str[i]=='('||str[i]==')'||str[i]==':'||str[i]==','){
      if(strlen(buf)>0){//plug everything in buf
	push_back_str(ret,buf);
	at=0;
      }
      //now str[i] is ():, so plug that in also
      push_back_char(ret,str[i]);
      continue;
    }else{
      buf[at++]=str[i];
    }
  }
  if(strlen(buf)>0){
    push_back_str(ret,buf);
    at=0;
  }
  
  return ret;
}

int atter=0;//dag //haha

//production rule: prod := (prod,prod)nam:len
node_t *parse(tokens *vec){
  node_t *nd = alloc();
  int seri = nd->serial;
  if(atter>=vec->l)
    return nd;
  //catch leaflike case, where we dont do recursion
  if(strcmp(vec->toks[atter],"(")!=0){
    if(strcmp(vec->toks[atter],":")!=0){
      nd->name = strdup(vec->toks[atter]);
      atter++;
    }
    if(strcmp(vec->toks[atter],":")==0){
      nd->blen = atof(vec->toks[atter+1]);
      atter+=2;
    }
    return nd;
  }

  //catch recursive cases;
  if(strcmp(vec->toks[atter],"(")==0){
    while(1){
      atter++;
      node_t *anode =parse(vec);
      anode->up = nd;
      if(nd->nbranches==nd->mbranches){
	nd->mbranches = 2*nd->mbranches;
	nd->branches =(node_t**) realloc(nd->branches,sizeof(node_t*)*nd->mbranches);
	//¯	fprintf(stderr,"reallocing\n");
      }
      nd->branches[nd->nbranches++] = anode;
      if(strcmp(vec->toks[atter],",")==0)
	continue;
      else
	break;
    }
  }
  if(strcmp(vec->toks[atter],")")==0){
    atter++;
    if(strcmp(vec->toks[atter],")")==0)//catch the case when there is no name or branch length
      return nd;
  }

  if(strcmp(vec->toks[atter],":")!=0){
    nd->name = strdup(vec->toks[atter]);
    atter++;
  }
  if(atter>=vec->l)
    return nd;
  if(strcmp(vec->toks[atter],":")==0){
    nd->blen = atof(vec->toks[atter+1]);
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
  //  char * newick =   "(ding,(be,ca:1.1)dd)hans:22.2";
  //char * newick =   "(ding,hejsa,(be,ca:1.1)dd)hans:22.2";
  //char * newick = "(ding,(be,(a,ca:1.1))):22.2;";
  //char * newick = "(be,(a,ca:1.1))banan:22.2;";
  //char * newick =   "(A:0.1,B:0.2,(C:0.3,D:0.4):0.5);";
  char * newick =   "(A:0.1,B:0.2,(C:0.3,D:0.4)E:0.5)F;";
  fprintf(stderr,"newick: %s\n",newick);
  tokens *vec=mysplit3(strdup(newick));
  for(int i=0;0&&i<vec->l;i++)
    fprintf(stderr,"%d) %s\n",i,vec->toks[i]);

  
  node_t *nd = parse(vec);
  node_t **lst = calloc(serial,sizeof(node_t*));
  serialize(nd,lst);
  
  fprintf(stderr,"serial: %d\n",serial);
  for(int i=0;i<serial;i++)
    print_node(stderr,lst[i]);

  
  return 0;
}
