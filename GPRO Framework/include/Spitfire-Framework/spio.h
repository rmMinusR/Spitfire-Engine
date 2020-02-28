#pragma once

#ifdef __cplusplus
extern "C"
{
#endif


	//void sprintarr(unsigned char x[]); //UNIMPL'D

	unsigned char ascol(bool r, bool g, bool b, bool light);
	void csetcolb(unsigned char col);
	void csetcolc(bool r, bool g, bool b, bool light);

	char cquerych();
	//char* cquerystr(); //UNIMPL'D
	
#ifdef __cplusplus
}
#endif