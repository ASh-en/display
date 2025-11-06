#pragma once
#ifndef INT16
typedef signed short INT16;
#endif

#ifndef UINT16
typedef unsigned short UINT16;
#endif
#ifndef INT8
typedef signed char INT8;
#endif

#ifndef EOK
#define EOK (0)
#endif // !EOK
#ifndef EMAT_ERROR
#define EMAT_ERROR (-1)
#endif // !EMAT_ERROR

#ifndef SAFE_DELETE
#define SAFE_DELTE(p) do { \
if(NULL != p) { delete p; }\
}while(0)
#endif

const int FOREVER = 1;
