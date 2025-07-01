#ifndef EXTVM_BASE
#define EXTVM_BASE 1

#include <malloc.h>


#ifndef INTI
#define INTI unsigned int
#else
#undef INTI
#define INTI unsigned int
#endif

#ifndef BYTE
#define BYTE unsigned char
#else
#undef BYTE
#define BYTE unsigned char
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif




#define get_X_(_,__,___,____) { INTI _____ = sizeof(_); do {_____=_____-1; *(((BYTE*)(____))+_____)=((*(((BYTE*)(__))+_____))^(*(((BYTE*)(___))+_____))); }while(_____); }
#define get_X(_,__,___,____) {get_X_(_,&__,&___,&____);}


struct _extVM_byteStack
	{
		BYTE b;
		struct _extVM_byteStack *x;
	};

//command tape
#define CT struct _extVM_byteStack


#define CT_new(_) {_=(CT*) malloc(sizeof(CT)); _->b=(BYTE)0; _->x = NULL; }
#define CT_del(_) {if(_){free(_);}}
#define CT_next(_,__,___) { get_X(CT*,_,__,___); }


//byte stack
#define BS CT

#define BS_new(_) {CT_new(_);}
#define BS_del(_) {CT_del(_);}


#endif