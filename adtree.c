#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static int adnum=1;
static int varynum=0;

typedef enum BOOLEN{false, true}bool;
typedef struct Link{
  int value;
  struct Link * next;
}link;
struct adNode
{
   int count;
   int father;
   int *leaflist;
   struct varynode *children;
};
typedef struct adNode ADnode;
struct varynode
{
   int variable;
   int mcv;
   struct adNode **children;        //one for the pointer of the ADTREE for the specialisation
};
typedef struct varynode VaryNode;

struct a_tree
{
   ADnode* root;
   int **data;
   int row;
   int col;
   int rmin;
   link ** value;
   int *num;
};
typedef struct a_tree the_tree;


link * getValueList(int var, int** a, int row);
bool isContain(link *t, int value);
int get_mcv(int** a, int var, link* value, bool judge[],int* num, int row);
void buildVaryNd(int** data, int var, link** Var, VaryNode* varynode, bool judge[], int* num, int row, int rmin, int col);
void buildADnode(int** data, ADnode* adnode, bool judge[], link* Var[], int* num, int row, int rmin, int col);
int getaddr(int *m,int size,int *addr,int *num);
void getTable(int **data, link **Var, int* variables, int orisize, int *oldvariables, int *newvariables, int size, int newsize, ADnode *adnode,int* num,int** tablelist, int index,int* address,int rmin,int col);
void freeTree(ADnode *adnode,int *num,int row,int col,int rmin);
void printTable(void *adtree, int* contab, int contabsize, int *variables, int nvariables);
void loop(int* contab, int *variables, int contabsize, int nvariables, int* num, link **Var, int var, int* value, int order);

void* return_adtree(int Rmin, int* data, int datasize, int row, int col){

  the_tree* adtree = malloc(sizeof(the_tree));

  ADnode* root = malloc(sizeof(ADnode));
  root->count=row;
  root->father=0;

  int** Data=(int **) malloc(row * sizeof(int *));
  for(int i=0;i<row;i++){
    Data[i]=(int *) malloc(col * sizeof(int));
    int x=col*i;
    for(int j=0;j<col;j++){
      Data[i][j]=data[x+j];
    }
  }

  bool judge[row];
  for( int i = 0; i < row; i++){judge[i] = true;}

  link **Var=(link **) malloc(col * sizeof(link *));
  int *num=(int *) malloc(col * sizeof(int));
  for(int i=0;i<col;i++){
    Var[i] =(link*)malloc(sizeof(link));
    Var[i] = getValueList(i+1,Data,row);
    link *temp = Var[i];
    num[i]=0;
    while(temp->next!=NULL){
      num[i]++;
      temp=temp->next;
    }
  }
  buildADnode(Data,root,judge,Var,num,row,Rmin,col);

  adtree->root=root;
  adtree->data=Data;
  adtree->row=row;
  adtree->col=col;
  adtree->rmin=Rmin;
  adtree->value=Var;
  adtree->num=num;
  
  //printf("The number of ADnode in the tree:%d\n",adnum);
  //printf("The number of Varynode in the tree:%d\n",varynum);
  adnum=1;
  varynum=0;

  return(void*)adtree;
}

void makecontab(void* tree, int* variables,int nvariables,int* contab,int contabsize){
	
  the_tree* thetree = (the_tree*)tree;
  if(thetree->root->children==NULL){printf("The tree has been released\n");return;}
  int contabsize2 = 1;
  for(int i=0;i<nvariables;i++){
    contabsize2*=thetree->num[variables[i]-1];
  }

  int tablelistsize=nvariables;
  int** tablelist=(int **) malloc(tablelistsize * sizeof(int *));
  for(int i=0;i<tablelistsize;i++){
    tablelist[i]=(int *) malloc(contabsize2 * sizeof(int));
    for(int j=0;j<contabsize2;j++){
      tablelist[i][j]=0;
    }
  }

  int* address=(int *) malloc(thetree->col * sizeof(int));
  for(int i=0;i<thetree->col;i++){address[i]=0;}

  int index=tableindex(variables,nvariables,variables,nvariables);
  getTable(thetree->data,thetree->value,variables,nvariables,variables,variables,nvariables,nvariables,thetree->root,thetree->num,tablelist,index,address,thetree->rmin,thetree->col);
  for(int i=0;i<contabsize2;i++){
    contab[i]=tablelist[index][i];
  }
  
  //printTable(contab,variables,contabsize,nvariables, thetree->value, thetree->num);

  for(int i=0;i<tablelistsize;i++){
    free(tablelist[i]);
  }
  free(tablelist);
}

