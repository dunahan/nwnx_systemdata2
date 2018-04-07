// 
//

#if !defined(SYSTEMDATA_H_)
#define SYSTEMDATA_H_

#include "..\NWNXDLL\NWNXBase.h"

typedef unsigned long dword;
typedef unsigned short int word;
typedef unsigned char byte;

class CNWNXSystemData2 : public CNWNXBase  
{
public:
	CNWNXSystemData2();
	virtual ~CNWNXSystemData2();

	char* OnRequest(char *gameObject, char* Request, char* Parameters);
	BOOL OnCreate(const char* LogDir);
	BOOL OnRelease();

	void WriteLogHeader();
};

#endif