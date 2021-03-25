#include<stdio.h>
void dfs(int step,int a[10],int book[10])
{
    int i=0;
    for (i=1;i<=10;i++)
    {
        if (step==11)
        {
            for (i=0;i< 10;i++)
            {
                printf("%d ",a[i]);
            }
            printf("\n");
            return;
        }
        if (book[i-1]==0)
        {
            a[step-1]=i;
            book[i-1]=1;
            dfs(step+1,a,book);
            book[i-1]=0;
        }
    }
}
int main()
{
    int a[10]={0};  //表示第i个箱子
    int book[10]={0};  //标记已使用的牌
    dfs(1,a,book);
}