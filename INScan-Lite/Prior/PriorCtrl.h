#ifndef __PriorCtrl_H__
#define __PriorCtrl_H__

//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus

#ifdef PRIORCTRL_EXPORTS
#define PriorCtrl_API extern "C" __declspec(dllexport)
#else
#define PriorCtrl_API extern "C" __declspec(dllimport)
#endif

#else

#ifdef PRIORCTRL_EXPORTS
#define PriorCtrl_API __declspec(dllexport)
#else
#define PriorCtrl_API __declspec(dllimport)
#endif

#endif

PriorCtrl_API	void	get_dll_ver_prior(char* buff);

PriorCtrl_API	bool	connect_prior(int iPort);

PriorCtrl_API	bool	disconnect_prior();

PriorCtrl_API	bool	get_is_connect_prior();

PriorCtrl_API	int	get_error_prior();

PriorCtrl_API	void	set_steplen_x_prior(int isteplen = 1000);

PriorCtrl_API	void	set_steplen_y_prior(int isteplen = 1000);

PriorCtrl_API	void	set_steplen_z_prior(int isteplen = 10);

PriorCtrl_API	bool	move_left_prior(int istep=1);

PriorCtrl_API	bool	move_right_prior(int istep=1);

PriorCtrl_API	bool	move_forward_prior(int istep=1);

PriorCtrl_API	bool	move_back_prior(int istep=1);

PriorCtrl_API	bool	move_left_forward_prior(int istep=1);

PriorCtrl_API	bool	move_left_back_prior(int istep=1);

PriorCtrl_API	bool	move_right_forward_prior(int istep=1);

PriorCtrl_API	bool	move_right_back_prior(int istep=1);

PriorCtrl_API	bool	move_up_prior(int istep=1);

PriorCtrl_API	bool	move_down_prior(int istep=1);

PriorCtrl_API	bool	move_position_XY_prior(double X, double Y);

PriorCtrl_API	bool	move_position_Z_prior(double Z);

PriorCtrl_API	bool	get_position_prior(double &X, double &Y, double &Z);

PriorCtrl_API	bool	move_zero_xy_prior();

PriorCtrl_API	bool	move_zero_z_prior();

PriorCtrl_API	bool	stop_prior();

PriorCtrl_API	bool	release_prior();

PriorCtrl_API	bool	reset_zero_position_prior();


#endif