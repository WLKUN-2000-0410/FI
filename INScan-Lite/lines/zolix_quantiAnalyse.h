#ifndef __zolix_quantiAnalyse_H__
#define __zolix_quantiAnalyse_H__

#ifdef __cplusplus

#ifdef QUANTITATIVEANALYSE_EXPORTS
#define QUANTI_ANALYSE_API extern "C" __declspec(dllexport)
#else
#define QUANTI_ANALYSE_API extern "C" __declspec(dllimport)
#endif

#else

#ifdef QUANTITATIVEANALYSE_EXPORTS
#define QUANTI_ANALYSE_API extern "C" __declspec(dllexport)
#else
#define QUANTI_ANALYSE_API extern "C" __declspec(dllimport)
#endif

#endif
//  1.1 预处理
//  extern void resolution(const emxArray_real_T *aa, double y_data[], int y_size[2], double *flag_ns);
QUANTI_ANALYSE_API void dll_Resolution(double *data, int numDimensions, double y_test[1650], double* flag);
//  1.2 PLS计算系数
//  extern void pls_theta_v1(const emxArray_real_T *pureMatrix, const emxArray_real_T *mixtureMatrix, emxArray_real_T *actual_ratio, emxArray_real_T *theta_pls, double *flag);
QUANTI_ANALYSE_API double* pls_theta(double *pureMatrix, int pureNum, double* mixtureMatrix, int mixtureNum, double* actual_ratio, double* theta_numDimensions, double* flag);
QUANTI_ANALYSE_API void delete_theta();
//  1.3 PLS计算结果
//  extern void pls_res_v1(const emxArray_real_T *theta_pls, const double y_test[1650], emxArray_real_T *y_pred);
QUANTI_ANALYSE_API void pls_res(double *theta_pls, int pureNum, int theta_numDimensions, double y_test[1650], double* y_pred);
//  1.4 LSRP计算系数
//extern void lsrp_theta_v1(const emxArray_real_T *pureMatrix, const emxArray_real_T *mixtureMatrix, emxArray_real_T *actual_ratio, emxArray_real_T *theta_lsrp, double *flag);
QUANTI_ANALYSE_API double* lsrp_theta(double *pureMatrix, int pureNum, double* mixtureMatrix, int mixtureNum, double* actual_ratio, double* flag);
//  1.5 LSRP计算结果
// extern void lsrp_res_v1(const emxArray_real_T *pureMatrix, const double y_test[1650], const emxArray_real_T *theta_lsrp, emxArray_real_T *y_pred);
QUANTI_ANALYSE_API void lsrp_res(double* pureMatrix, double *theta_lsrp, int pureNum, double y_test[1650], double* y_pred);

#endif