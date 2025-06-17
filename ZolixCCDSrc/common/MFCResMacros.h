#pragma once
// 类的导入/导出宏定义

#ifdef _USRDLL
#define ZOLIX_DLL_MFC_RES   __declspec(dllexport)
#else
#define ZOLIX_DLL_MFC_RES  __declspec(dllimport)
#endif

// 游戏引擎的命名空间
#define NS_MFC_Res_BEGIN namespace ZOLIX_DLL_MFC_Res {
#define NS_MFC_Res_END	}
#define US_MFC_Res_NS	using namespace ZOLIX_DLL_MFC_Res;


