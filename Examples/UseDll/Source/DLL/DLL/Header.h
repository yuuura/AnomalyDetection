#pragma once

#ifdef MESSAGE_EXPORTS
#define CREATEFILE_API __declspec(dllexport)
#else
#define CREATEFILE_API __declspec(dllimport)
#endif
//
#ifdef __cplusplus
extern "C"
{
	CREATEFILE_API void createFileFunc();
}
#endif
