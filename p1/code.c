#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>


////////////////////////////////////////////////////////////////////////////////
//Utility functions

// Print the whole array
void print_array(int** a,int p,int q){

    int i,j;
    for(i=0;i<p;i++){
      for(j=0;j<q;j++){
        printf("%d ",a[i][j]);
      }
      printf("\n");

    }
}
// Special printing function for block
void print_array_second(int **a,int p){
  int j,k;
  for(j=0;j<p;j++){
    int x=a[j][1]+3;
    for(k=0;k<x;k++){
      printf("%d ",a[j][k]);
      }
      printf("\n");
  }
}

// Allocate memory to any array
void allocate_memory(int*** x,int rows,int cols){
  *x = (int**)malloc(rows * sizeof(int*));
  int i;
 for (i=0; i<rows; i++)
 {
   (*x)[i] = (int*)malloc(cols * sizeof(int));
 }

}
void allocate_memory_second(int*** x,int rows){
  *x=(int**)malloc(rows * sizeof(int*));
}
void deallocate_memory(int*** x,int rows){
  for (int i = 0; i < rows; i++)
{
    int* currentIntPtr = (*x)[i];
    free(currentIntPtr);
}
free(*x);
}
void copy(int ** a,int ** b,int r1,int c1){
  int i,j;
  for(i=0;i<r1;i++){
    for(j=0;j<c1;j++){
      a[i][j]=b[i][j];
    }
  }
}
void check_independency(int ** block,int r1,int no_of_blocks){
int i,j,k,l;
  for(i=0;i<r1;i++){
    for(l=i+1;l<r1;l++){
      int x=0;
    for(j=3;j<block[i][1]+3;j++){

      for(k=3;k<block[l][1]+3;k++){
        if(block[i][j]==block[l][k])
          {
            x=1;
            break;
          }
        }

      }
      if(x==0){
        printf("independent pairs %d %d \n",i,l);
        int p;
        for(p=0;p<block[i][1]+3;p++)
        printf("%d ",block[i][p]);
        printf("\n");
        for(p=0;p<block[l][1]+3;p++)
        printf("%d ",block[l][p]);
        printf("\n");
      }

    }
  }

}
void shuffle(int *array, size_t n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}
void print_array_to_file(int **range,int r1,int c1,int value,char *name){
  FILE *f = fopen(name, "w");

  if (f == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }
  fprintf(f,"%d ",value);
  for(int i=0;i<r1;i++){
      if(range[i][2]!=-1){
        int x=range[i][2];
        //printf("%d\n",x);
        fprintf(f,"%d ",range[i][2]);

      }
  }
}
///////////////////////////////////////////////////////////////////////////////
//Restart Hill Climbing functions
//
//////////////////////////////////////////////////////////////////////////////
//Prototyping
//int is_valid_state_space(int **,int,int,int **,int,int);
//void start_hill_climbing(int **,int,int,int**,int,int);
//void hill_climbing(int **,int,int,int **,int,int,int **,int,int,int);



///////////////////////////////////////////////////////////////////
//function definition
int state_space_value(int ** range,int r1,int c1,int ** blocks,int r2,int c2){
  int sum=0;
  for(int i=0;i<r1;i++){
    if(range[i][2]!=-1){
      sum+=blocks[range[i][2]][2];
    }
  }
  //deallocate_memory(&range,r1);
  //deallocate_memory(&blocks,r1);
return sum;
}




int is_valid_state_space(int ** range,int r1,int c1,int ** block,int r2,int no_of_blocks){
  //printf("printing \n");
 //print_array(state_space,r1,c1);

  int i,j;
  int track=0;
  int allocated[no_of_blocks];
  for(i=0;i<no_of_blocks;i++)
  allocated[i]=0;

  for(i=0;i<r1;i++){
    if(range[i][2]==-1){
      continue;
    }
    else{


      for(j=3;j<block[range[i][2]][1]+3;j++)
      {
        if(allocated[block[range[i][2]][j]]==1)
        {

    //      printf("returning with 0\n");
          //exit(1);
          //deallocate_memory(&range,r1);
        //deallocate_memory(&block,r2);

          return 0;
        }
        allocated[block[range[i][2]][j]]=1;
      //  printf("allocated %d\n",block[range[i][2]][j]);
      //block[track+pos-1][] denotes the position at which our collection of blocks is stored
      }
    }
  }
 // deallocate_memory(&range,r1);
  //deallocate_memory(&block,r2);
  //printf("returning with 1\n");
  return 1;
}

