#include "inttrie.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct IntTrieNode
{
    char c;
    int value;
    int leaf;
    struct IntTrieNode **children;
    size_t num_children;
    size_t children_size;
} IntTrieNode;

/* typedef'd in the header */
struct IntTrie
{
    IntTrieNode *root;
};

/*
 * Creates a new node with the character c.
 */
static IntTrieNode *node_init(char c)
{
    IntTrieNode *node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }

    node->c = c;
    node->value = 0;
    node->leaf = 0;
    node->children = NULL;
    node->num_children = 0;
    node->children_size = 0;

    return node;
}

/*
 * Frees a node and all its children.
 */
static void node_free(IntTrieNode *node)
{
    for (int i = 0; i < node->num_children; ++i) {
        node_free(node->children[i]);
    }

    free(node->children);
    free(node);
}

/*
 * Recursively inserts a key-value pair in a node.
 */
static IntTrieStatus node_insert(IntTrieNode *node, const char *key, int value)
{
    if (*key == '\0') {
        if (node->num_children > 0) {
            /* new key is prefix of an existing key */
            return INTTRIE_AMBIGUOUS;
        } else if (node->leaf) {
            return INTTRIE_EXISTS;
        } else {
            node->value = value;
            node->leaf = 1;
            return INTTRIE_OK;
        }
    } else {
        if (node->leaf) {
            /* existing key is prefix of the new key */
            return INTTRIE_AMBIGUOUS;
        }

        /* find the correct child node or create it if it doesn't exist yet */
        IntTrieNode *child;
        int low = 0;
        int mid;
        int high = node->num_children - 1;
        while (low <= high) {
            mid = low + ((high - low) / 2);
            char c = node->children[mid]->c;
            if (c == *key) {
                return node_insert(node->children[mid], ++key, value);
            } else if (c < *key) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        /* node not found; move other nodes and create new node */
        if (node->num_children == node->children_size) {
            if (node->children_size == 0) {
                node->children_size = 1;
            } else {
                node->children_size *= 2;
            }
            node->children =
                    realloc(node->children,
                            node->children_size * sizeof(*node->children));
        }
        for (size_t i = node->num_children; i > low; --i) {
            node->children[i] = node->children[i-1];
        }
        node->children[low] = node_init(*key);
        node->num_children += 1;
        return node_insert(node->children[low], ++key, value);
    }
}

/*
 * Recursively finds a value in a trie.
 */
static IntTrieStatus node_find(const IntTrieNode *node,
                               const char *key, int *value)
{
    if (*key == '\0') {
        if (!node->leaf) {
            return INTTRIE_PARTIAL_MATCH;
        } else {
            *value = node->value;
            return INTTRIE_OK;
        }
    } else {
        if (node->leaf) {
            return INTTRIE_NOT_FOUND;
        }

        /* find the correct child node or create it if it doesn't exist yet */
        IntTrieNode *child;
        int low = 0;
        int mid;
        int high = node->num_children - 1;
        while (low <= high) {
            mid = low + ((high - low) / 2);
            char c = node->children[mid]->c;
            if (c == *key) {
                return node_find(node->children[mid], ++key, value);
            } else if (c < *key) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        return INTTRIE_NOT_FOUND;
    }
}

IntTrie *inttrie_init(void)
{
    IntTrie *trie = malloc(sizeof(*trie));
    if (trie == NULL) {
        goto trie_init_failed;
    }

    /* root is a "dummy" node */
    trie->root = node_init('\0');
    if (trie->root == NULL) {
        goto root_init_failed;
    }

    return trie;

root_init_failed:
    free(trie);

trie_init_failed:
    return NULL;
}

void inttrie_free(IntTrie *trie)
{
    node_free(trie->root);
    free(trie);
}

IntTrieStatus inttrie_insert(IntTrie *trie, const char *key, int value)
{
    return node_insert(trie->root, key, value);
}

IntTrieStatus inttrie_find(IntTrie *trie, const char *key, int *value)
{
    return node_find(trie->root, key, value);
}
