#ifndef __DFIELD_H__
#define __DFIELD_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DFIELD_EXPORTS
#define DFIELD_API __declspec(dllexport)
#else
#define DFIELD_API __declspec(dllimport)
#endif
	/////////////////////////struct/////////////////////////////////////////////////
	typedef struct _DCCD_DEV_INFO_ {
		char manufacturer[9];
		char model[25];
		char sn[25];
		char date[9];
	} dfield_dev_info, *ptr_dfield_dev_info;
	//////////////////////////enums////////////////////////////////////////////////
	typedef enum _SHUTTER_status_ {
		shutter_off = 0,
		shutter_on,
		shutter_auto,		// During the scanning process, the instrument automatically 
									// controls the shutter action according to the integration time.
	} shutter_status, *ptr_shutter_status;

	typedef enum _DATA_MODE_ {
		mode_normal = 0,
		mode_bin,			// The accumulated number of pixels is determined by the "dfield_set_bin" function.
									// (Increase sensitivity, sacrifice resolution).
		mode_acc,			// The number of accumulations is determined by the "dfield_set_accumulation" function.
									// (Improve signal-to-noise ratio).
		mode_avg,			// The number of accumulations is determined by the "dfield_set_accumulation" function.
		mode_bin_avg,	// The accumulated number of pixels is determined by the "dfield_set_bin" function.
									// (Improve sensitivity, signal-to-noise ratio, sacrifice resolution).
	} data_mode, *ptr_data_mode;

	typedef enum _READOUT_MODE_ {
		mode_fvb = 0,
		mode_image,
	} readout_mode, *ptr_readout_mode;

	typedef enum _READOUT_SPEED_ {
		speed_0 = 0,	// 250KHz
		speed_1,			// 125KHz
		speed_2,			// 62.5KHz
		speed_3,			// 31.25KHz
	}readout_speed, *ptr_readout_speed;

	typedef struct _PIXELS_ROI_ {
		short begin_h;
		short end_h;
		short begin_v;
		short end_v;
	}pixels_roi, *ptr_pixels_roi;

	typedef enum _ERROR_CODE_ {
		OK = 0,
		E00,			// Device is not connected
		E01,			// Communication error, illegal control command sent or communication timeout
		E02,			// EEPROM error or instrument has not set parameters
		E03,			// Invalid parameter - Detector
		E04,			// Invalid parameter - Other
		E05,			// Reserve
		E06,			// Reserve
		E07,			// Auto exposure error, timeout or weak signal
		E08,			// Auto exposure error, signal saturation
		//...			// E09 ~ E19 Reserved.
		E20,			// Unknown device
	}error_code, *ptr_error_code;


	DFIELD_API short dfield_enum_dev_count();
	DFIELD_API bool dfield_enum_dev_sn(char idx, char* buff, int len);

	DFIELD_API void dfield_usb_mode(bool usb);

	DFIELD_API char dfield_open(const char* info);
	DFIELD_API void dfield_close(char id);

	DFIELD_API bool dfield_is_open(char id);
	DFIELD_API void dfield_get_error(char id, ptr_error_code code);

	DFIELD_API void dfield_get_pixel_size(char id, short* width_um, short* height_um);
	DFIELD_API void dfield_get_detector_size(char id, short* width, short* height);

	DFIELD_API bool dfield_get_info(char id, ptr_dfield_dev_info ptr);
	DFIELD_API bool dfield_set_info(char id, dfield_dev_info info);

	DFIELD_API bool dfield_get_eeprom(char id, short offset, char* buff, short buff_len);
	DFIELD_API bool dfield_set_eeprom(char id, short offset, const char* buff, short buff_len);

	DFIELD_API bool dfield_get_baseline_adjust(char id, bool* by_auto);
	DFIELD_API bool dfield_set_baseline_adjust(char id, bool by_auto);

	DFIELD_API bool dfield_get_shutter(char id, ptr_shutter_status state);
	DFIELD_API bool dfield_set_shutter(char id, shutter_status state);

	DFIELD_API bool dfield_get_temperature_range(char id, float* min, float* max);
	DFIELD_API bool dfield_get_temperature(char id, float* temp);
	DFIELD_API bool dfield_set_temperature(char id, float temp);
	DFIELD_API bool dfield_get_real_temperature(char id, float* temp);
	DFIELD_API bool dfield_get_internal_temperature(char id, float* temp);
	DFIELD_API bool dfield_get_external_temperature(char id, float* temp);

	DFIELD_API bool dfield_start_cooling(char id);
	DFIELD_API bool dfield_stop_cooling(char id);

	DFIELD_API bool dfield_get_exposure_range(char id, int* min_us, int* max_us);
	DFIELD_API bool dfield_get_exposure(char id, int* us);
	DFIELD_API bool dfield_set_exposure(char id, int us);

	DFIELD_API bool dfield_get_trigger_mode(char id, bool* on);
	DFIELD_API bool dfield_set_trigger_mode(char id, bool on);

	DFIELD_API bool dfield_get_data_mode(char id, ptr_data_mode mode);
	DFIELD_API bool dfield_set_data_mode(char id, data_mode mode);

	DFIELD_API bool dfield_get_bin(char id, short* bin_h, short* bin_v);
	DFIELD_API bool dfield_set_bin(char id, short bin_h, short bin_v);

	DFIELD_API bool dfield_get_accumulation(char id, short* times);
	DFIELD_API bool dfield_set_accumulation(char id, short times);

	DFIELD_API bool dfield_get_readout_mode(char id, ptr_readout_mode mode);
	DFIELD_API bool dfield_set_readout_mode(char id, readout_mode mode);

	DFIELD_API bool dfield_get_readout_speed(char id, ptr_readout_speed speed);
	DFIELD_API bool dfield_set_readout_speed(char id, readout_speed speed);

	DFIELD_API bool dfield_get_dummy_mode(char id, bool* used);
	DFIELD_API bool dfield_set_dummy_mode(char id, bool used);

	DFIELD_API bool dfield_get_sub_bg_mode(char id, bool* used);
	DFIELD_API bool dfield_set_sub_bg_mode(char id, bool used);

	DFIELD_API bool dfield_get_roi(char id, ptr_pixels_roi roi);
	DFIELD_API bool dfield_set_roi(char id, pixels_roi roi);

	DFIELD_API bool dfield_get_gain(char id, short* gain);
	DFIELD_API bool dfield_set_gain(char id, short gain);

	DFIELD_API bool dfield_get_pga_gain(char id, short* gain);
	DFIELD_API bool dfield_set_pga_gain(char id, short gain);

	DFIELD_API bool dfield_get_ad_offset(char id, short* offset);
	DFIELD_API bool dfield_set_ad_offset(char id, short offset);

	DFIELD_API bool dfield_get_am_offset(char id, short ch, short* offset);
	DFIELD_API bool dfield_set_am_offset(char id, short ch, short offset);

	DFIELD_API bool dfield_get_auto_exposure(char id, bool* in_auto);
	DFIELD_API bool dfield_set_auto_exposure(char id, bool in_auto);

	DFIELD_API bool dfield_get_auto_exp_threshold(char id, short* threshold);
	DFIELD_API bool dfield_set_auto_exp_threshold(char id, short threshold);

	DFIELD_API void dfield_set_flip(char id, bool flip);

	typedef void(*data_callback)(unsigned short* buff, int buff_size, void* ctx);
	DFIELD_API void dfield_set_data_callback(char id, data_callback f, void* ctx);

	DFIELD_API bool dfield_capture(char id);
	DFIELD_API bool dfield_get_data(char id, unsigned short * buff, int buff_size);

	DFIELD_API bool dfield_get_noise(char id, unsigned short* buff, int buff_size);
	DFIELD_API bool dfield_save_noise(char id);
	// 外触发模式生效
	DFIELD_API bool dfield_abort(char id);

	DFIELD_API bool dfield_get_user_data(char id, short addr, unsigned char* buff, int buff_size);
	DFIELD_API bool dfield_set_user_data(char id, short addr, const unsigned char* buff, int buff_size);

	DFIELD_API bool dfield_debug(char id, const char* cmd, short len, char* recv, int* recv_bytes);

#ifdef __cplusplus
}
#endif

#endif