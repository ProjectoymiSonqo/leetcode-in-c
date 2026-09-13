#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>

char pattern[] = "abc";
char s[] = "b c a";

int main(){
    int len=strlen(pattern);
    char *word[len];
    int start=0;
    int i_start=0;
    int map_s[len];
    int state[len];
    int map_t[len];
    bool result=true;
    memset(state, 0, len * sizeof(int)); 
    int word_count = 1;
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (s[i] == ' ')
            word_count++;
    }
    if (word_count != strlen(pattern))
    result=false;
    for(int i=i_start; ;i++)
    {
        if(s[i]==' '||s[i]=='\0')
        {
            int word_len=i-i_start;
            int length=word_len+1;
            word[start]=(char *)malloc(length * sizeof(char));
            
            for (int j = 0; j < word_len; j++) {
            word[start][j] = s[i_start+j]; 
//            printf("word[%d]=%c\n",start,word[start][j]);
            }
            word[start][word_len] = '\0';
            start++;
            i_start=i+1;
        }
        if(s[i]=='\0')
        {
            break;
        }   
    }
 int index_s = 0;

for (int i = 0; i < len; i++)

{

    bool found = false;

    for (int j = 0; j < i; j++)

    {

        if (strcmp(word[i], word[j]) == 0)

        {

            map_s[i] = map_s[j];

            found = true;

            break;

        }

    }

    if (!found)

    {

        index_s++;

        map_s[i] = index_s;

    }

}
    for(int g=0;g<len;g++)
    printf("%d,",map_s[g]);

    int map_pattern[26]={0};
    int index_pattern=1;
    for(int m=0;m<len;m++)
    {
        char c=pattern[m];
        if(map_pattern[(unsigned char)c-'a']==0)
        {
            map_pattern[(unsigned char)c-'a']=index_pattern;
            index_pattern++;
            
        }
        map_t[m]=map_pattern[(unsigned char)c-'a'];
//        printf("%d,",map_t[m]);
        if(map_t[m]!=map_s[m])
        {
            result=false;
            break;
        }
        
    }
    printf("result=%d",result);

    
    return 0;
}

