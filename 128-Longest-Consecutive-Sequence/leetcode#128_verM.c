
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

struct node {
    int val;
    struct node *next;
};

static int hash_index(int value, int bucket)
{
    int index = value % bucket;

    if (index < 0)
        index += bucket;

    return index;
}

static bool hash_contains(struct node **table, int bucket, int value)
{
    int index = hash_index(value, bucket);
    struct node *current = table[index];

    while (current != NULL)
    {
        if (current->val == value)
            return true;

        current = current->next;
    }

    return false;
}

int longestConsecutive(int *nums, int numsSize)
{
    if (numsSize <= 0)
        return 0;

    if (numsSize > (INT_MAX - 1) / 2)
        return 0;

    int bucket = numsSize * 2 + 1;

    // 1. 配置 Bucket Array
    struct node **table = calloc(bucket, sizeof(*table));

    // 2. 一次配置所有可能需要的 Node
    struct node *pool = malloc((size_t)numsSize * sizeof(*pool));

    if (table == NULL || pool == NULL)
    {
        free(table);
        free(pool);
        return 0;
    }

    int used = 0;

    // 3. 建立 Hash Table
    for (int i = 0; i < numsSize; i++)
    {
        if (hash_contains(table, bucket, nums[i]))
            continue;

        int index = hash_index(nums[i], bucket);

        // 從 Pool 取出下一個可用 Node
        struct node *new_node = &pool[used++];

        new_node->val = nums[i];
        new_node->next = table[index];
        table[index] = new_node;
    }

    // 4. 只從連續序列起點開始搜尋
    int max_length = 0;

    for (int i = 0; i < bucket; i++)
    {
        struct node *current = table[i];

        while (current != NULL)
        {
            int value = current->val;

            if (value == INT_MIN ||
                !hash_contains(table, bucket, value - 1))
            {
                int length = 1;
                int target = value;

                while (target != INT_MAX &&
                       hash_contains(table, bucket, target + 1))
                {
                    target++;
                    length++;
                }

                if (length > max_length)
                    max_length = length;
            }

            current = current->next;
        }
    }

    // 5. 只需要兩次 free
    free(pool);
    free(table);

    return max_length;
}
