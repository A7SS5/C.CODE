#include<stdio.h>
void dfs(int steps,int i,int arrlen,int *count)
{   if (steps<0)
    return;    
    if (i<0||i>=arrlen)
    return;
    if (steps==0)
    {
        if (i==0)
        {
            *count=*count+1;
        }    
        else return;
    }
    dfs(steps-1,i-1,arrlen,count);
    dfs(steps-1,i,arrlen,count);
    dfs(steps-1,i+1,arrlen,count);
}
int numWays(int steps, int arrLen){
int i=0;
int count=0;
dfs(steps-1,i+1,arrLen,&count);
dfs(steps-1,i,arrLen,&count);
return count;
}
int main()
{
    int steps=27;int arrLen=7;
    int p=numWays(steps,arrLen);
    printf("%d\n",p);
}