void find_successor(int ** range,int r1,int c1,int ** blocks,int r2,int c2,int no_of_blocks){
  int value=state_space_value(range,r1,c1,blocks,r2,c2);
  int ** temp_range;
  int ** last_range;
  allocate_memory(&temp_range,r1,c1);
  allocate_memory(&last_range,r1,c1);
  int *arr;
  arr=(int*) malloc(r1*sizeof(int));
  for(int i=0;i<r1;i++)
  arr[i]=i;
  shuffle(arr,r1);
  copy(temp_range,range,r1,c1);
  int uphill=0;
    for(int j=0;j<r1;j++){
      copy(last_range,range,r1,c1);
      int i=arr[j];
      int x=rand()%2;
      if(x==0){
        range[i][2]=-1;
      }
      else{
        x=rand()%2;
        if(x==0)
        {
          if(range[i][2]==-1){
            range[i][2]=range[i][0];
          }
          else{
          range[i][2]=range[i][2]+1;
          if(range[i][2]>range[i][1])
          {
            range[i][2]=range[i][0];
          }
        }
        }
        else{
          if(range[i][2]==-1){
            range[i][2]=range[i][1];
          }
          else{
          range[i][2]=range[i][2]-1;
          if(range[i][2]<range[i][0])
          range[i][2]=range[i][1];
        }
        }
      }
      if(is_valid_state_space(range,r1,c1,blocks,r2,no_of_blocks)==1){
        int currentvalue=state_space_value(range,r1,c1,blocks,r2,c2);
        if(currentvalue>value){
          copy(temp_range,range,r1,c1);
          value=currentvalue;
        }
      }
      copy(range,last_range,r1,c1);
    }

    copy(range,temp_range,r1,c1);
    deallocate_memory(&temp_range,r1);
    deallocate_memory(&last_range,r1);
    free(arr);
   // deallocate_memory(&range,r1);
    //deallocate_memory(&blocks,r2);

  }
//function to start hill Climbing
void start_hill_climbing(int** range,int r1,int c1,int** blocks,int r2,int no_of_blocks){
  for(int j=0;j<r1;j++){
    range[j][2]=-1;
  }
  for(int j=0;j<r1;j++){
    int i=rand()%r1;
    if(range[i][2]!=-1)
      continue;
    if(range[i][1]-range[i][0]<0)
    {
      range[i][2]=-1;
      continue;
    }
    int x=rand()%2;
    if(x==0){
      range[i][2]=-1;
    }
    else{
    range[i][2]=(rand()%(range[i][1]-range[i][0]+1))+range[i][0];
  }
    int count=10;
    while(is_valid_state_space(range,r1,c1,blocks,r2,no_of_blocks)!=1 && count!=0)
    {
      range[i][2]=(rand()%(range[i][1]-range[i][0]+1))+range[i][0];
      count--;
    }
    if(count==0){
      range[i][2]=-1;
    }
  }

}

//function to find the best successor from the successor of current node
//void find_successor(int ** state_space,int r1,int c1,int ** range,int r2,int c2,int ** blocks,int r3,int c3);

//function to find out state space value of any state
//int state_space_value(int ** state_space,int r1,int c1,int ** range,int r2,int c2,int ** blocks,int r3,int c3);

