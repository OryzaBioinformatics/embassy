#ifndef _H_BOOLEAN
#define _H_BOOLEAN 

/*typedef int (*ProcPtr)();*/    /* A pointer to a procedure */

#ifndef NULL
#define NULL  (void *)(0)
#endif

#ifndef EOS
#define EOS  '\0'
#endif

#ifndef Boolean
#define Boolean int
#endif

#ifndef false
#define false     0
#endif

#ifndef true
#define true      1
#endif

/* typedef enum {false, true} Boolean; */

#ifndef Choice
#define Choice int
#endif

#endif
