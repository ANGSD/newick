# newick
Small code for parsing annoying newick trees recursively

Some c++ anc c code for parsing newick trees. This code works for arbitrary branches, it parses names and branchlengths if they exist. Please let me know if you find bugs.

```c++
char * newick =   "(A:0.1,B:0.2,(C:0.3,D:0.4)E:0.5)F;";
//split into relevant tokens
tokens *vec=mysplit3(strdup(newick));
//parse everything
node_t *nd = parse(vec);
//now serialize and printout
node_t **lst = calloc(serial,sizeof(node_t*));
serialize(nd,lst);
  
for(int i=0;i<serial;i++)
  print_node(stderr,lst[i]);
````
