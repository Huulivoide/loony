/*
 * @file inttrie.h
 * @author dreamyeyed
 *
 * A trie that maps strings to integers.
 */

#pragma once

/*
 * We'll just declare the struct here. The struct's contents need not be known
 * to use the trie.
 */
struct IntTrie;
typedef struct IntTrie IntTrie;

/**
 * Status messages.
 */
typedef enum IntTrieStatus
{
    INTTRIE_OK,                 // no problems
    INTTRIE_NOT_FOUND,          // key was not found in the trie
    INTTRIE_PARTIAL_MATCH,      // key matches a key in the trie only partially
    INTTRIE_AMBIGUOUS,          // the new key is a prefix of another key
                                // (or vice versa)
    INTTRIE_EXISTS              // new key already exists in the trie
} IntTrieStatus;

/*
 * IntTrie methods.
 */

/**
 * Creates a new trie.
 *
 * @return dynamically allocated trie or NULL
 */
IntTrie *inttrie_init(void);

/**
 * Destroys a trie.
 *
 * @param trie trie to destroy
 */
void inttrie_free(IntTrie *trie);

/**
 * Adds a key-value pair to the trie.
 *
 * @param trie
 * @param key string key
 * @param value value assigned to the given key
 * @return INTTRIE_OK if successful, INTTRIE_AMBIGUOUS if key is prefix of an
 * existing key (or vice versa), INTTRIE_EXISTS if key already exists
 */
IntTrieStatus inttrie_insert(IntTrie *trie, const char *key, int value);

/**
 * Looks up a key in a trie.
 *
 * @param trie
 * @param key key to search for
 * @param value address where the value (if found) will be stored
 * @return INTTRIE_OK if key found, INTTRIE_NOT_FOUND if key not found,
 * INTTRIE_PARTIAL_MATCH if key matched only partially
 */
IntTrieStatus inttrie_find(IntTrie *trie, const char *key, int *value);
