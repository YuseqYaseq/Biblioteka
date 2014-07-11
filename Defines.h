//Defines.h

#pragma once
/*
#define HANDLE_HRESULT(result)							\
	{													\
		_HANDLE_HRESULT_(result, __LINE__, __FILE__);	\
	}													

#define _HANDLE_HRESULT_(result, line, file)	\
	{											\
		if(FAILED(result))						\
		{										\
		DXTRACE_MSG("Failed function result: " ((char*)result) ", line: " line ", file: " file);			\
		}										\
	}							
	*/
#define RELEASE_COM(com)		\
	{							\
		if(com)					\
		{						\
			com->Release();		\
		}						\
		com = 0;				\
	}							

struct fRect
{
	float top;
	float bottom;
	float left;
	float right;
};