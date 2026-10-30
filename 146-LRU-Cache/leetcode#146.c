#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ============================================================
// LeetCode 146 - LRU Cache
//
// Your Solution
// ============================================================
//
//LRUCache
//│
//├── Capacity / Size
//│
//├── Hash Table
//│     ├── Bucket 0 → Node
//│     ├── Bucket 1 → Node → Node
//│     └── Bucket 2 → NULL
//│
//└── LRU Linked List
//      HEAD ⇄ Node ⇄ Node ⇄ TAIL
//
//bucket_count = capacity * 2 + 1;
//index = key % bucket_count;
struct Node {
    int key;                //題目需求的key
    int value;              //題目需求的value
    struct Node *hash_next; //用hash table快速找值
    struct Node *prev;      //doubly linked list
    struct Node *next;      //找到值後 不需要再遍歷一次能直接把前後值接上
};

typedef struct {
    int capacity;           //Cache大小
    int size;               //當前資料量
    struct Node *head;      //把head當最新值 如果有get將調整順序
    struct Node *tail;      //tail最舊值 沒有被呼叫過的值會漸漸後移 size滿了先刪他
    int bucket_count;       //雜湊表大小
    struct Node **table;    //雜湊表
} LRUCache;

LRUCache* lRUCacheCreate(int capacity)
{
    //Capacity、Bucket Count、Head/Tail 初始化
    LRUCache *cache=malloc(sizeof(LRUCache));   //cache初始化
    if(cache==NULL)                             //初始化失敗回傳NULL
    {
        return NULL;
    }
    cache->size=0;                              //初始化size=0;
    cache->capacity=capacity;                   //初始化導入capacity
    cache->head=NULL;                           //初始化head
    cache->tail=NULL;                           //初始化tail
    cache->bucket_count=cache->capacity*2+1;    //設置浮動bucket降低雜湊碰撞機率
    cache->table=calloc(cache->bucket_count,sizeof(struct Node *));
                                                //初始化table 由於起始希望是NULL所以用calloc
    if(cache->table==NULL)                      //初始化失敗回傳NULL並釋放空間
    {
        free(cache);
        return NULL;
    }
    return cache;
    
}

static struct Node *hashFind(LRUCache *obj, int key)
{
    int index = key % obj->bucket_count;

    struct Node *current = obj->table[index];

    while (current != NULL)
    {
        // 如果找到 key，回傳 Node 地址
        if(current->key==key)
        {
            return current;
        }
        // 否則往下一個 Hash Node 移動
        else
        {
            current=current->hash_next;
        }
    }

    return NULL;
}

static void moveToHead(LRUCache *obj, struct Node *node)
{
    if(node==obj->head)
    {
        return;
    }
    else if(node==obj->tail)
    {
        obj->tail=obj->tail->prev;      //處理尾巴
        obj->tail->next=NULL;       

        obj->head->prev=node;           //處理頭
        node->next=obj->head;           //接上node
        node->prev=NULL;
        obj->head=node;                 //改掉頭
    }
    else
    {
        node->prev->next=node->next;    //處理中間接點
        node->next->prev=node->prev;

        node->next=obj->head;           //處理頭
        obj->head->prev=node;           //接上node
        node->prev=NULL;
        obj->head=node;                 //改掉頭
    }
}

int lRUCacheGet(LRUCache* obj, int key)
{
    struct Node *node=hashFind(obj,key);
    if(node!=NULL)
    {
        moveToHead(obj,node);
        return node->value;
    }
    else
    {
        return -1;
    }
}
static void hashRemove(LRUCache *obj, struct Node *node)
{
    int index=node->key%obj->bucket_count;
    struct Node *current = obj->table[index];
    struct Node *previous = NULL;
    while (current != NULL)
    {
        if (current == node)
        {
            // 找到了！
            if(previous==NULL)
            {
                obj->table[index]=current->hash_next;
            }
            else
            {
                previous->hash_next=current->hash_next;
            }
           break;
        }
        previous = current;
        current = current->hash_next;
    }    
}

void lRUCachePut(LRUCache* obj, int key, int value)
{
    // Your solution here
    struct Node *node=hashFind(obj,key);
    if(node!=NULL)                      //有既有值
    {
        node->value=value;              //更新值 
        moveToHead(obj,node);            //移動
    }
    else                                //無既有值
    {
        if(obj->size==obj->capacity)    //capacity滿了 分別要處理hash跟doubly linked list
        {                         
            struct Node *new_node=obj->tail;
                  
            hashRemove(obj,new_node);
            new_node->value=value;     //把tail改值
            new_node->key=key;         //tail改key
            
            int index=key%obj->bucket_count;

            new_node->hash_next = obj->table[index];//把節點放到最前面接到table原本所指向的
            obj->table[index] = new_node;//table指向新節點
            
            moveToHead(obj,new_node);  //直接搬運
        }
        else // 沒滿的情況要放值進去 考慮到兩邊hash table 跟doubly linked list的銜接
        {
            obj->size++;                                        //內容物+1
            struct Node *new_node=malloc(sizeof(struct Node));  //為新資料分配一個空間
            new_node->key=key;                                  //放key
            new_node->value=value;                              //放value
            //處理hash table
            int index=new_node->key%obj->bucket_count;
            new_node->hash_next=obj->table[index];
            obj->table[index]=new_node;
            //處理doubly linked list
            if(obj->size==1)
            {
                obj->tail=new_node;
                new_node->next=NULL;
            }
            else
            {
                obj->head->prev=new_node;
                new_node->next=obj->head;
            }
            obj->head=new_node;
            new_node->prev=NULL;
        }
    }
    //基本插入 Node 配置、Hash Index、LRU 順序
    //Capacity 滿載後插入 淘汰 LRU、解除 Hash 連結、釋放節點
}


