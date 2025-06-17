//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: analysePure.cpp
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//

// Include Files
#include <cmath>
#include <string.h>
#include "rt_nonfinite.h"
#include "MixtureFeatureExtract.h"
#include "PureFeatureExtract.h"
#include "analyseMixture.h"
#include "analysePure.h"
#include "autoestibl.h"
#include "baseline_correct.h"
#include "cal_noiselevel.h"
#include "cal_snr.h"
#include "calkafang.h"
#include "cwt1.h"
#include "deri2.h"
#include "fastsmooth.h"
#include "geto_new.h"
#include "offsetCorrect.h"
#include "resolution.h"
#include "resolution_factor.h"
#include "selectitem_m.h"
#include "selectitem_p.h"
#include "zl_fullspectrumcontrast.h"
#include "zl_lsqnonneg1.h"
#include "sum.h"
#include "vvarstd.h"
#include "analyseMixture_rtwutil.h"

// Function Definitions

//
// 输入量sam为数据库中某一纯净物的特征数组，mix为待鉴定纯净物的特殊数组，两个数组行数可能不同，但都是两列
// Arguments    : const double sam[14]
//                const double mix[14]
//                double rt[2]
// Return Type  : void
//
void analysePure(const double sam[14], const double mix[14], double rt[2])
{
  int trueCount;
  int sam_tmp;
  double b_sam[14];
  double b_mix[14];
  int count;
  double mix_match[14];
  double sam_match[14];
  int i;
  int j;
  boolean_T exitg1;
  double y;
  double mix_match1_data[14];
  int loop_ub;
  int b_loop_ub;
  double sam_match1_data[14];
  int subject_size[2];
  double subject_data[7];
  int c_size[2];
  double c_data[7];
  double rt_data[7];
  double b_y;
  int b_rt[1];
  int sam_match1_size[2];
  double b_sam_match1_data[7];
  emxArray_real_T b_rt_data;
  emxArray_real_T c_sam_match1_data;
  emxArray_real_T b_subject_data;
  double result;
  boolean_T x[7];
  emxArray_real_T b_c_data;

  //   lcz：相似度分析，结果为匹配度和比率
  for (trueCount = 0; trueCount < 7; trueCount++) {
    sam_tmp = trueCount << 1;
    b_sam[sam_tmp] = sam[trueCount];
    b_mix[sam_tmp] = mix[trueCount];
    sam_tmp++;
    b_sam[sam_tmp] = sam[trueCount + 7];
    b_mix[sam_tmp] = mix[trueCount + 7];
  }

  //  特征峰信息：位移，强度，半高宽
  //    计算方式如下： 首先计算每个匹配峰的相关系数，乘上两位移根据模糊隶属度函数计算得到的因子，再乘上纯净物该峰强度占对应 
  //    上的特征峰的比例，强度越高，理应影响越大。
  //    计算待识别物和数据库纯净物匹配度，将两相似度相乘。
  //    更改谱峰匹配的阈值为10
  //   找到匹配的峰
  count = -1;
  memset(&mix_match[0], 0, 14U * sizeof(double));
  memset(&sam_match[0], 0, 14U * sizeof(double));
  for (i = 0; i < 7; i++) {
    j = 0;
    exitg1 = false;
    while ((!exitg1) && (j < 7)) {
      if (1 + j != 7) {
        trueCount = j << 1;
        y = b_mix[trueCount] - b_sam[i << 1];
        if ((y <= 12.0) && (y >= -12.0) && (b_mix[(j + 1) << 1] - b_sam[i << 1] >
             12.0) && (b_sam[i << 1] != 0.0)) {
          count++;
          mix_match[count << 1] = b_mix[trueCount];
          sam_match[count << 1] = b_sam[i << 1];
          mix_match[1 + (count << 1)] = b_mix[1 + trueCount];
          sam_match[1 + (count << 1)] = b_sam[1 + (i << 1)];
          exitg1 = true;
        } else if ((y <= 12.0) && (y >= -12.0) && (b_mix[(j + 1) << 1] - b_sam[i
                    << 1] < 12.0) && (b_sam[i << 1] != 0.0)) {
          y = b_sam[i << 1];
          if (std::abs(b_mix[j << 1] - y) < std::abs(b_mix[(j + 1) << 1] - y)) {
            count++;
            mix_match[count << 1] = b_mix[trueCount];
            sam_match[count << 1] = b_sam[i << 1];
            mix_match[1 + (count << 1)] = b_mix[1 + (j << 1)];
            sam_match[1 + (count << 1)] = b_sam[1 + (i << 1)];
          } else {
            count++;
            mix_match[count << 1] = b_mix[(j + 1) << 1];
            sam_tmp = count << 1;
            sam_match[sam_tmp] = b_sam[i << 1];
            sam_tmp++;
            mix_match[sam_tmp] = b_mix[1 + ((j + 1) << 1)];
            sam_match[sam_tmp] = b_sam[1 + (i << 1)];
          }

          exitg1 = true;
        } else {
          j++;
        }
      } else {
        trueCount = i << 1;
        y = b_mix[12] - b_sam[trueCount];
        if ((y <= 12.0) && (y >= -12.0) && (b_sam[trueCount] != 0.0)) {
          count++;
          mix_match[count << 1] = b_mix[12];
          sam_tmp = count << 1;
          sam_match[sam_tmp] = b_sam[trueCount];
          sam_tmp++;
          mix_match[sam_tmp] = b_mix[13];
          sam_match[sam_tmp] = b_sam[1 + trueCount];
          exitg1 = true;
        } else {
          j++;
        }
      }
    }
  }

  if (count + 1 > 0) {
    sam_tmp = 0;
    for (i = 0; i < 14; i++) {
      if (mix_match[i] != 0.0) {
        sam_tmp++;
      }
    }

    j = 0;
    trueCount = 0;
    for (i = 0; i < 14; i++) {
      if (mix_match[i] != 0.0) {
        mix_match1_data[j] = mix_match[i];
        j++;
      }

      if (sam_match[i] != 0.0) {
        trueCount++;
      }
    }

    j = 0;
    for (i = 0; i < 14; i++) {
      if (sam_match[i] != 0.0) {
        sam_match1_data[j] = sam_match[i];
        j++;
      }
    }

    loop_ub = sam_tmp / 2;
    b_loop_ub = trueCount / 2;

    //   构建模糊隶属度函数，模糊范围为±12 cm-1
    //    计算匹配的每个峰的拉曼位移模糊匹配值  subject，
    subject_size[0] = 1;
    subject_size[1] = count + 1;
    sam_tmp = count + 1;
    if (0 <= sam_tmp - 1) {
      memset(&subject_data[0], 0, (unsigned int)(sam_tmp * static_cast<int>
              (sizeof(double))));
    }

    c_size[0] = 1;
    c_size[1] = static_cast<signed char>((count + 1));
    for (i = 0; i <= count; i++) {
      sam_tmp = i << 1;
      c_data[i] = sam_match1_data[sam_tmp] - mix_match1_data[sam_tmp];

      //  a：center point ;  b: circle point
      y = mix_match1_data[sam_tmp] - sam_match1_data[sam_tmp];
      if ((y >= -12.0) && (y <= -6.0)) {
        subject_data[i] = std::cos(3.1415926535897931 * (y + 6.0) / 12.0);
      } else if ((y > -6.0) && (y < 6.0)) {
        subject_data[i] = 1.0;
      } else {
        if ((y >= 6.0) && (y <= 12.0)) {
          subject_data[i] = std::cos(3.1415926535897931 * (y - 6.0) / 12.0);
        }
      }
    }

    // ---计算匹配上的特征峰之间的比率
    for (i = 0; i <= count; i++) {
      sam_tmp = 1 + (i << 1);
      rt_data[i] = sam_match1_data[sam_tmp] / mix_match1_data[sam_tmp];
    }

    // ----根据比率计算其标准差，得到比率相似度rcsd
    // ---- 分别计算正反向匹配系数
    b_y = b_sam[1];

    //  -- 数据库物质
    y = b_mix[1];
    for (sam_tmp = 0; sam_tmp < 6; sam_tmp++) {
      j = 1 + ((sam_tmp + 1) << 1);
      b_y += b_sam[j];
      y += b_mix[j];
    }

    //  -- 待识别物质
    b_rt[0] = static_cast<signed char>((count + 1));
    sam_match1_size[0] = 1;
    sam_match1_size[1] = b_loop_ub;
    for (trueCount = 0; trueCount < b_loop_ub; trueCount++) {
      b_sam_match1_data[trueCount] = sam_match1_data[1 + (trueCount << 1)] / b_y
        * subject_data[trueCount];
    }

    subject_size[0] = 1;
    subject_size[1] = loop_ub;
    for (trueCount = 0; trueCount < loop_ub; trueCount++) {
      subject_data[trueCount] *= mix_match1_data[1 + (trueCount << 1)] / y;
    }

    b_rt_data.data = &rt_data[0];
    b_rt_data.size = &b_rt[0];
    b_rt_data.allocatedSize = 7;
    b_rt_data.numDimensions = 1;
    b_rt_data.canFreeData = false;
    c_sam_match1_data.data = &b_sam_match1_data[0];
    c_sam_match1_data.size = &sam_match1_size[0];
    c_sam_match1_data.allocatedSize = 7;
    c_sam_match1_data.numDimensions = 2;
    c_sam_match1_data.canFreeData = false;
    b_subject_data.data = &subject_data[0];
    b_subject_data.size = &subject_size[0];
    b_subject_data.allocatedSize = 7;
    b_subject_data.numDimensions = 2;
    b_subject_data.canFreeData = false;
    result = 1.0 / (1.0 + vvarstd(&b_rt_data, (int)(signed char)(count + 1))) *
      b_sum(&c_sam_match1_data) * b_sum(&b_subject_data);
    if (result > 0.99) {
      result = 0.9999;
    }

    for (trueCount = 0; trueCount < 7; trueCount++) {
      x[trueCount] = (b_sam[trueCount << 1] > 0.0);
    }

    b_y = x[0];
    for (sam_tmp = 0; sam_tmp < 6; sam_tmp++) {
      b_y += static_cast<double>(x[sam_tmp + 1]);
    }

    if (b_y == 1.0) {
      for (trueCount = 0; trueCount < 7; trueCount++) {
        x[trueCount] = (b_mix[trueCount << 1] > 0.0);
      }

      b_y = x[0];
      for (sam_tmp = 0; sam_tmp < 6; sam_tmp++) {
        b_y += static_cast<double>(x[sam_tmp + 1]);
      }

      if (b_y > 1.0) {
        result *= 0.1;
      }
    }

    b_c_data.data = &c_data[0];
    b_c_data.size = &c_size[0];
    b_c_data.allocatedSize = 7;
    b_c_data.numDimensions = 2;
    b_c_data.canFreeData = false;
    y = b_sum(&b_c_data);
    rt[0] = result;
    rt[1] = rt_roundd_snf(y / static_cast<double>((count + 1)));
  } else {
    rt[0] = 0.0;
    rt[1] = 0.0;
  }
}

//
// File trailer for analysePure.cpp
//
// [EOF]
//