void del_adtree(void *adtree){
  the_tree* thetree = (the_tree*)adtree;
  freeTree(thetree->root,thetree->num,thetree->row,thetree->col,thetree->rmin);

  for(int i=0;i<thetree->row;i++){
    free(thetree->data[i]);
  }
  free(thetree->data);

  for(int i=0;i<thetree->col;i++){
    free(thetree->value[i]);
  }
  free(thetree->value);

  free(thetree->num);
  free(adtree);
}

link * getValueList(int var, int **a, int row){
  link * head=(link*)malloc(sizeof(link));
  head->next=NULL;
  link * temp=head;
    for (int i=0; i<row; i++) {
        if(isContain(head,a[i][var-1])==false){
          link *b=(link*)malloc(sizeof(link));
          b->value=a[i][var-1];
          b->next=NULL;
          temp->next=b;
          temp=temp->next;
        }
    }
    temp->next=NULL;
    return head;
}
bool isContain(link *t, int value){
  if(t->next==NULL){return false;}
  link *temp=t->next;
  while(temp!=NULL){
    if(temp->value==value){return true;}
    temp=temp->next;
  }
  return false;
}
int get_mcv(int **a, int var, link *value, bool judge[],int num[], int row){
  int childcount[num[var]];
  for(int i=0;i<num[var];i++){
    childcount[i]=0;
    for(int j=0;j<row;j++){
      if(judge[j] == true && a[j][var]==value->value){
        childcount[i]++;
      }
    }
    value=value->next;
  }
  int mcv=0;
  int max=0;
  for(int i=0;i<num[var];i++){
    if(childcount[i]>max){
      max=childcount[i];
      mcv=i;
    }
  }
  return mcv;
}
void buildVaryNd(int **data, int var, link **Var, VaryNode *varynode, bool judge[],int* num, int row, int rmin, int col){
  varynode->variable =var+1;
  varynode->children = (ADnode **) malloc(num[var] * sizeof(ADnode *));
  link *temp = Var[var]->next;
  int mcv=get_mcv(data,var,temp,judge,num,row);
  varynode->mcv=mcv;
  for(int i=0;i<num[var];i++){
    if(i==mcv){
      varynode->children[i]=NULL;
    }
    else{
      bool tempJudge[row];
      int count=0;
      for( int j = 0; j < row; j++){
        if(judge[j] == true && data[j][var]==temp->value){
            tempJudge[j]=true;
            count++;
        }
        else{tempJudge[j]=false;}
      }
      if(count>0){
		adnum++;
        varynode->children[i] = (ADnode *) malloc(sizeof(ADnode));
        varynode->children[i]->father=var+1;
        varynode->children[i]->count=count;
        buildADnode(data,varynode->children[i],tempJudge,Var,num,row,rmin,col);
      }
      else{
        varynode->children[i] = NULL;
      }
    }
    temp=temp->next;
  }
}
void buildADnode(int **data, ADnode *adnode, bool judge[], link** Var, int* num, int row, int rmin, int col){
  if(adnode->count<rmin){
    adnode->leaflist = (int *) malloc(adnode->count * sizeof(int));
    int j=0;
    for(int i=0;i<row;i++){
      if(judge[i]==true){
        adnode->leaflist[j] = i;
        j++;
      }
    }
    return;
  }
  if(adnode->father<col){
    adnode->children = (VaryNode *) malloc((col - adnode->father) * sizeof(VaryNode));
  }
  for( int i = adnode->father; i < col; i++){
	varynum++;
    buildVaryNd(data,i,Var,(adnode->children) + (i - adnode->father),judge,num,row,rmin,col);
  }
}

