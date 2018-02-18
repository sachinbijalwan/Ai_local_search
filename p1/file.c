//for printing blocks
/*
i=0;
for(i=0;i<l;i++)
{
  int j;
  for(j=0;blocks[i][j]!=0;j++)
  {
    printf("%d ",blocks[i][j]);
  }
  printf("\n");
}
*/
//Creating state space
/*int max=-1;
int index=-1;
for(i=0;i<C;i++){
int c=range[i][1]-range[i][0]+1;
if(c>max)
{
  max=c;
  index=i;
}
}
if(index==-1)
{
  printf("No bids entered \n");
  exit(1);
}
int **state_space;
allocate_memory(state_space,C,max+1);
int x=0,y=0;
for(i=0;i<C;i++){             //initializing state space
  int j;
  y=range[i][1]-x;
  x=range[i][1];
  if(i==0)
  y++;

  for(j=0;j<max+1;j++){
    if(j<=y)
    {
      state_space[i][j]=0;
    }
    else
    {
      state_space[i][j]=-1;
    }
  }
  //state_space[i][0]=
}

// Performing hill climb search
int restart=1;
*/


/*  for(i=0;i<C;i++)
{
  int j=0;
  for(j=0;j<2;j++)
  printf("%d ",range[i][j]);;
  printf("\n");
}
*/
