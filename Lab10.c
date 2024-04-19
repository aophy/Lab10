#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 100

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
    int count; // Number of occurrences
};

// Initializes a trie node
struct TrieNode *createTrieNode() {
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (node) {
        node->isEndOfWord = 0;
        node->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++)
            node->children[i] = NULL;
    }
    return node;
}

// Trie structure
struct Trie {
    struct TrieNode *root;
};

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word) {
    struct TrieNode *current = pTrie->root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (!current->children[index])
            current->children[index] = createTrieNode();
        current = current->children[index];
    }
    current->isEndOfWord = 1;
    current->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct Trie *pTrie, char *word) {
    struct TrieNode *current = pTrie->root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (!current->children[index])
            return 0; // Word not found
        current = current->children[index];
    }
    if (current != NULL && current->isEndOfWord)
        return current->count;
    return 0; // Word not found
}

// Deallocate the trie structure
void deallocateTrie(struct TrieNode *root) {
    if (!root)
        return;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        deallocateTrie(root->children[i]);
    free(root);
}

// Initializes a trie structure
struct Trie *createTrie() {
    struct Trie *trie = (struct Trie *)malloc(sizeof(struct Trie));
    trie->root = createTrieNode();
    return trie;
}

// This function reads words from a dictionary file and stores them in pInWords array
// Returns the number of words read
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    int numWords = 0;
    char word[MAX_WORD_LENGTH];

    while (fscanf(file, "%s", word) != EOF) {
        pInWords[numWords] = strdup(word);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void) {
    char *inWords[256];
    
    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }
    
    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }
    
    // Parse line by line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }
    
    deallocateTrie(pTrie->root);
    free(pTrie);
    
    return 0;
}
