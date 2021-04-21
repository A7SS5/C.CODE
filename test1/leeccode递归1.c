#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void fun(char *s,int a[],int i,int j,int *c)
{
    int len = strlen(s);
    if (i==len)
    {
        *c=*c+1;
        return;
    }
    if (s[i]=='0')
    {
        return;
    }
    else 
    {
        a[j]=s[i]-'0';
        fun(s,a,i+1,j+1,c);
        i++;
        if (i==len)
        {
            return;
        }
        a[j]=a[j]*10+s[i]-'0';
        if (a[j]>26)
        return;
        else {
            fun(s,a,i+1,j+1,c);
            }
    }
    return;
}
int main()
{
    char *s="111111111111111111111111111111111111111111111";
    int *a;
    int i=0,j=0;
    int len=strlen(s);
    int count=0;
    a=(int *)malloc(sizeof(int)*len);
    int h=0;
    for (h=0;h<len;h++)
    a[h]=0;
    fun(s,a,i,j,&count);
    free(a);
    printf("%d",count);
}