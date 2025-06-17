#ifndef __zolix_cal_fit_H__
#define __zolix_cal_fit_H__

#ifdef __cplusplus

#ifdef CAL_FIT_EXPORTS
#define CAL__FIT_API extern "C" __declspec(dllexport)
#else
#define CAL__FIT_API extern "C" __declspec(dllimport)
#endif

#else

#ifdef CAL_FIT_EXPORTS
#define CAL__FIT_API extern "C" __declspec(dllexport)
#else
#define CAL__FIT_API extern "C" __declspec(dllimport)
#endif

#endif
//  1. 平滑
CAL__FIT_API void dll_SG_Smooth(double *yl_data, int numDimensions, int times, double* yl_smoothed);
//  2. 计算半高宽和峰面积
// data：XY数据；numDimensions：数据点个数；peaks：谱峰峰位；numDimensions2：峰位个数；flag：XY数据为空，或者为匹配到峰，或者匹配的峰的偏差平均值大于等于12cm-1
// 输出：double[numDimensions2 * 2]，前半为峰半高宽，后半为峰面积
CAL__FIT_API double* dll_calFwhmArea(double *data, int numDimensions, double *peaks, int numDimensions2, double& flag);
CAL__FIT_API void dll_deleteFwhmArea();
//  3. 三次多项式拟合校正系数
// x：像素值数组；y：波长或波数数组；numDimensions：数据点个数，返回double[4]
CAL__FIT_API double* dll_fit3(double *x, double *y, int numDimensions);
CAL__FIT_API void dll_deleteCorrPara();
//  4. 根据一系列点进行高斯拟合，返回拟合得到的谱峰
CAL__FIT_API double dll_realPeak(double *aa, int numDimensions);
//  5. 基线拟合
CAL__FIT_API void dll_baseline_correct(double *yl_data, int numDimensions, int times, double* yl_corrected);
//  6. 自动寻峰
//  extern void findPeaksZ(const emxArray_real_T *data, double IntThreshold, double ProminenceThresholde, emxArray_real_T *peaks_info, double *peaks_num);
CAL__FIT_API double* dll_findPeaksG(double *data, int numDimensions, double IntThreshold, double ProminenceThresholde, double* peaks_num);
CAL__FIT_API void dll_deletePeakInfo();
//  7. 手动寻峰
//  extern void manualMarkPeak(const emxArray_real_T *aa_prepro, double xMax, double IntThreshold, double ProminenceThresholde, double peakXY[2], double *flag);
CAL__FIT_API void dll_markPeak(double *data, int numDimensions, double xMax, double IntThreshold, double ProminenceThresholde, double peakXY[4], double *flag);
//  8. 高斯拟合，得到峰位、峰强、半高宽
CAL__FIT_API void dll_GuassFitAll( double *data, int numDimensions, double *pos, double *Height, double *Width );
#endif