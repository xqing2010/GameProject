#ifndef __BASETYPEDEF_H__
#define __BASETYPEDEF_H__

////////////////////////////////////////
//basic type define
////////////////////////////////////////

#if defined(WIN32)
#elif defined(__LINUX__)

#endif

typedef void		   VOID;
typedef int		   	   BOOL;
typedef char		   CHAR;
typedef unsigned char  UCHAR;
typedef short		   SHORT;
typedef unsigned short USHORT;
typedef int			   INT;
typedef unsigned int   UINT;
typedef long		   LONG;
typedef unsigned long  ULONG;
typedef long long      INT64;
typedef float		   FLOAT;


typedef UCHAR		   BYTE;



////////////////////////////////////////
//basic macro define
////////////////////////////////////////

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if(NULL != (x)) { delete (x); (x) = NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) if(NULL != (x)) { delete [](x); (x) = NULL; }
#endif

#ifndef MAX_PATH
#define MAX_PATH 260
#endif


#endif
