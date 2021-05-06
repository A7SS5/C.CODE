#include <stdio.h>
#include <stdlib.h>
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* decode(int* encoded, int encodedSize, int first, int* returnSize){

    int i=1;
    returnSize=(int*)malloc(sizeof(int)*(encodedSize+1));
    returnSize[0]=first;
    for (i=1;i<(encodedSize+1);i++)
    {
        returnSize[i]=encoded[i-1]^returnSize[i-1];
    }
    return returnSize;
}
int main()
{
    int *returnSize;
 int encoded[3] = {1,2,3}; int first = 1;
 decode(encoded, 3, first,returnSize);
printf("ok");
}