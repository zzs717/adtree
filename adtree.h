void* return_adtree(
   int Rmin,
   int* data,
   int datasize,
   int row,
   int col
);

void makecontab(
   void* tree,
   int* variables,
   int nvariables,
   int* contab,
   int contabsize
);

void printTable(void *adtree, int* contab, int contabsize, int *variables, int nvariables);

void del_adtree(
   void *adtree
);
