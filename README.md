# newick
Small code for parsing annoying newick trees recursively

Some c++ anc c code for parsing newick trees. This code works for arbitrary branches, it parses names and branchlengths if they exist. Please let me know if you find bugs.

char * newick =   "(A:0.1,B:0.2,(C:0.3,D:0.4)E:0.5)F;";
fprintf(stderr,"newick: %s\n",newick);
tokens *vec=mysplit3(strdup(newick));
  
node_t *nd = parse(vec);
node_t **lst = calloc(serial,sizeof(node_t*));
serialize(nd,lst);
  
fprintf(stderr,"serial: %d\n",serial);
for(int i=0;i<serial;i++)
  print_node(stderr,lst[i]);
