#ifndef _Included_STDAFX_H
#define _Included_STDAFX_H
#ifdef __cplusplus
extern "C" {
#endif
	//
	// 此函数用来计算当前曲线的背景值
	//  输入y为信号值,numiter控制拟合背景的效果,数值越小越接近原始信号分布，一般取10~30之间，
	//  默认numiter=25
	//  输出为背景信号值
	// Arguments    : emxArray_real_T *y
	//                double numiter
	//                emxArray_real_T *basesignal
	// Return Type  : void
	//
	__declspec(dllexport)bool main_baseline_correct(double* dIn,int nInSize,double nNUMber,double* result,int* nResultSize);
	// Function Definitions

	//
	// 面积计算
	//  输入aa为两列数据，第一列为x，第二列为y
	//  smoothwidth匀滑宽度,奇数,3~31，默认5,smoothtype 匀滑类型，1，2，3 三种模式,默认1
	//  peakgroup峰组，拟合时所需的点数，奇数，范围3~17，默认为7
	//  mode=1是高斯峰型拟合方法
	//  mode=2是直接计算面积方法
	//  输出结果，第一个是纯峰的面积，第二个是背景部分
	//  第三个是总面积，计算出错则全为0
	// Arguments    : const emxArray_real_T *aa
	//                double mode
	//                double smoothwidth
	//                double smoothtype
	//                double peakgroup
	//                double area[3]
	// Return Type  : void
	//
	__declspec(dllexport)bool main_cal_peak_area(double*aa,int nAaSize,double mode,double smoothwidth,
		double smoothtype, double peakgroup,double* area_data);
	// Function Definitions

	//
	// sg匀滑方法
	//  输入y为信号值,niter控制匀滑效果,数值越大匀滑效果越激进，
	//  建议1~50,默认取20
	//  输出为匀滑后的效果
	// Arguments    : const emxArray_real_T *y
	//                double niter
	//                emxArray_real_T *bb
	// Return Type  : void
	//
	__declspec(dllexport)bool main_denoise2(double* y,int ySize,double niter,double*bb,int*bbSize);
	// Function Definitions

	//
	// 计算微分,采用《计算方法引论（第四版）》中page42的公式
	//  dx为x轴数据间隔，当输入dx1为1时，为普通的导数计算，
	//  为其他非零数值时，为微分计算
	//  y为信号值
	// Arguments    : double dx1
	//                const emxArray_real_T *y
	//                emxArray_real_T *b_y1
	// Return Type  : void
	//
	__declspec(dllexport)bool main_differential(double dx1,double*y,int ySize,double*b_y1,int*b_ySize);
	//
	// 普通的滑动匀滑方法
	//  输入Y为一列数据，待匀滑的信号，w为匀滑窗口大小，建议取奇数3~31，type为匀滑方法
	//  If type=1, 矩形窗口 (1次滑动平均)方法
	//  If type=2, 三角窗口 (2次滑动平均)
	//  If type=3, p-Gaussian窗口 (3次滑动平均)
	//  默认w=5，type=1
	//  输出为匀滑后的y值
	// Arguments    : const emxArray_real_T *Y
	//                double w
	//                double type
	//                emxArray_real_T *SmoothY
	// Return Type  : void
	//
	__declspec(dllexport)bool main_fastsmooth(double*y,int ySize,double w,double type,double* smoothY,int* smoothYSize);
	// Function Definitions

	//
	// 傅里叶滤波，滤波器形状：超高斯函数
	//  y为一列数据，原始信号，filterwidth，取值必须为0~1,指频率域中需要过滤掉的高频信息的占比，
	//  建议为0.6~0.9
	//  y1为滤波后的信号
	// Arguments    : emxArray_real_T *y
	//                double filterwidth
	//                emxArray_real_T *b_y1
	// Return Type  : void
	//
	__declspec(dllexport)bool main_fftfilter(double*y,int ySize,double filterwidth,double* by,int* bySize);
	//
	// function P=findpeaksG(x,y,SlopeThreshold,AmpThreshold,peakwidth,smoothwidth,peakgroup,smoothtype)
	//  寻峰函数用来在x,y的信号中定位峰并计算峰的相关信息
	//  输入x,y为数据的x轴单位数据，y轴信号数据，
	//  SlopeThreshold控制寻峰的敏感度，数值大会忽略很小的峰，默认设置为0.0001
	//  AmpThreshold控制寻峰的强度阈值，只有在强度阈值以上的峰才会被寻到，默认设置为0.7*max（y），一般改变0.7这个系数来控制
	//  peakwidth 控制寻峰的峰宽阈值，只有在峰宽阈值以上的才会被寻到，默认为0，宽度以x轴单位为准，
	//  smoothwidth,smoothtype这2个参数控制匀滑效果，默认设置为7，3,
	//  smoothwidth建议取奇数3~31，smoothtype只能取1，2，3
	//  peakgroup建议取取奇数3~31，默认设置为7
	// Arguments    : const emxArray_real_T *x
	//                const emxArray_real_T *y
	//                double SlopeThreshold
	//                double AmpThreshold
	//                double peakwidth
	//                double smoothwidth
	//                double smoothtype
	//                double peakgroup
	//                emxArray_real_T *P
	// Return Type  : void
	//
	__declspec(dllexport)bool main_findpeaksG(
						double* x,
						int xSize,
						double* y,
						int ySize ,
						double SlopeThreshold,
		                double AmpThreshold,
		                double peakwidth,
		                double smoothwidth,
		                double smoothtype,
		                double peakgroup,
						double * p,
						double* pSize);

	// Function Definitions

	//
	// 直接法计算一个独立峰的半高全宽
	//  输入aa为两列数据，第一列为x，第二列为y值
	//  输出为此峰的半高全宽值
	//  注意：：：：如果输入的数据有多个峰，默认仅计算最高峰的半高全宽
	// Arguments    : const emxArray_real_T *aa
	//                double FWHM_data[]
	//                int FWHM_size[1]
	// Return Type  : void
	//
	__declspec(dllexport)bool main_halfwidth(double*aa,int aaSize,double*FWHM_data,int*FWHM_size);
	// Function Definitions

	//
	// 离查标准化，使结果映射到[0,1]之间
	//  输入Y为一列数据，待归一化的信号
	// Arguments    : const emxArray_real_T *y
	//                emxArray_real_T *b_y1
	// Return Type  : void
	//
	__declspec(dllexport)bool main_MinMaxNormalization(double* y,int ySize,double* by,int*bySize);
	// Function Definitions

	//
	// 使用原始数据的平均值和标准差进行数据的标准化
	//  输入Y为一列数据，待归一化的信号
	// Arguments    : const emxArray_real_T *y
	//                emxArray_real_T *b_y1
	// Return Type  : void
	//
	__declspec(dllexport)bool main_Zscore(double* y,int ySize,double* by,int* bySize);

	//手动标峰函数
	//double* x 整条谱线x 
	//double* y 整条谱线y 
	//int dataSize 整条谱线 
	//double dpos 峰位
	//double* xx 返回截取峰的谱线xx 
	//double* yy 返回截取峰的谱线yy
	//peakLen 返回截取峰的谱线长度
	__declspec(dllexport)bool main_GetPeakRange(double* x,
												double* y,
												int dataSize,
												double dpos,
												double smoothwidth,
												double smoothtype,
												double* xx,
												double* yy,
												double* peakLen);

#ifdef __cplusplus
} 
#endif
#endif