//function to perform hill Climbing
void hill_climbing(int ** range,int r1,int c1,int ** blocks,int r2,int c2,int no_of_blocks,time_t start,time_t seconds,char* name){
  int** max_state_space;
  int** maxstatespaceforloop;
  start=time(NULL);
  time_t end;
  //seconds=3;
  end=start+seconds;
  allocate_memory(&max_state_space,r1,c1);
  allocate_memory(&maxstatespaceforloop,r1,c1);
  int max_value=0;
  int value=0;

  //random starting for initialization
  start_hill_climbing(range,r1,c1,blocks,r2,no_of_blocks);
  max_value=state_space_value(range,r1,c1,blocks,r2,c2);
  copy(max_state_space,range,r1,c1);
  int i=0,j=0;

  //Performing hill climbing
  for(j=0;start<end;j++){

    //randomly initializing state space
    start_hill_climbing(range,r1,c1,blocks,r2,no_of_blocks);
    //printf("initial state\n");
    //print_array(range,r1,c1);
    int last=0;
    int maxvalueforloop=0;
    for(i=0;i<2*r1 && start<end;i++){
      start=time(NULL);
      int currentvalue=0;
      currentvalue=state_space_value(range,r1,c1,blocks,r2,c2);
      if(maxvalueforloop<currentvalue)
      {
        maxvalueforloop=currentvalue;
        copy(maxstatespaceforloop,range,r1,c1);
        last=i;
      }
      if(i-last>3)
      break;
      if(max_value<maxvalueforloop){
        max_value=maxvalueforloop;
        copy(max_state_space,maxstatespaceforloop,r1,c1);
      }
      find_successor(range,r1,c1,blocks,r2,c2,no_of_blocks);

    }
  //  printf("Loop time is %s max time is %s",ctime(&start),ctime(&end));
  }
  print_array_to_file(max_state_space,r1,c1,max_value,name);
  deallocate_memory(&maxstatespaceforloop,r1);
  deallocate_memory(&max_state_space,r1);

//  printf("max value found %d for the state space \n State space : \n",max_value);
//  print_array(max_state_space,r1,c1);
}
int max_value(int ** block,int r1,int ** max_val){
  *max_val=(int*) malloc(sizeof(int)*3);
  (*max_val)[0]=0;
  for(int i=0;i<r1;i++){
    if((*max_val)[0]<block[i][2])
    {
      (*max_val)[0]=block[i][2];
      (*max_val)[1]=block[i][0];
    }
  }
    free(*max_val);
}
/////////////////////////////////////////////////////////////////////////////////
//
int main(int argc,char* argv[]){

  srand(time(NULL));
  time_t start=time(NULL);
  // Checking command line arguments
  if(argc < 2)
  {
    printf("No input file provided.\nBreaking the program");
    exit(1);
  }
  if(argc<3){
    printf("No output file provided.\nBreaking the program");
    exit(1);
  }
  FILE *in;
  in=fopen(argv[1],"r");
  if(in == NULL)
  {
    printf("File not opened.\nBreaking the program");
    exit(2);
  }

  //Taking input
  int T,N,B,C;
  fscanf(in, "%d", &T);
  time_t seconds=T*60;
  seconds=180;
  fscanf(in,"%d",&N);
  fscanf(in,"%d",&B);
  fscanf(in,"%d",&C);
  int cid[C][2];
  int i=0;
  int **blocks;
  allocate_memory_second(&blocks,B);

  int** range;
  allocate_memory(&range,C,3);

  int l=0;
  for(;i<C;i++){
    int j;
    fscanf(in,"%d %d",&cid[i][0],&cid[i][1]);
    range[i][0]=l;
    range[i][1]=l+cid[i][1]-1;
    range[i][2]=-1;
    for(j=0;j<cid[i][1];j++){
      //printf("\n j: %d cid[i][1] % d\n",j,cid[i][1]);
      int companyid,no_of_blocks,value;
      fscanf(in,"%d %d %d",&companyid,&no_of_blocks,&value);
      //printf("\ncompany id %d  no of blocks %d  value %d\n",companyid,no_of_blocks,l);//debug
      int k=0;

      blocks[l]=(int *)malloc((no_of_blocks+3)*sizeof(int));
      blocks[l][0]=companyid;
      blocks[l][1]=no_of_blocks;
      blocks[l][2]=value;
      for(;k<no_of_blocks;k++){
        fscanf(in,"%d",&blocks[l][k+3]);
        }
      l++;
    }
   //printf("\n\n\nequal j: %d,cid[i][1 :%d\n\n\n",j,cid[i][1]);

    if(j!=cid[i][1])
    {
    //  printf("exiting j %d cid[i][0] %d cid[i][1] %d i %d blocks no %d",j,cid[i][0],cid[i][1],i,blocks[l-1][1]);
      exit(1);
    }
  }
//check_independency(blocks,B,N);
//print_array(state_space,C,max+1);
//printf("printing second\n");
//print_array(range,C,3);
hill_climbing(range,C,3,blocks,B,N,N,start,seconds,argv[2]);
deallocate_memory(&range,C);
deallocate_memory(&blocks,B);
//int *max_val;
//max_value(blocks,B,&max_val);
//printf("max value %d at %d",max_val[0],max_val[1]);
//printf("Line \n");
//print_array(range,C,3);
//printf("printing %d",is_valid_state_space(state_space,C,max+1,blocks,B,N));

  fclose(in);
  return 0;
}
