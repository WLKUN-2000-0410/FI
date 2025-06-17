#pragma once
#include "stdafx.h"
#include <iostream>
using namespace std;
//镜像函数，对数据进行水平翻转
template<class T1,class T2>
void CIsMirror(T1 sMirror,T1 nPixSize,T2 *pSur,T2 *pDst)
{
	if (sMirror==1)
	{
		reverse(pSur,(pSur+nPixSize));
	}
		memcpy(pDst,pSur,sizeof(double)*nPixSize);

}
template<class T1,class T2,class T3>
void Float2Long(T1 *Src,T2 *Dst,T3 nPixSize)
{
	for (int i=0;i<nPixSize;i++)
	{
		Dst[i]=(T1)Src[i];
	}
} 
template<class T1,class T2,class T3>
T1 * CamPixProcessing(T1 *intensityData,T2 MaxSize,T2 iCCDNUM,T3 strCCD)
{
	CString strvalue;
	int num=0;
	T1 first=0;
	int m_Num=0;
	BOOL nSt=TRUE;
	CStringArray sGataWave;
	if (iCCDNUM>0)
	{
		sGataWave.RemoveAll();
		Split(strCCD,";",sGataWave);
		for (int i=0;i<sGataWave.GetSize();i++)
		{
			if (i>=(sGataWave.GetSize()-1))
			{
				nSt=FALSE;
			}
			while (nSt&&(atoi(sGataWave.GetAt(i))+1)==atoi(sGataWave.GetAt(i+1)))
			{
				num++;
				i++;
				if (i==(sGataWave.GetSize()-1))
				{
					nSt=FALSE;
				}
			}

			if (num==0)
			{
				if (atoi(sGataWave.GetAt(i))<MaxSize-1)
				{
					if(atoi(sGataWave.GetAt(i))==0)
					{
						first=intensityData[1];
					}
					else
					{
						first=intensityData[atoi(sGataWave.GetAt(i))-1];
					}
					intensityData[atoi(sGataWave.GetAt(i))]=(first+intensityData[atoi(sGataWave.GetAt(i))+1])/2;
				}
				else
				{
					intensityData[atoi(sGataWave.GetAt(i))]=(intensityData[atoi(sGataWave.GetAt(i))-2]+intensityData[atoi(sGataWave.GetAt(i))-1])/2;
				}
			}
			else
			{
				m_Num=num;
				while(m_Num!=-1)
				{
					if (atoi(sGataWave.GetAt(i))<MaxSize-1)
					{
						if((atoi(sGataWave.GetAt(i))-num)==0)
						{
							first=0.0;
						}
						else
						{
							first=intensityData[atoi(sGataWave.GetAt(i))-num-1];
						}
						intensityData[atoi(sGataWave.GetAt(i))-m_Num]=first+(num+1-m_Num)*(intensityData[atoi(sGataWave.GetAt(i))+1]-first)/(num+2);
					}
					else
					{
						intensityData[atoi(sGataWave.GetAt(i))-m_Num]=(intensityData[atoi(sGataWave.GetAt(i))-m_Num-2]+intensityData[atoi(sGataWave.GetAt(i))-m_Num-1])/2;

					}
					m_Num--;
				}
			}
			num=0;
		}
	}	
	return intensityData;
}

template <typename T>  // 直线版本
void RemoveGhosting1D(T* buffer, int size, const CString& ghostRegionStr)
{
	if (ghostRegionStr.IsEmpty()) {
		return;
	}

	int curPos = 0;
	while (curPos < ghostRegionStr.GetLength()) {
		CString region = ghostRegionStr.Tokenize(_T(";"), curPos);
		if (region.IsEmpty()) continue;

		int commaPos = region.Find(_T(','));
		if (commaPos == -1) continue;

		int x_start = _ttoi(region.Left(commaPos));
		int x_end = _ttoi(region.Mid(commaPos + 1));

		if (x_start < 0 || x_end >= size || x_start >= x_end) {
		
			continue;
		}

		int d = x_end - x_start;
		int leftIdx = x_start - 1;
		int rightIdx = x_end;

		if (leftIdx < 0 || rightIdx >= size) {
		
			continue;
		}

		T leftVal = buffer[leftIdx];
		T rightVal = buffer[rightIdx];

		for (int j = 0; j < d; ++j) {
			int idx = x_start + j;
			buffer[idx] = static_cast<T>(
				leftVal + (rightVal - leftVal) * (j + 1.0) / (d + 1));
		}
	}
}

