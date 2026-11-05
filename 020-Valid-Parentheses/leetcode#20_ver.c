
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

typedef struct{
    char stack[10000];
    int top;
}Mystack;

Mystack* myStackCreate(){
    Mystack *Bunstack=malloc(sizeof(Mystack));
    if(Bunstack==NULL)
        return NULL;
    // for(int i=0;i<sizeof(Bunstack->stack);i++)
    // {
    //     Bunstack->stack[i]=0;
    // }
    Bunstack->top=-1;
    return Bunstack;
}
// push peek pop
bool stack_push(Mystack *obj,char val){
    if(obj->top==9999)
        return false;
    if(val=='['||val=='('||val=='{')
    {
        obj->top++;
        obj->stack[obj->top]=val;
        return true;
    }    
    return false;
}
char stack_peek(Mystack *obj){
    if(obj->top==-1)
        return '\0';
    else
        return obj->stack[obj->top];    
}
bool stack_pop(Mystack *obj){
    if(obj->top==-1)
        return false;
    else
    {
        obj->top--;
        return true;
    }
}
bool isValid(char* s)
{
    int charsize=strlen(s);
    Mystack *bun_stack=myStackCreate();
    bool result;
    for(int i=0;i<strlen(s);i++)
    {
        if(s[i]=='{'||s[i]=='['||s[i]=='(')
        {
            result=stack_push(bun_stack,s[i]);
            if(result==false)
            {
                free(bun_stack);
                return result;
            }
        }
        else if(s[i]=='}'||s[i]==']'||s[i]==')')
        {
            char check=stack_peek(bun_stack);
            if((check=='{'&&s[i]=='}')||(check=='['&&s[i]==']')||(check=='('&&s[i]==')'))
            {
                stack_pop(bun_stack);
            }
            else
            {
                free(bun_stack);
                return false;
            }
        }
    }
    if(bun_stack->top==-1)
        result=true;
    else
        result=false;
    free(bun_stack);
    return result;    
    
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
