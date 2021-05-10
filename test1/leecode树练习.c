#include<stdbool.h>
#include<stdio.h>
 struct TreeNode {
     int val;
     struct TreeNode *left;
     struct TreeNode *right;
 };
int compare(int *arry1,int *arry2,int size)
{
    
    int i=0;
    for (i=0;i<size;i++)
    {
        if (arry1[i]!=arry2[i])
        return 0;
    }
    return 1;
}
void getgroup(struct TreeNode* T,int* arry,int *size)
{
    if (T->left==NULL&&T->right==NULL)
    {
        arry[*(size)++]=T->val;
        return;
    }
    if (T->left)
    getgroup(T->left,arry,size);
    if (T->right)
    getgroup(T->right,arry,size);
    return;
}
bool leafSimilar(struct TreeNode* root1, struct TreeNode* root2){
int size1=0;
int size2=0;
int arry1[100];
int arry2[100];
getgroup(root1,arry1,&size1);
getgroup(root2,arry2,&size2);
if (size1!=size2)
return false;
if (compare(arry1,arry2,size1))
{
    return true;
}
else return false;
}