//template <typename T>  // 非常好用的版本
//void RemoveGhosting1D_CrossFade(T* buffer, int size, const CString& ghostRegionStr)
//{
//	if (ghostRegionStr.IsEmpty()) return;
//
//	int curPos = 0;
//	while (curPos < ghostRegionStr.GetLength()) {
//		CString region = ghostRegionStr.Tokenize(_T(";"), curPos);
//		if (region.IsEmpty()) continue;
//		int commaPos = region.Find(_T(','));
//		if (commaPos == -1) continue;
//
//		int x0 = _ttoi(region.Left(commaPos));
//		int x1 = _ttoi(region.Mid(commaPos + 1));
//		if (x0 < 1 || x1 >= size - 1 || x0 >= x1) continue;
//
//		int d = x1 - x0;
//		// 确保左右两侧都有足够的点来做交叉淡入淡出
//		if (x0 >= d && x1 + d < size) {
//			for (int j = 0; j < d; ++j) {
//				double t = double(j + 1) / double(d + 1);
//				T vL = buffer[x0 - d + j];      // 左侧真实段
//				T vR = buffer[x1 + 1 + j];      // 右侧真实段
//												// 按权重混合
//				double v = vL * (1.0 - t) + vR * t;
//				buffer[x0 + j] = static_cast<T>(v);
//			}
//		}
//		else {
//			// 回退到简单线性插值
//			T leftVal = buffer[x0 - 1];
//			T rightVal = buffer[x1];
//			for (int j = 0; j < d; ++j) {
//				double t = double(j + 1) / double(d + 1);
//				buffer[x0 + j] = static_cast<T>(leftVal + (rightVal - leftVal) * t);
//			}
//		}
//	}
//}
//
//template <typename T> //deep-seek 边界能行
//void RemoveGhosting1D_CrossFade(T* buffer, int size, const CString& ghostRegionStr)
//{
//	if (ghostRegionStr.IsEmpty()) return;
//
//	int curPos = 0;
//	while (curPos < ghostRegionStr.GetLength()) {
//		CString region = ghostRegionStr.Tokenize(_T(";"), curPos);
//		if (region.IsEmpty()) continue;
//		int commaPos = region.Find(_T(','));
//		if (commaPos == -1) continue;
//
//		int x0 = _ttoi(region.Left(commaPos));
//		int x1 = _ttoi(region.Mid(commaPos + 1));
//		if (x0 < 0 || x1 > size || x0 >= x1) continue; // 放宽边界条件检查
//
//		int d = x1 - x0; // 鬼像区域长度
//		if (d <= 0) continue;
//
//		// 计算实际可用的参考区域长度
//		int leftRefLen = x0; // 左侧可用参考点数量
//		int rightRefLen = size - x1; // 右侧可用参考点数量
//
//									 // 情况1：两侧都有足够参考点（>=d）
//		if (leftRefLen >= d && rightRefLen >= d) {
//			for (int j = 0; j < d; ++j) {
//				double t = double(j + 1) / double(d + 1);
//				T vL = buffer[x0 - d + j];      // 左侧参考点
//				T vR = buffer[x1 + j];          // 右侧参考点
//				buffer[x0 + j] = static_cast<T>(vL * (1.0 - t) + vR * t);
//			}
//		}
//		// 情况2：仅右侧有足够参考点
//		else if (rightRefLen >= d) {
//			T leftBoundary = (x0 > 0) ? buffer[x0 - 1] : buffer[x1]; // 左边界值
//			for (int j = 0; j < d; ++j) {
//				double t = double(j + 1) / double(d + 1);
//				T vR = buffer[x1 + j]; // 右侧参考点
//				buffer[x0 + j] = static_cast<T>(leftBoundary * (1.0 - t) + vR * t);
//			}
//		}
//		// 情况3：仅左侧有足够参考点
//		else if (leftRefLen >= d) {
//			T rightBoundary = (x1 < size) ? buffer[x1] : buffer[x0 - 1]; // 右边界值
//			for (int j = 0; j < d; ++j) {
//				double t = double(j + 1) / double(d + 1);
//				T vL = buffer[x0 - d + j]; // 左侧参考点
//				buffer[x0 + j] = static_cast<T>(vL * (1.0 - t) + rightBoundary * t);
//			}
//		}
//		// 情况4：两侧都不够参考点
//		else {
//			T leftVal = (x0 > 0) ? buffer[x0 - 1] : T(0);
//			T rightVal = (x1 < size) ? buffer[x1] : T(0);
//			for (int j = 0; j < d; ++j) {
//				double t = double(j + 1) / double(d + 1);
//				buffer[x0 + j] = static_cast<T>(leftVal * (1.0 - t) + rightVal * t);
//			}
//		}
//	}
//}


#include <vector>       // 必须: 为了使用 std::vector
#include <random>       // 必须: 为了使用 std::mt19937, std::random_device, std::normal_distribution
#include <numeric>      // 必须: 为了使用 std::accumulate, std::inner_product
#include <cmath>        // 必须: 为了使用 std::sqrt
#include <algorithm>    // 必须: 为了使用 std::copy
#include <iostream>     // (可选) 用于调试输出, 如果您的项目使用了预编译头文件(stdafx.h), 
// 请确保这些include位于预编译头之后

