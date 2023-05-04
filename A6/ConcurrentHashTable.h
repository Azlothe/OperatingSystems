/**
 * Description: This is a header file for the concurrent hash table. Members and specifications of the hash table entries are here and can be edited here. Include this header file to use the hash table.
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 5/3/2023
 * Creation date: 5/1/2023
 **/



/**
* This nlist is a node that stores in the hashtable a name and its occurrence count.
* There is one hashtable slot for any hash value. The linked list allows for storing multiple nlist nodes within a slot.
    * The nlist *next field is a pointer to the next node in the linked list.
*/

// Note: Changing the struct members may require changes in functions defined within the c file, most notably freeNList and printNList.
struct nlist {

    struct nlist *next; // next entry in chain

    char *name;
    int count;

    // Give every node its own lock. We optimize concurrency by locking individual nodes, not entire lists or entire table.
    pthread_mutex_t nodeLock;

};


// The hash table stores bucket slots of nlists where each slot represents a computed hash value. Each position holds a linked list of nlist nodes.
typedef struct {
    int size;
    struct nlist **bucket;
} ConcurrentHashTable;


// Function prototypes / Interface
struct nlist *lookup(ConcurrentHashTable *, char *);
struct nlist *insert(ConcurrentHashTable *, char *);
ConcurrentHashTable *initializeHashTable(ConcurrentHashTable *);
void freeHashTable(ConcurrentHashTable *);
void printHashTable(ConcurrentHashTable *);
