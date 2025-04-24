#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10          /* big enough for queue / stack */
#define LOGSIZE 6        /* fixed log size              */
#define LEN 20           /* max chars in any string     */

/* ------------- helper to show an array of strings -------------- */
void showRange(char arr[][LEN], int first, int last, const char* tag)
{
    int i;
    printf("%s: [", tag);
    if (first == -1) 
    {            /* empty */
        printf("empty");
    } else 
    {
        for (i = first; i <= last; i++) 
        {
            printf("%s", arr[i]);
            if (i < last) 
                printf(", ");
        }
    }
    printf("]\n");
}

// Part (a)
char q[SIZE][LEN];
int  front = -1, rear = -1;

char st[SIZE][LEN];
int  top = -1;

void enqueue(const char name[])
{
    rear = rear + 1;
    if (front == -1)
        front = 0;
    strcpy(q[rear], name);
    showRange(q, front, rear, "QUEUE");
}

char* dequeue(void)
{
    char* ret = q[front];
    front = front + 1;
    if (front > rear) 
        { front = rear = -1; }
    showRange(q, front, rear, "QUEUE");
    return ret;
}

void push(const char name[])
{
    top = top + 1;
    strcpy(st[top], name);
    showRange(st, 0, top, "STACK");
}

char* pop(void)
{
    char* ret = st[top];
    top = top - 1;
    if (top < 0) 
        printf("STACK: [empty]\n");
    else         
        showRange(st, 0, top, "STACK");
    return ret;
}

//Part (b) 
char logArr[LOGSIZE][LEN];
int  start = 0;            /* oldest */
int  count = 0;            /* how many used */

void showLog(void)
{
    int i;
    printf("LOG  : [");
    if (count == 0) {
        printf("empty");
    } else {
        for (i = 0; i < count; i++) {
            int idx = (start + i) % LOGSIZE;
            printf("%s", logArr[idx]);
            if (i < count - 1) printf(", ");
        }
    }
    printf("]\n");
}

void addLog(const char mission[])
{
    if (count < LOGSIZE) {
        int pos = (start + count) % LOGSIZE;
        strcpy(logArr[pos], mission);
        count = count + 1;
    } else {
        printf("-> removing oldest %s\n", logArr[start]);
        strcpy(logArr[start], mission);
        start = (start + 1) % LOGSIZE;
    }
    showLog();
}

// Part (c)  : Damaged SLL   +   Repaired DLL
struct SNode {
    char name[LEN];
    struct SNode* next;
};

struct DNode {
    char name[LEN];
    struct DNode* next;
    struct DNode* prev;
};

struct SNode* Shead = NULL;   /* damaged list */
struct DNode* Dhead = NULL;   /* repaired list */

void addDamaged(const char name[])
{
    struct SNode* n = (struct SNode*)malloc(sizeof(struct SNode));
    strcpy(n->name, name);
    n->next = Shead;
    Shead = n;

    /* show SLL */
    printf("DAMAGED SLL: ");
    struct SNode* t = Shead;
    while (t != NULL) 
        { printf("%s ", t->name); t = t->next; }
    printf("\n");
}

void moveToRepaired(const char name[])
{
    struct SNode* cur = Shead;
    struct SNode* prev = NULL;

    while (cur != NULL && strcmp(cur->name, name) != 0) 
    {
        prev = cur;
        cur = cur->next;
    }
    if (cur == NULL) 
    {   
        printf("Robot not in damaged list\n"); 
        return;
    }

    /* unlink from SLL */
    if (prev == NULL) 
        Shead = cur->next;
    else              
        prev->next = cur->next;

    /* add to DLL tail */
    struct DNode* dn = (struct DNode*)malloc(sizeof(struct DNode));
    strcpy(dn->name, name);
    dn->next = NULL;
    dn->prev = NULL;

    if (Dhead == NULL) Dhead = dn;
    else {
        struct DNode* x = Dhead;
        while (x->next != NULL) x = x->next;
        x->next = dn;
        dn->prev = x;
    }
    free(cur);

    /* show lists */
    printf("DAMAGED SLL: ");
    struct SNode* t = Shead;
    while (t != NULL) 
        { 
            printf("%s ", t->name);
            t = t->next; }
    printf("\n");

    printf("REPAIRED DLL forward: ");
    struct DNode* y = Dhead;
    while (y != NULL)
    { 
        printf("%s ", y->name); y = y->next;
    }
    printf("\n");

    printf("REPAIRED DLL backward: ");
    if (Dhead != NULL) 
    {
        y = Dhead;
        while (y->next != NULL)
            y = y->next;  // last node 
        while (y != NULL)
        {
          printf("%s ", y->name);
          y = y->prev;
        }
    }
    printf("\n");
}

// Part (d) 
struct CNode {
    char name[LEN];
    struct CNode* next;
};

struct CNode* Cfront = NULL;
struct CNode* Ctail  = NULL;

void addUrgent(const char name[])
{
    struct CNode* n = (struct CNode*)malloc(sizeof(struct CNode));
    strcpy(n->name, name);

    if (Cfront == NULL) {
        Cfront = Ctail = n;
        n->next = n;
    } else {
        n->next = Cfront;
        Ctail->next = n;
        Ctail = n;
    }

    /* show queue once */
    printf("URGENT C‑LIST: ");
    struct CNode* t = Cfront;
    if (t != NULL) {
        do {
            printf("%s ", t->name);
            t = t->next;
        } 
        while (t != Cfront);
    }
    printf("\n");
}

void traverseUrgentTwice(void)
{
    int i, count = 0;
    struct CNode* t = Cfront;
    printf("Traversed two times: ");
    if (t == NULL) 
    {
        printf("empty\n"); return;
    }
    while (count < 2) 
    {
        printf("%s ", t->name);
        t = t->next;
        if (t == Cfront) 
        {
            count = count + 1;
        }
    }
    printf("\n");
}

int main(void)
{
    int i;
    const char* tasks[6] = {"Scanner","Digger","Lift","Light","Drone","Cutter"};

    //Part (a) 
    printf("\nQueue -> Stack -> LIFO \n");
    for (i = 0; i < 6; i++) {            // enqueued all 
        printf("Enqueue %s\n", tasks[i]);
        enqueue(tasks[i]);
    }
    for (i = 0; i < 6; i++) {            // dequeue to stack 
        printf("Dequeue -> Push\n");
        push(dequeue());
    }
    printf("Pop all for execution order:\n"); //popped all in LIFO order
    for (i = 0; i < 6; i++) {
        printf("Executed the function %s\n", pop());
    }

    // Part (b) 
    printf("\n PART (b)\n");
    for (i = 1; i <= 8; i++) {
        char m[5];
        sprintf(m, "Mis%d", i);
        printf("Add %s\n", m);
        addLog(m);
    }

    //Part (c) 
    printf("\n PART (c)\n");
    addDamaged("Digger");
    addDamaged("Drone");
    printf("Repair Digger (move to DLL)\n");
    moveToRepaired("Digger");

    //Part (d) 
    printf("\n=== PART (d) Urgent Redeploy (circular) ===\n");
    addUrgent("Scanner");
    addUrgent("Lift");
    traverseUrgentTwice();

    return 0;
}
