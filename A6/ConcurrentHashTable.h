/**
* This nlist is a node, which stores one command's info in the
hashtable.
* There is one hashtable slot for any hash value. The linked list allows for storing multiple nlist nodes within a slot.
    * The nlist *next field is a pointer to the next node in the linked list.
*/

struct nlist { //table entry

    struct nlist *next; // next entry in chain

    char *name;
    int count;
    pthread_mutex_t nodeLock; // give every node its own lock

};


typedef struct {
    int size;
    struct nlist **bucket;
} ConcurrentHashTable;


struct nlist *lookup(ConcurrentHashTable *, char *);
struct nlist *insert(ConcurrentHashTable *, char *);
ConcurrentHashTable *initializeHashTable(ConcurrentHashTable *);
void freeHashTable(ConcurrentHashTable *);
void printHashTable(ConcurrentHashTable *);
