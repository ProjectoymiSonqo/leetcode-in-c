#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
char *strs[] = {"eat","tea","tan","ate","nat","bat"};

char*** groupAnagrams(char** strs, int strsSize, int* returnSize, int** returnColumnSizes) {
//    int strsSize = sizeof(strs) / sizeof(strs[0]);
    int map[strsSize][26];
    memset(map, 0, sizeof(map)); 
    int group[strsSize];
    for(int i=0;i<strsSize;i++)
    {
        for(int j=0;j<strlen(strs[i]);j++)
        {
            char c=strs[i][j];
            map[i][c-'a']++;
        }
    }
    int group_index=0;    
    for(int a=0;a<strsSize;a++)
    {
        bool found=false;
        for(int b=0;b<a;b++)
        {
            bool partial_result=true;
            for(int i=0;i<26;i++)
            {
                if(map[a][i]!=map[b][i])
                {
                    partial_result=false;
                    break;
                }
            }
            if(partial_result==true)
            {
                group[a]=group[b];
                found=true;
                break;
            }
        }
        if(found!=true)
        {
            group_index++;
            group[a]=group_index;
        }
        printf("%d,",group[a]);
    }  
    *returnSize = group_index;
    *returnColumnSizes = (int*)malloc(group_index * sizeof(int));
    memset(*returnColumnSizes, 0, group_index * sizeof(int)); 
    char*** ans = (char***)malloc(group_index * sizeof(char**));
    for (int i = 0; i < group_index; i++) {
        ans[i] = (char**)malloc(strsSize * sizeof(char*));
    }
    for (int i = 0; i < strsSize; i++) {
        int g_id = group[i] - 1; 
        int current_size = (*returnColumnSizes)[g_id]; 
        ans[g_id][current_size] = strs[i];             
        (*returnColumnSizes)[g_id]++;                 
    }
    return ans;
}

int main(){
    int strsSize = sizeof(strs) / sizeof(strs[0]);
    int map[strsSize][26];
    memset(map, 0, sizeof(map)); 
    int group[strsSize];
    for(int i=0;i<strsSize;i++)
    {
        for(int j=0;j<strlen(strs[i]);j++)
        {
            char c=strs[i][j];
            map[i][c-'a']++;
            // printf("map[%d][%d]=%d\n",i,c-'a',map[i][c-'a']);
        }
    }
    int group_index=0;    
    for(int a=0;a<strsSize;a++)
    {
        bool found=false;
        for(int b=0;b<a;b++)
        {
            bool partial_result=true;
            for(int i=0;i<26;i++)
            {
                if(map[a][i]!=map[b][i])
                {
                    partial_result=false;
                    break;
                }
            }
            if(partial_result==true)
            {
                group[a]=group[b];
                found=true;
                break;
            }
        }
        if(found!=true)
        {
            group_index++;
            group[a]=group_index;
        }
        printf("%d,",group[a]);
    }
    


    return 0;
}