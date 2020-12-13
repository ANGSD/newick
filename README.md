# newick
Small code for parsing annoying newick trees recursively

Some c++ anc c code for parsing newick trees. This code works for arbitrary branches, it parses names and branchlengths if they exist. Please let me know if you find bugs.

```c++
char * newick =   "(A:0.1,B:0.2,(C:0.3,D:0.4)E:0.5)F;";
fprint(stderr,"newick: %s",newick);
//split into relevant tokens
tokens *vec=mysplit3(strdup(newick));
//parse everything
node_t *nd = parse(vec);
//now serialize and printout
fprintf(stderr,"serial: %d\n",serial);
node_t **lst = calloc(serial,sizeof(node_t*));
serialize(nd,lst);
  
for(int i=0;i<serial;i++)
  print_node(stderr,lst[i]);
````

This prints out
```
newick: (A:0.1,B:0.2,(C:0.3,D:0.4)E:0.5)F;
serial: 6
----------
nd->serial: 0 this: 0x7f93a0405a70
nd->name: F nd->blen: -1.000000 nd->up:0x0
----------
----------
nd->serial: 1 this: 0x7f93a0405ab0
nd->name: A nd->blen: 0.100000 nd->up:0x7f93a0405a70
----------
----------
nd->serial: 2 this: 0x7f93a0405b00
nd->name: B nd->blen: 0.200000 nd->up:0x7f93a0405a70
----------
----------
nd->serial: 3 this: 0x7f93a0405b50
nd->name: E nd->blen: 0.500000 nd->up:0x7f93a0405a70
----------
----------
nd->serial: 4 this: 0x7f93a0405b90
nd->name: C nd->blen: 0.300000 nd->up:0x7f93a0405b50
----------
----------
nd->serial: 5 this: 0x7f93a0405be0
nd->name: D nd->blen: 0.400000 nd->up:0x7f93a0405b50