// 假设 CString 和 _ttoi 相关的头文件 (如 <afx.h> 或 <atlstr.h>) 已经包含在您的项目中

/**
* @brief 使用上下文噪声建模方法修复一维数据中的多个异常区域("鬼像")。
*        此版本兼容 Visual Studio 2015。
* @tparam T 数据类型 (如 float, double, int)
* @param buffer 指向要处理的数据的指针
* @param size buffer 的大小
* @param ghostRegionStr 一个 CString, 包含一个或多个待修复区域，格式为 "x0,x1;x2,x3;..."
*/
template<typename T>
void RemoveGhosting1D_CrossFade(T* buffer, int size, const CString& ghostRegionStr)
{
	if (ghostRegionStr.IsEmpty() || buffer == nullptr || size <= 0) {
		return;
	}

	// 1. 为了安全和方便地操作，将 C 风格数组复制到 std::vector
	std::vector<T> data(buffer, buffer + size);

	// 2. 创建一个高质量的静态随机数生成器。
	//    'static' 确保它只被初始化一次，即使函数被多次调用。
	//    【关键修改】: 下面的初始化方式对 VS2015 更友好。
	static std::random_device rd;  // 先创建一个 random_device 实例
	static std::mt19937 rng(rd()); // 然后用它来做种子

								   // 3. 复用原有的多区域解析逻辑
	int curPos = 0;
	while (curPos < ghostRegionStr.GetLength()) {
		CString region = ghostRegionStr.Tokenize(_T(";"), curPos);
		if (region.IsEmpty()) continue;
		int commaPos = region.Find(_T(','));
		if (commaPos == -1) continue;

		// 获取待修复区域的起止索引
		int startIndex = _ttoi(region.Left(commaPos));
		int endIndex = _ttoi(region.Mid(commaPos + 1));

		// --- 核心修复算法 (与之前版本相同) ---

		const int context_size = 30;

		// a. 边界检查
		if (startIndex < context_size || endIndex >= size - context_size || startIndex >= endIndex) {
#ifdef _DEBUG
			// 使用 ATL/MFC 的 TRACE 宏或者标准输出进行调试
			TRACE(L"警告: 区域 [%d, %d] 因太靠近数据边界或无效而被跳过。\n", startIndex, endIndex);
#endif
			continue;
		}

		// b. 提取 "纯噪声"
		std::vector<double> local_noise;
		local_noise.reserve(context_size * 2);

		// 分析 "之前" 的上下文
		double trend_start_before = static_cast<double>(data[startIndex - context_size]);
		double trend_end_before = static_cast<double>(data[startIndex - 1]);
		for (int i = 0; i < context_size; ++i) {
			double trend_val = trend_start_before + (trend_end_before - trend_start_before) * (static_cast<double>(i) / (context_size - 1));
			local_noise.push_back(static_cast<double>(data[startIndex - context_size + i]) - trend_val);
		}

		// 分析 "之后" 的上下文
		double trend_start_after = static_cast<double>(data[endIndex + 1]);
		double trend_end_after = static_cast<double>(data[endIndex + context_size]);
		for (int i = 0; i < context_size; ++i) {
			double trend_val = trend_start_after + (trend_end_after - trend_start_after) * (static_cast<double>(i) / (context_size - 1));
			local_noise.push_back(static_cast<double>(data[endIndex + 1 + i]) - trend_val);
		}

		// c. 计算噪声的标准差
		double sum = std::accumulate(local_noise.begin(), local_noise.end(), 0.0);
		double mean = local_noise.empty() ? 0.0 : sum / local_noise.size();
		double sq_sum = std::inner_product(local_noise.begin(), local_noise.end(), local_noise.begin(), 0.0);
		double std_dev = local_noise.empty() ? 0.0 : std::sqrt(sq_sum / local_noise.size() - mean * mean);

		// d. 进行修复
		double repair_start_val = static_cast<double>(data[startIndex - 1]);
		double repair_end_val = static_cast<double>(data[endIndex + 1]);
		int repair_length = endIndex - startIndex + 1;

		std::normal_distribution<double> dist(0.0, std_dev);

		for (int i = 0; i < repair_length; ++i) {
			double baseline = repair_start_val + (repair_end_val - repair_start_val) * (static_cast<double>(i + 1) / (repair_length + 1));
			double noise = dist(rng);
			data[startIndex + i] = static_cast<T>(baseline + noise);
		}
}

	// 4. 将修改后的 std::vector 数据复制回原始的 C 风格数组
	std::copy(data.begin(), data.end(), buffer);
}