int getaddr(int *m,int size,int *addr,int *num){
  int a=0;
  for(int i=0;i<size;i++){
    int addr1=addr[m[i]-1];
    for(int j=i+1;j<size;j++){
      addr1*=num[m[j]-1];
    }
    a+=addr1;
  }
  return a;
}
void getTable(int **data, link **Var,int * variables, int orisize, int *oldvariables, int *newvariables, int size, int newsize, ADnode *adnode,int* num,int** tablelist, int index,int* address,int rmin,int col){
  if(adnode==NULL){return;}
  if(size==0){
    tablelist[0][0]=adnode->count;
    return;
  }
  if(adnode->count<rmin){
    for(int i=0;i<adnode->count;i++){
      for(int j=0;j<size;j++){
        link* find=Var[oldvariables[j]-1]->next;
        address[oldvariables[j]-1]=0;
        while(find->value!=data[adnode->leaflist[i]][oldvariables[j]-1]){
          find=find->next;
          address[oldvariables[j]-1]+=1;
        }
      }
      tablelist[index][getaddr(oldvariables,size,address,num)]+=1;
    }
  }
  else{
    if(newsize==1){
      int sum=0;
      VaryNode varynode=adnode->children[newvariables[0]-1-adnode->father];
      for(int i=0; i<num[newvariables[0]-1]; i++){
        if(i!=varynode.mcv&&varynode.children[i]!=NULL){
          address[varynode.variable-1]=i;
          tablelist[index][getaddr(oldvariables,size,address,num)]=varynode.children[i]->count;
          sum+=varynode.children[i]->count;
        }
      }
      address[varynode.variable-1]=varynode.mcv;

      tablelist[index][getaddr(oldvariables,size,address,num)]=adnode->count-sum;
    }
    if(newsize>1){
      int mmm[newsize-1];
      for(int i=0;i<newsize-1;i++){
        mmm[i]=newvariables[i+1];
      }
      VaryNode varynode=adnode->children[newvariables[0]-1-adnode->father];
      for(int i=0; i<num[newvariables[0]-1]; i++){
        if(i!=varynode.mcv&&varynode.children[i]!=NULL){
          address[varynode.variable-1]=i;
          getTable(data,Var,variables,orisize,oldvariables,mmm,size,newsize-1,varynode.children[i],num,tablelist,index,address,rmin,col);
        }
      }

      address[varynode.variable-1]=varynode.mcv;

      for(int i=varynode.variable;i<col;i++){address[i]=0;}

      int begin1 = getaddr(oldvariables,size,address,num);
      address[varynode.variable-1]+=1;
      int end1 = getaddr(oldvariables,size,address,num);
      address[varynode.variable-1]=0;
      int begin2 = getaddr(oldvariables,size,address,num);
      address[varynode.variable-1]=num[varynode.variable-1];
      int end2 = getaddr(oldvariables,size,address,num);
      address[varynode.variable-1]=varynode.mcv;

      int newindex=newsize-2;

      getTable(data,Var,variables,orisize,mmm,mmm,newsize-1,newsize-1,adnode,num,tablelist,newindex,address,rmin,col);

      int sum[end1-begin1];
      for(int i=0;i<end1-begin1;i++){
        sum[i]=0;
        for(int j=i+begin2;j<end2;j=j+end1-begin1){
          sum[i]+=tablelist[index][j];
        }
      }
      for(int i=0; i<end1-begin1; i++){
        tablelist[index][begin1+i]=tablelist[newindex][i]-sum[i];
        tablelist[newindex][i]=0;
      }
    }
    return;
  }
}
void freeTree(ADnode *adnode,int *num,int row,int col,int rmin){
  if(adnode==NULL){return;}
  if(adnode->count<rmin){
    free(adnode->leaflist);
	adnode->leaflist=NULL;
    return;
  }

  if(adnode->father<col){
    for( int i = adnode->father; i < col; i++){
      for(int j=0;j<num[i];j++){
        freeTree(((adnode->children) + (i - adnode->father))->children[j],num,row,col,rmin);
        ((adnode->children) + (i - adnode->father))->children[j]=NULL;
      }
      free(((adnode->children) + (i - adnode->father))->children);
      ((adnode->children) + (i - adnode->father))->children=NULL;
    }
    free(adnode->children);
    adnode->children=NULL;
  }
  return;
}
void printTable(void *adtree, int* contab, int contabsize, int *variables, int nvariables){
  the_tree* tree = (the_tree*)(adtree);
  if(tree->root->children==NULL){printf("The tree has been released\n");return;}
  link **Var = tree->value;
  int *num = tree->num;
  int value[nvariables];
  for(int i=0;i<nvariables;i++){
    value[i]=0;
  }
  for(int i=0;i<nvariables;i++){
    printf("\tVar %d\t",variables[i]);
  }
  printf("\tnum\t");
  printf("\n");
  static int order=0;
  loop(contab,variables,contabsize,nvariables,num,Var,0,value,order);
}

