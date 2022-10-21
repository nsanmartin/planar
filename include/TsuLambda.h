#ifndef __LAMBDA_H_
#define __LAMBDA_H_

typedef struct Lambda Lambda;

typedef struct Lambda {
    void (*app)(Lambda*, void*);
    void* value;
    void* ctx;
} Lambda;
    
// To be used in for_each iterations
typedef struct LamConsumer LamConsumer;
typedef struct LamConsumer {
    int (*app)(LamConsumer*, void*);
    void* ctx;
} LamConsumer;


typedef struct LamPredicate LamPredicate;
typedef struct LamPredicate {
    int (*app)(LamPredicate*, void*);
    void* ctx;
    bool pred;
} LamPredicate;
#endif