void lRUCacheFree(LRUCache* obj)
{
    //完整釋放記憶體
    if (obj == NULL)
    {
        return;
    }
    struct Node *current=obj->head;
    while(current!=NULL)
    {
        struct Node *next=current->next;
        free(current);
        current=next;
    }
    free(obj->table);
    free(obj);
    
}


// ============================================================
// Test Environment
// ============================================================

static int totalTests = 0;
static int passedTests = 0;

void check(int actual, int expected, const char *description)
{
    totalTests++;

    if (actual == expected)
    {
        passedTests++;
        printf("  [PASS] %s | Expected: %d, Got: %d\n",
               description, expected, actual);
    }
    else
    {
        printf("  [FAIL] %s | Expected: %d, Got: %d\n",
               description, expected, actual);
    }
}

void testHeader(const char *name)
{
    printf("\n==================================================\n");
    printf("%s\n", name);
    printf("==================================================\n");
}

int main(void)
{
    LRUCache *cache;

    // ========================================================
    // Case 1: LeetCode Example
    // ========================================================

    testHeader("Case 1 - LeetCode Example");

    cache = lRUCacheCreate(2);

    lRUCachePut(cache, 1, 1);
    lRUCachePut(cache, 2, 2);

    check(lRUCacheGet(cache, 1), 1, "get(1)");

    lRUCachePut(cache, 3, 3);

    check(lRUCacheGet(cache, 2), -1, "get(2) evicted");

    lRUCachePut(cache, 4, 4);

    check(lRUCacheGet(cache, 1), -1, "get(1) evicted");
    check(lRUCacheGet(cache, 3), 3, "get(3)");
    check(lRUCacheGet(cache, 4), 4, "get(4)");

    lRUCacheFree(cache);


    // ========================================================
    // Case 2: Capacity = 1
    // ========================================================

    testHeader("Case 2 - Capacity 1");

    cache = lRUCacheCreate(1);

    lRUCachePut(cache, 1, 100);

    check(lRUCacheGet(cache, 1), 100, "get(1)");

    lRUCachePut(cache, 2, 200);

    check(lRUCacheGet(cache, 1), -1, "get(1) evicted");
    check(lRUCacheGet(cache, 2), 200, "get(2)");

    lRUCacheFree(cache);


    // ========================================================
    // Case 3: Update Existing Key
    // ========================================================

    testHeader("Case 3 - Update Existing Key");

    cache = lRUCacheCreate(2);

    lRUCachePut(cache, 1, 10);
    lRUCachePut(cache, 2, 20);
    lRUCachePut(cache, 1, 100);

    check(lRUCacheGet(cache, 1), 100, "updated value");

    lRUCachePut(cache, 3, 30);

    check(lRUCacheGet(cache, 2), -1, "key 2 evicted");
    check(lRUCacheGet(cache, 1), 100, "key 1 remains");
    check(lRUCacheGet(cache, 3), 30, "key 3 remains");

    lRUCacheFree(cache);


    // ========================================================
    // Case 4: Get Changes LRU Order
    // ========================================================

    testHeader("Case 4 - Get Changes LRU Order");

    cache = lRUCacheCreate(3);

    lRUCachePut(cache, 1, 10);
    lRUCachePut(cache, 2, 20);
    lRUCachePut(cache, 3, 30);

    check(lRUCacheGet(cache, 1), 10, "access key 1");

    lRUCachePut(cache, 4, 40);

    check(lRUCacheGet(cache, 2), -1, "key 2 evicted");
    check(lRUCacheGet(cache, 1), 10, "key 1 remains");
    check(lRUCacheGet(cache, 3), 30, "key 3 remains");
    check(lRUCacheGet(cache, 4), 40, "key 4 remains");

    lRUCacheFree(cache);


    // ========================================================
    // Case 5: Missing Key
    // ========================================================

    testHeader("Case 5 - Missing Key");

    cache = lRUCacheCreate(2);

    check(lRUCacheGet(cache, 99), -1, "get from empty cache");

    lRUCachePut(cache, 1, 10);

    check(lRUCacheGet(cache, 99), -1, "missing key");
    check(lRUCacheGet(cache, 1), 10, "existing key");

    lRUCacheFree(cache);


    // ========================================================
    // Case 6: Repeated Get
    // ========================================================

    testHeader("Case 6 - Repeated Get");

    cache = lRUCacheCreate(2);

    lRUCachePut(cache, 1, 10);
    lRUCachePut(cache, 2, 20);

    check(lRUCacheGet(cache, 1), 10, "first get(1)");
    check(lRUCacheGet(cache, 1), 10, "second get(1)");
    check(lRUCacheGet(cache, 1), 10, "third get(1)");

    lRUCachePut(cache, 3, 30);

    check(lRUCacheGet(cache, 2), -1, "key 2 evicted");
    check(lRUCacheGet(cache, 1), 10, "key 1 remains");

    lRUCacheFree(cache);


    // ========================================================
    // Case 7: Continuous Eviction
    // ========================================================

    testHeader("Case 7 - Continuous Eviction");

    cache = lRUCacheCreate(2);

    lRUCachePut(cache, 1, 10);
    lRUCachePut(cache, 2, 20);
    lRUCachePut(cache, 3, 30);
    lRUCachePut(cache, 4, 40);
    lRUCachePut(cache, 5, 50);

    check(lRUCacheGet(cache, 1), -1, "key 1 evicted");
    check(lRUCacheGet(cache, 2), -1, "key 2 evicted");
    check(lRUCacheGet(cache, 3), -1, "key 3 evicted");
    check(lRUCacheGet(cache, 4), 40, "key 4 remains");
    check(lRUCacheGet(cache, 5), 50, "key 5 remains");

    lRUCacheFree(cache);


    // ========================================================
    // Case 8: Update Changes LRU Order
    // ========================================================

    testHeader("Case 8 - Update Changes LRU Order");

    cache = lRUCacheCreate(3);

    lRUCachePut(cache, 1, 10);
    lRUCachePut(cache, 2, 20);
    lRUCachePut(cache, 3, 30);

    lRUCachePut(cache, 1, 100);

    lRUCachePut(cache, 4, 40);

    check(lRUCacheGet(cache, 2), -1, "key 2 evicted");
    check(lRUCacheGet(cache, 1), 100, "key 1 updated");
    check(lRUCacheGet(cache, 3), 30, "key 3 remains");
    check(lRUCacheGet(cache, 4), 40, "key 4 remains");

    lRUCacheFree(cache);


    // ========================================================
    // Case 9: Value = 0
    // ========================================================

    testHeader("Case 9 - Zero Value");

    cache = lRUCacheCreate(2);

    lRUCachePut(cache, 1, 0);
    lRUCachePut(cache, 2, 20);

    check(lRUCacheGet(cache, 1), 0, "value is zero");

    lRUCacheFree(cache);


    // ========================================================
    // Case 10: Negative Key
    // ========================================================

    // testHeader("Case 10 - Negative Key");

    // cache = lRUCacheCreate(2);

    // lRUCachePut(cache, -1, 100);
    // lRUCachePut(cache, -2, 200);

    // check(lRUCacheGet(cache, -1), 100, "negative key -1");
    // check(lRUCacheGet(cache, -2), 200, "negative key -2");

    // lRUCacheFree(cache);


    // ========================================================
    // Case 11: Evicted Key Reinserted
    // ========================================================

    testHeader("Case 11 - Reinsert Evicted Key");

    cache = lRUCacheCreate(2);

    lRUCachePut(cache, 1, 10);
    lRUCachePut(cache, 2, 20);
    lRUCachePut(cache, 3, 30);

    check(lRUCacheGet(cache, 1), -1, "key 1 evicted");

    lRUCachePut(cache, 1, 100);

    check(lRUCacheGet(cache, 2), -1, "key 2 evicted");
    check(lRUCacheGet(cache, 1), 100, "key 1 reinserted");
    check(lRUCacheGet(cache, 3), 30, "key 3 remains");

    lRUCacheFree(cache);


    // ========================================================
    // Case 12: Large Capacity
    // ========================================================

    testHeader("Case 12 - Large Capacity");

    cache = lRUCacheCreate(100);

    for (int i = 0; i < 100; i++)
    {
        lRUCachePut(cache, i, i * 10);
    }

    for (int i = 0; i < 100; i++)
    {
        char description[64];
        snprintf(description, sizeof(description), "get(%d)", i);

        check(lRUCacheGet(cache, i), i * 10, description);
    }

    lRUCachePut(cache, 100, 1000);

    check(lRUCacheGet(cache, 0), -1, "oldest key evicted");
    check(lRUCacheGet(cache, 100), 1000, "new key inserted");

    lRUCacheFree(cache);


    // ========================================================
    // Summary
    // ========================================================

    printf("\n==================================================\n");
    printf("TEST SUMMARY\n");
    printf("==================================================\n");

    printf("Passed: %d / %d\n", passedTests, totalTests);

    if (passedTests == totalTests)
    {
        printf("ALL TESTS PASSED!\n");
    }
    else
    {
        printf("SOME TESTS FAILED!\n");
    }

    return (passedTests == totalTests) ? 0 : 1;
}