
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

bool check_continuous(char val1,char val2)
{
    if(val1=='('&&val2==')')
        return true;
    else if(val1=='['&&val2==']')
        return true;
    else if(val1=='{'&&val2=='}')
        return true;
    else
        return false;
}
bool isValid(char* s)
{
    int char_size=strlen(s);
    if(char_size%2!=0)
    {
        return false;
    }
    else
    {
        int mark[char_size];
        for(int i=0;i<char_size;i++)
        {
            mark[i]=1;
        }
        int count=0;
        while(count<char_size/2)
        {
            for(int i=0;i<char_size;i++)
            {
                if(i+1<char_size)
                {
                    if(check_continuous(s[i],s[i+1])&&mark[i]!=0&&mark[i+1]!=0)
                    {
                        mark[i]=0;
                        mark[i+1]=0;
                    }
                    if(mark[i]!=0&&mark[i+1]==0)
                    {
                        int j=i+1;
                        while(j<char_size&&mark[j]==0)
                        {
                            j=j+1;
                        }
                        if(j<char_size)
                        {
                            if(check_continuous(s[i],s[j]))
                            {
                                mark[i]=0;
                                mark[j]=0;
                            }
                        }
                    }
                }
            }
            count++;
        }
        for(int i=0;i<char_size;i++)
        {
            if(mark[i]==1)
            {
                return false;
            }
        }
        
        return true;
    }

}

int main(void)
{
    // Basic cases
    assert(isValid("()") == true);
    assert(isValid("()[]{}") == true);
    assert(isValid("(]") == false);
    assert(isValid("([])") == true);
    assert(isValid("([)]") == false);

    // Nested parentheses
    assert(isValid("{[()]}") == true);
    assert(isValid("((()))") == true);
    assert(isValid("((())") == false);
    assert(isValid("({[]})") == true);

    // Unmatched parentheses
    assert(isValid("(") == false);
    assert(isValid(")") == false);
    assert(isValid("(((") == false);
    assert(isValid(")))") == false);
    assert(isValid("(){") == false);

    // Additional cases
    assert(isValid("[]{}()") == true);
    assert(isValid("([{}])") == true);
    assert(isValid("[(])") == false);
    assert(isValid("(){[()]}") == true);

    printf("All tests passed!\n");
    return 0;
}