void loop(int* contab, int *variables, int contabsize, int nvariables, int *num, link **Var, int var, int *value, int order){
  if(var==nvariables-1){
    link* varvalue=Var[variables[var]-1]->next;
    for(int j=0;j<num[variables[var]-1];j++){
      value[var]=varvalue->value;
      for(int i=0;i<nvariables;i++){
        printf("\t%d\t",value[i]);
      }
      printf("\t%d\t",contab[order]);
      printf("\n");
      order+=1;
      varvalue=varvalue->next;
    }
  }
  else{
    link *varvalue=Var[variables[var]-1]->next;
    for(int j=0;j<num[variables[var]-1];j++){
      value[var]=varvalue->value;
      loop(contab,variables,contabsize,nvariables,num,Var,var+1,value,order);
      int race=1;
      for(int i=var+1;i<nvariables;i++){
        race*=num[variables[i]-1];
      }
      order=order+race;
      varvalue=varvalue->next;
    }
  }
}

void main(){
  int xxx[32][6] = {
    {1, 1, 1, 1, 1, 1} ,
    {1, 1, 1, 1, 2, 1} ,
    {1, 1, 1, 2, 1, 1} ,
    {1, 1, 1, 2, 2, 1} ,
    {1, 1, 2, 1, 1, 1} ,
    {1, 1, 2, 1, 2, 1} ,
    {1, 1, 2, 2, 1, 1} ,
    {1, 1, 2, 2, 2, 1} ,
    {1, 2, 1, 1, 1, 1},
    {1, 2, 1, 1, 2, 1} ,
    {1, 2, 1, 2, 1, 1} ,
    {1, 2, 1, 2, 2, 1} ,
    {1, 2, 2, 1, 1, 1} ,
    {1, 2, 2, 1, 2, 1} ,
    {1, 2, 2, 2, 1, 1} ,
    {1, 2, 2, 2, 2, 1} ,
    {2, 1, 1, 1, 1, 1},
    {2, 1, 1, 1, 2, 1} ,
    {2, 1, 1, 2, 1, 1} ,
    {2, 1, 1, 2, 2, 1} ,
    {2, 1, 2, 1, 1, 1} ,
    {2, 1, 2, 1, 2, 1} ,
    {2, 1, 2, 2, 1, 1} ,
    {2, 1, 2, 2, 2, 1} ,
    {2, 2, 1, 1, 1, 1},
    {2, 2, 1, 1, 2, 1} ,
    {2, 2, 1, 2, 1, 1} ,
    {2, 2, 1, 2, 2, 1} ,
    {2, 2, 2, 1, 1, 1} ,
    {2, 2, 2, 1, 2, 1} ,
    {2, 2, 2, 2, 1, 1} ,
    {2, 2, 2, 2, 2, 1} ,
  };
  int data[192];
  int row=32;
  int col=6;
  int rmin=0;

  for(int i=0;i<row;i++){
    for(int j=0;j<col;j++){
      data[i*col+j]=xxx[i][j];
    }
  }

  void* p=return_adtree(rmin,data,192,row,col);
  the_tree* tree = (the_tree*)p;

  int *contab=(int *) malloc(64 * sizeof(long));

  int nvariables=3;
  int variables[3]={1,4,5};
  int contabsize=1;
  for(int i=0;i<nvariables;i++){
    contabsize*=tree->num[variables[i]-1];
  }


  makecontab(p, variables, nvariables, contab,contabsize);
  for(int i=0;i<contabsize;i++){
    printf("%d\n",contab[i]);
  }

  //printTable(contab,variables,contabsize,nvariables, tree->value, tree->num);
  free(contab);
  del_adtree(p);
  return;
}
