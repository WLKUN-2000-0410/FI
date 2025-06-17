//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: analyseMixture.cpp
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//

// Include Files
#include <cmath>
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
#include "analyseMixture_emxutil.h"
#include "sum.h"
#include "analyseMixture_rtwutil.h"

// Function Definitions

//
// Arguments    : const double sam[14]
//                const emxArray_real_T *mix
//                double res[2]
// Return Type  : void
//
void analyseMixture(const double sam[14], const emxArray_real_T *mix, double
                    res[2])
{
  int i0;
  emxArray_real_T *b_mix;
  int max1;
  double b_sam[14];
  int i1;
  unsigned int count;
  emxArray_real_T *mix_match;
  int sz_idx_1;
  emxArray_real_T *sam_match;
  int i;
  emxArray_real_T *subject;
  int exitg1;
  double a;
  emxArray_int32_T *r0;
  emxArray_real_T *mix_match1;
  emxArray_int32_T *r1;
  emxArray_real_T *sam_match1;
  emxArray_real_T *c;
  double b;

  //   lcz:分析纯净物和混合物的相似度，得到相似度，位移量,输入为纯净物和混合物的峰位峰强信息 
  //   12-12  6-6  cos  40
  if (mix->size[0] != 0) {
    for (i0 = 0; i0 < 7; i0++) {
      max1 = i0 << 1;
      b_sam[max1] = sam[i0];
      b_sam[1 + max1] = sam[i0 + 7];
    }

    emxInit_real_T(&b_mix, 2);
    i0 = b_mix->size[0] * b_mix->size[1];
    b_mix->size[0] = 2;
    b_mix->size[1] = mix->size[0];
    emxEnsureCapacity_real_T(b_mix, i0);
    max1 = mix->size[0];
    for (i0 = 0; i0 < max1; i0++) {
      i1 = i0 << 1;
      b_mix->data[i1] = mix->data[i0];
      b_mix->data[1 + i1] = mix->data[i0 + mix->size[0]];
    }

    //     %% 特征峰信息：位移，强度，半高宽
    //    计算方式如下： 首先计算每个匹配峰的相关系数，乘上两位移根据模糊隶属度函数计算得到的因子，再乘上纯净物该峰强度占对应 
    //    上的特征峰的比例，强度越高，理应影响越大。
    //    计算匹配峰的相关系数方法：首先需要将对应的强度求权重
    //     %%  找到匹配的峰
    count = 0U;
    max1 = b_mix->size[1];
    if (7 > max1) {
      max1 = 7;
    }

    emxInit_real_T(&mix_match, 2);
    i0 = mix_match->size[0] * mix_match->size[1];
    mix_match->size[0] = 2;
    mix_match->size[1] = max1;
    emxEnsureCapacity_real_T(mix_match, i0);
    sz_idx_1 = max1 << 1;
    for (i0 = 0; i0 < sz_idx_1; i0++) {
      mix_match->data[i0] = 0.0;
    }

    emxInit_real_T(&sam_match, 2);
    i0 = sam_match->size[0] * sam_match->size[1];
    sam_match->size[0] = 2;
    sam_match->size[1] = max1;
    emxEnsureCapacity_real_T(sam_match, i0);
    for (i0 = 0; i0 < sz_idx_1; i0++) {
      sam_match->data[i0] = 0.0;
    }

    for (i = 0; i < 7; i++) {
      max1 = 0;
      do {
        exitg1 = 0;
        i0 = b_mix->size[1] - 1;
        if (max1 <= i0) {
          i0 = b_mix->size[1];
          if (1 + max1 != i0) {
            if ((b_mix->data[max1 << 1] - b_sam[i << 1] <= 12.0) && (b_mix->
                 data[max1 << 1] - b_sam[i << 1] >= -12.0) && (b_mix->data[(max1
                  + 1) << 1] - b_sam[i << 1] > 12.0)) {
              count++;

              //                  mix_match=[mix_match,mix(:,j)];
              //                  sam_match=[sam_match,sam(:,i)];
              mix_match->data[(static_cast<int>(count) - 1) << 1] = b_mix->
                data[max1 << 1];
              mix_match->data[1 + ((static_cast<int>(count) - 1) << 1)] =
                b_mix->data[1 + (max1 << 1)];
              sam_match->data[(static_cast<int>(count) - 1) << 1] = b_sam[i << 1];
              sam_match->data[1 + ((static_cast<int>(count) - 1) << 1)] = b_sam
                [1 + (i << 1)];
              exitg1 = 1;
            } else if ((b_mix->data[max1 << 1] - b_sam[i << 1] <= 12.0) &&
                       (b_mix->data[max1 << 1] - b_sam[i << 1] >= -12.0) &&
                       (b_mix->data[(max1 + 1) << 1] - b_sam[i << 1] < 12.0)) {
              a = b_sam[i << 1];
              if (std::abs(b_mix->data[max1 << 1] - a) < std::abs(b_mix->data
                   [(max1 + 1) << 1] - a)) {
                count++;

                //                  mix_match(:,count)=mix(:,j);
                //                  sam_match(:,count)=sam(:,i);
                mix_match->data[(static_cast<int>(count) - 1) << 1] =
                  b_mix->data[max1 << 1];
                mix_match->data[1 + ((static_cast<int>(count) - 1) << 1)] =
                  b_mix->data[1 + (max1 << 1)];
                sam_match->data[(static_cast<int>(count) - 1) << 1] = b_sam[i <<
                  1];
                sam_match->data[1 + ((static_cast<int>(count) - 1) << 1)] =
                  b_sam[1 + (i << 1)];
              } else {
                count++;
                mix_match->data[(static_cast<int>(count) - 1) << 1] =
                  b_mix->data[(max1 + 1) << 1];
                mix_match->data[1 + ((static_cast<int>(count) - 1) << 1)] =
                  b_mix->data[1 + ((max1 + 1) << 1)];
                sam_match->data[(static_cast<int>(count) - 1) << 1] = b_sam[i <<
                  1];
                sam_match->data[1 + ((static_cast<int>(count) - 1) << 1)] =
                  b_sam[1 + (i << 1)];
              }

              exitg1 = 1;
            } else {
              max1++;
            }
          } else {
            i0 = max1 << 1;
            i1 = i << 1;
            if ((b_mix->data[i0] - b_sam[i1] <= 12.0) && (b_mix->data[i0] -
                 b_sam[i << 1] >= -12.0)) {
              count++;
              max1 = (static_cast<int>(count) - 1) << 1;
              mix_match->data[max1] = b_mix->data[i0];
              mix_match->data[1 + ((static_cast<int>(count) - 1) << 1)] =
                b_mix->data[1 + i0];
              sam_match->data[max1] = b_sam[i1];
              sam_match->data[1 + max1] = b_sam[1 + i1];
              exitg1 = 1;
            } else {
              max1++;
            }
          }
        } else {
          exitg1 = 1;
        }
      } while (exitg1 == 0);
    }

    emxFree_real_T(&b_mix);
    emxInit_real_T(&subject, 2);

    //   mix_match : 匹配的混合物信息
    //     %%  由匹配峰的个数计算的比值
    //  c1=sqrt(count/size(sam,2));
    //     %%  构建模糊隶属度函数，模糊范围为±12 cm-1  换为10看效果
    //    计算匹配的每个峰的拉曼位移模糊匹配值  subject，以及偏移量
    i0 = subject->size[0] * subject->size[1];
    subject->size[0] = 1;
    subject->size[1] = static_cast<int>(count);
    emxEnsureCapacity_real_T(subject, i0);
    max1 = static_cast<int>(count);
    for (i0 = 0; i0 < max1; i0++) {
      subject->data[i0] = 0.0;
    }

    sz_idx_1 = (mix_match->size[1] << 1) - 1;
    max1 = 0;
    for (i = 0; i <= sz_idx_1; i++) {
      if (mix_match->data[i] != 0.0) {
        max1++;
      }
    }

    emxInit_int32_T(&r0, 1);
    i0 = r0->size[0];
    r0->size[0] = max1;
    emxEnsureCapacity_int32_T(r0, i0);
    max1 = 0;
    for (i = 0; i <= sz_idx_1; i++) {
      if (mix_match->data[i] != 0.0) {
        r0->data[max1] = i + 1;
        max1++;
      }
    }

    emxInit_real_T(&mix_match1, 1);
    i0 = mix_match1->size[0];
    mix_match1->size[0] = r0->size[0];
    emxEnsureCapacity_real_T(mix_match1, i0);
    max1 = r0->size[0];
    for (i0 = 0; i0 < max1; i0++) {
      mix_match1->data[i0] = mix_match->data[r0->data[i0] - 1];
    }

    emxFree_real_T(&mix_match);
    sz_idx_1 = (sam_match->size[1] << 1) - 1;
    max1 = 0;
    for (i = 0; i <= sz_idx_1; i++) {
      if (sam_match->data[i] != 0.0) {
        max1++;
      }
    }

    emxInit_int32_T(&r1, 1);
    i0 = r1->size[0];
    r1->size[0] = max1;
    emxEnsureCapacity_int32_T(r1, i0);
    max1 = 0;
    for (i = 0; i <= sz_idx_1; i++) {
      if (sam_match->data[i] != 0.0) {
        r1->data[max1] = i + 1;
        max1++;
      }
    }

    emxInit_real_T(&sam_match1, 1);
    i0 = sam_match1->size[0];
    sam_match1->size[0] = r1->size[0];
    emxEnsureCapacity_real_T(sam_match1, i0);
    max1 = r1->size[0];
    for (i0 = 0; i0 < max1; i0++) {
      sam_match1->data[i0] = sam_match->data[r1->data[i0] - 1];
    }

    emxFree_real_T(&sam_match);
    sz_idx_1 = r1->size[0] / 2;
    emxFree_int32_T(&r1);
    emxFree_int32_T(&r0);
    if (static_cast<int>(count) > 0) {
      emxInit_real_T(&c, 1);
      i0 = static_cast<int>(count);
      i1 = c->size[0];
      c->size[0] = static_cast<int>(count);
      emxEnsureCapacity_real_T(c, i1);
      for (i = 0; i < i0; i++) {
        max1 = i << 1;
        a = sam_match1->data[max1];
        b = mix_match1->data[max1];
        c->data[i] = a - b;

        //  a：center point ;  b: circle point
        a = b - a;
        if ((a >= -12.0) && (a <= -6.0)) {
          subject->data[i] = std::cos(3.1415926535897931 * (a + 6.0) / 12.0);
        } else if ((a > -6.0) && (a < 6.0)) {
          subject->data[i] = 1.0;
        } else {
          if ((a >= 6.0) && (a <= 12.0)) {
            subject->data[i] = std::cos(3.1415926535897931 * (a - 6.0) / 12.0);
          }
        }
      }

      //      ratio=min(mix_match(2,:)./sam_match(2,:));
      a = b_sam[1];
      for (max1 = 0; max1 < 6; max1++) {
        a += b_sam[1 + ((max1 + 1) << 1)];
      }

      i0 = subject->size[0] * subject->size[1];
      subject->size[0] = 1;
      subject->size[1] = sz_idx_1;
      emxEnsureCapacity_real_T(subject, i0);
      for (i0 = 0; i0 < sz_idx_1; i0++) {
        subject->data[i0] *= sam_match1->data[1 + (i0 << 1)] / a;
      }

      b = b_sum(subject);

      //          if (length(sam_loc)<=4 && result<0.9) || (length(sam_loc)<=3 && count<=2) % 纯净物谱峰个数小于等属于4， 
      //              result = 0.75*result;
      //          end
      //          if max(sam_match(2,:)) == 1
      //                  if mix_match(2,sam_match(2,:) == 1) <=0.1
      //                      result = 0.75*result;
      //                  end
      //          end
      if (b > 0.99) {
        b = 0.9999;
      }

      a = sum(c);
      res[0] = b;
      res[1] = rt_roundd_snf(a / static_cast<double>(count));
      emxFree_real_T(&c);
    } else {
      res[0] = 0.0;
      res[1] = 0.0;
    }

    emxFree_real_T(&sam_match1);
    emxFree_real_T(&mix_match1);
    emxFree_real_T(&subject);
  } else {
    res[0] = 0.0;
    res[1] = 0.0;
  }
}

//
// File trailer for analyseMixture.cpp
//
// [EOF]
//
