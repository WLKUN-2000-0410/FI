#pragma once

#pragma warning(push)
#pragma warning (disable : 4146)

#import "C:\\Program Files (x86)\\Prior Scientific\\Prior Software\\Prior.dll"  no_namespace named_guids 

#pragma warning(pop)

class CImportPriorDll
{
public:
	CImportPriorDll(void);
	~CImportPriorDll(void);

	bool	connect_prior(long iPort);

	bool	disconnect_prior();

	bool	get_is_connect();

	int	get_error();

	bool	move_left(int istep=1);

	bool	move_right(int istep=1);

	bool	move_forward(int istep=1);

	bool	move_back(int istep=1);

	bool	move_left_forward(int istep=1);

	bool	move_left_back(int istep=1);

	bool	move_right_forward(int istep=1);

	bool	move_right_back(int istep=1);

	bool	move_up(int istep=1);

	bool	move_down(int istep=1);

	bool	move_position(double X, double Y);

	bool	get_position(double &X, double &Y);

	bool	move_zero_xy();

	bool	move_zero_z();

	bool	stop();


private:
	IScanPtr pScan;
	IStagePtr pStage;
	IPiezoPtr pCPiezo;
	bool m_bConnect;
	double m_fCurX;
	double m_fCurY;
	int m_iError;
};

