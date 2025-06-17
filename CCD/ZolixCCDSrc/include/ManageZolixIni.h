#pragma once
class CManageZolixIni
{
private:

public:
	CManageZolixIni(void);
	~CManageZolixIni(void);
public:
	
	bool ReadIni();
	bool WriteIni();
};
extern CManageZolixIni *ManageZolixIni;

