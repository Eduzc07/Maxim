//
// File: quadrotor_EKF.cpp
//
// Code generated for Simulink model 'quadrotor_EKF'.
//
// Model version                  : 1.30
// Simulink Coder version         : 8.9 (R2015b) 13-Aug-2015
// C/C++ source code generated on : Mon Oct 10 01:28:14 2016
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "quadrotor_EKF.h"
#include "quadrotor_EKF_private.h"

real_T rt_powd_snf(real_T u0, real_T u1)
{
  real_T y;
  real_T tmp;
  real_T tmp_0;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else {
    tmp = fabs(u0);
    tmp_0 = fabs(u1);
    if (rtIsInf(u1)) {
      if (tmp == 1.0) {
        y = (rtNaN);
      } else if (tmp > 1.0) {
        if (u1 > 0.0) {
          y = (rtInf);
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = (rtInf);
      }
    } else if (tmp_0 == 0.0) {
      y = 1.0;
    } else if (tmp_0 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = (rtNaN);
    } else {
      y = pow(u0, u1);
    }
  }

  return y;
}

void MWDSPCG_Pseudoinverse_D(const real_T pS[], const real_T pU[], const real_T
  pV[], real_T pX[], int32_T M, int32_T N)
{
  real_T tol;
  real_T cabsS;
  int32_T P;
  int32_T rank;
  int32_T ppXIdx;
  int32_T ppUIdx;
  int32_T ppVIdx;
  int32_T j;
  int32_T k;
  int32_T M_0;

  // S-Function (sdsppinv): '<S3>/Pseudoinverse'
  rank = 0;
  if (M <= N) {
    P = M;
  } else {
    P = N;
  }

  cabsS = fabs(pS[0]);
  if (0.0 < cabsS) {
    rank = 1;
    if (M >= N) {
      M_0 = M;
    } else {
      M_0 = N;
    }

    tol = (real_T)M_0 * pS[0U] * 2.2204460492503131E-16;
    M_0 = P;
    while (P > 0) {
      cabsS = fabs(pS[M_0 - 1]);
      M_0--;
      if (cabsS >= tol) {
        rank = P;
        P = 0;
      }

      P--;
    }
  }

  ppXIdx = 0;
  for (P = 0; P < N * M; P++) {
    pX[ppXIdx] = 0.0;
    ppXIdx++;
  }

  if (rank != 0) {
    ppXIdx = 0;
    for (j = 0; j < M; j++) {
      for (P = 0; P < N; P++) {
        ppVIdx = P;
        ppUIdx = j;
        M_0 = 0;
        for (k = 0; k < rank; k++) {
          cabsS = pV[ppVIdx] * pU[ppUIdx] / pS[M_0];
          pX[ppXIdx] += cabsS;
          M_0++;
          ppVIdx += N;
          ppUIdx += M;
        }

        ppXIdx++;
      }
    }
  }

  // End of S-Function (sdsppinv): '<S3>/Pseudoinverse'
}

// Model step function
void quadrotor_EKFModelClass::step()
{
  real_T t2;
  real_T t3;
  real_T t4;
  real_T t5;
  real_T t6;
  real_T t7;
  real_T t8;
  real_T t9;
  real_T t11;
  real_T t12;
  real_T t13;
  real_T t14;
  real_T t15;
  real_T t16;
  real_T t17;
  real_T t18;
  real_T t19;
  real_T t20;
  real_T t21;
  real_T t22;
  real_T t10;
  real_T t24;
  real_T t25;
  real_T t26;
  real_T t27;
  real_T t28;
  real_T t29;
  real_T t30;
  real_T t31;
  real_T t32;
  real_T t33;
  real_T t34;
  real_T t35;
  real_T t36;
  real_T t37;
  real_T t38;
  real_T t39;
  real_T t40;
  real_T t41;
  real_T t42;
  real_T t43;
  real_T t44;
  real_T t45;
  real_T t46;
  real_T t47;
  real_T t48;
  real_T t49;
  real_T t50;
  real_T t51;
  real_T t52;
  real_T t53;
  real_T t54;
  real_T t55;
  real_T t56;
  real_T t57;
  real_T t58;
  real_T t59;
  real_T t60;
  real_T t61;
  real_T t62;
  real_T t63;
  real_T t64;
  real_T t65;
  real_T t66;
  real_T t67;
  real_T t68;
  real_T t69;
  real_T t70;
  real_T t71;
  real_T t72;
  real_T t73;
  real_T t74;
  real_T t75;
  int32_T yElIdx;
  int32_T uElOffset1;
  int32_T ntIdx1;
  int32_T uElOffset0;
  int32_T ntIdx0;
  real_T rtb_f[21];
  real_T rtb_Add[21];
  real_T Pseudoinverse_X[144];
  real_T Pseudoinverse_V[144];
  real_T Pseudoinverse_U[144];
  real_T Pseudoinverse_WORK[12];
  real_T Pseudoinverse_E[12];
  real_T Pseudoinverse_S[12];
  real_T rtb_F_k[441];
  real_T rtb_H_k[252];
  real_T rtb_Product_e[441];
  real_T rtb_Add_a[441];
  real_T rtb_Transpose[252];
  real_T rtb_Product_a[252];
  real_T rtb_Pseudoinverse[144];
  real_T tmp[441];
  real_T rtb_Add_hg[252];
  real_T rtb_f_0[12];
  real_T tmp_0[12];

  // Delay: '<Root>/Resettable Delay1' incorporates:
  //   Inport: '<Root>/NReset'
  //   Inport: '<Root>/x_0'

  if (rt_ZCFcn(RISING_ZERO_CROSSING,
               &quadrotor_EKF_PrevZCX.ResettableDelay1_Reset_ZCE,
               (quadrotor_EKF_U.NReset)) != NO_ZCEVENT) {
    quadrotor_EKF_DW.icLoad = 1U;
  }

  if (quadrotor_EKF_DW.icLoad != 0) {
    memcpy(&quadrotor_EKF_DW.x[0], &quadrotor_EKF_U.x_0[0], 21U * sizeof(real_T));
  }

  // MATLAB Function: '<S5>/nonlinear model' incorporates:
  //   Delay: '<Root>/Resettable Delay1'
  //   Inport: '<Root>/params'
  //   Inport: '<Root>/u_k'

  // MATLAB Function 'predicted state estimate/nonlinear model': '<S14>:1'
  // F
  //     F = F(X1,X2,X3,PHI,THETA,PSI,DX1,DX2,DX3,DPHI,DTHETA,DPSI,DDX1,DDX2,DDX3,B_A1,B_A2,B_A3,B_G1,B_G2,B_G3,G,M,JXX,JYY,JZZ,TAU_X,TAU_Y,TAU_Z,THRUST) 
  //     This function was generated by the Symbolic Math Toolbox version 6.3.
  //     03-Oct-2016 13:50:37
  // '<S14>:1:8'
  t2 = 1.0 / quadrotor_EKF_U.params[1];

  // '<S14>:1:9'
  t3 = cos(quadrotor_EKF_DW.x[4]);

  // '<S14>:1:10'
  t4 = sin(quadrotor_EKF_DW.x[4]);

  // '<S14>:1:11'
  t5 = cos(quadrotor_EKF_DW.x[3]);

  // '<S14>:1:12'
  t6 = cos(quadrotor_EKF_DW.x[5]);

  // '<S14>:1:13'
  t7 = t6 * t6;

  // '<S14>:1:14'
  t8 = quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[2];

  // '<S14>:1:15'
  t9 = sin(quadrotor_EKF_DW.x[5]);

  // '<S14>:1:16'
  // '<S14>:1:17'
  t11 = sin(quadrotor_EKF_DW.x[4] * 2.0);

  // '<S14>:1:18'
  t12 = sin(quadrotor_EKF_DW.x[3]);

  // '<S14>:1:19'
  t13 = t3 * t3;

  // '<S14>:1:20'
  t14 = quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[3];

  // '<S14>:1:21'
  t15 = quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[9];

  // '<S14>:1:22'
  t16 = 1.0 / quadrotor_EKF_U.params[2];

  // '<S14>:1:23'
  t17 = 1.0 / quadrotor_EKF_U.params[3];

  // '<S14>:1:24'
  t18 = 1.0 / (t3 * t3);

  // '<S14>:1:25'
  t19 = quadrotor_EKF_U.params[4] * quadrotor_EKF_U.params[4];

  // '<S14>:1:26'
  t20 = t9 * t9;

  // '<S14>:1:27'
  t21 = t13 * t13;

  // '<S14>:1:28'
  t22 = quadrotor_EKF_DW.x[10] * quadrotor_EKF_DW.x[10];

  // '<S14>:1:29'
  rtb_f[0] = quadrotor_EKF_DW.x[6] * 0.005 + quadrotor_EKF_DW.x[0];
  rtb_f[1] = quadrotor_EKF_DW.x[7] * 0.005 + quadrotor_EKF_DW.x[1];
  rtb_f[2] = quadrotor_EKF_DW.x[8] * 0.005 + quadrotor_EKF_DW.x[2];
  rtb_f[3] = quadrotor_EKF_DW.x[9] * 0.005 + quadrotor_EKF_DW.x[3];
  rtb_f[4] = quadrotor_EKF_DW.x[10] * 0.005 + quadrotor_EKF_DW.x[4];
  rtb_f[5] = quadrotor_EKF_DW.x[11] * 0.005 + quadrotor_EKF_DW.x[5];
  rtb_f[6] = t2 * t4 * quadrotor_EKF_U.u_k[3] * 0.005 + quadrotor_EKF_DW.x[6];
  rtb_f[7] = quadrotor_EKF_DW.x[7] - t2 * t3 * t12 * quadrotor_EKF_U.u_k[3] *
    0.005;
  rtb_f[8] = quadrotor_EKF_DW.x[8] - (quadrotor_EKF_U.params[0] *
    quadrotor_EKF_U.params[1] - t3 * t5 * quadrotor_EKF_U.u_k[3]) * t2 * 0.005;
  rtb_f[9] = quadrotor_EKF_DW.x[9] -
    (((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((-quadrotor_EKF_U.params
    [2] * t4 * quadrotor_EKF_U.u_k[2] - quadrotor_EKF_U.params[2] * t3 * t6 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] * t5 * t6 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] * t3 * t9 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] * t4 * t7 *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[2] * t5 * t9 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t4 * t7 *
    quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t8 * t11 * 0.5) - quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t3 * t8)
    - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t11 * 0.5) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x
    [11] * quadrotor_EKF_DW.x[10] * t3) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t11 * 0.5) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t3) +
    quadrotor_EKF_U.params[2] * t3 * t4 * t5 * quadrotor_EKF_U.u_k[2]) +
    quadrotor_EKF_U.params[2] * t3 * t5 * t6 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[2] * t3 * t6 * t7 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[2] * t5 * t6 * t7 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * t3 * t7 * t9 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * t5 * t7 * t9 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * t4 * t6 * t12 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * t5 * t6 * t13 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[2] * t4 * t9 * t12 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * t5 * t9 * t13 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[3] * t3 * t5 * t6 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[3] * t3 * t6 * t7 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[3] * t5 * t6 * t7 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[3] * t3 * t7 * t9 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[3] * t5 * t7 * t9 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t3 * t7 * t8) -
    quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t3 * t7 * t14) - t4 * t6 *
    t8 * t9 * t13 * t15) + t4 * t6 * t9 * t13 * t14 * t15) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x
    [11] * quadrotor_EKF_DW.x[10] * t3 * t7) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] *
    t3 * t7) - quadrotor_EKF_U.params[2] * t3 * t4 * t5 * t7 *
    quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[2] * t3 * t5 * t6 * t7 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] * t3 * t5 * t7 * t9 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t3 * t4 * t6 * t12 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t4 * t6 * t7 * t12 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t5 * t6 * t7 * t13 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] * t4 * t7 * t9 * t12 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] * t5 * t7 * t9 * t13 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t6 * t9 * t12 * t13 *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[3] * t3 * t4 * t5 * t7 *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[3] * t3 * t5 * t6 * t7 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] * t3 * t5 * t7 * t9 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] * t3 * t4 * t6 * t12 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] * t4 * t6 * t7 * t12 *
                       quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] * t5 *
                      t6 * t7 * t13 * quadrotor_EKF_U.u_k[0]) +
                     quadrotor_EKF_U.params[3] * t4 * t7 * t9 * t12 *
                     quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] * t5 *
                    t7 * t9 * t13 * quadrotor_EKF_U.u_k[1]) +
                   quadrotor_EKF_U.params[3] * t6 * t9 * t12 * t13 *
                   quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_DW.x[9] *
                  quadrotor_EKF_DW.x[11] * t6 * t8 * t9 * t13) +
                 quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t6 * t9 * t13 *
                 t14) + quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t4
                * t7 * t8) - quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3
               * t4 * t7 * t14) + quadrotor_EKF_U.params[2] *
              quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] *
              quadrotor_EKF_DW.x[11] * t6 * t9 * t13) - quadrotor_EKF_U.params[3]
             * quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] *
             quadrotor_EKF_DW.x[11] * t6 * t9 * t13) - quadrotor_EKF_U.params[2]
            * quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] *
            quadrotor_EKF_DW.x[10] * t3 * t4 * t7) + quadrotor_EKF_U.params[3] *
           quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] *
           quadrotor_EKF_DW.x[10] * t3 * t4 * t7) + quadrotor_EKF_U.params[2] *
          quadrotor_EKF_U.params[4] * t4 * t6 * t9 * t13 * t15) -
         quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t4 * t6 * t9 *
         t13 * t15) + quadrotor_EKF_U.params[2] * t3 * t4 * t6 * t7 * t12 *
        quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] * t3 * t4 * t7 * t9 *
       t12 * quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] * t3 * t4 * t6 *
      t7 * t12 * quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t3 * t4 *
     t7 * t9 * t12 * quadrotor_EKF_U.u_k[0]) * (t16 * t17 * t18) * 0.005;
  rtb_f[10] =
    (((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((quadrotor_EKF_U.params
    [2] * t3 * t6 * quadrotor_EKF_U.u_k[1] + quadrotor_EKF_U.params[2] * t5 * t6
    * quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t3 * t6 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t5 * t6 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t12 * t13 *
    quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] *
    t13 * t14) - t4 * t13 * t14 * t15) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] *
    t13) + quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t13) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t4 * t13 * t15) -
    quadrotor_EKF_U.params[2] * t3 * t6 * t7 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * t3 * t7 * t9 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * t5 * t6 * t7 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * t4 * t6 * t9 * quadrotor_EKF_U.u_k[2]) -
    quadrotor_EKF_U.params[2] * t5 * t7 * t9 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[2] * t4 * t6 * t12 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * t5 * t6 * t13 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * t7 * t12 * t13 * quadrotor_EKF_U.u_k[2]) +
    quadrotor_EKF_U.params[3] * t3 * t5 * t6 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[3] * t3 * t5 * t9 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[3] * t3 * t6 * t7 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[3] * t3 * t7 * t9 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[3] * t5 * t6 * t7 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[3] * t4 * t6 * t9 * quadrotor_EKF_U.u_k[2]) +
    quadrotor_EKF_U.params[3] * t5 * t7 * t9 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[3] * t4 * t6 * t12 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[3] * t5 * t6 * t13 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[3] * t7 * t12 * t13 * quadrotor_EKF_U.u_k[2]) -
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t7 * t8 * t13) +
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t7 * t13 * t14) - t4 * t7 *
    t8 * t13 * t15) + t4 * t7 * t13 * t14 * t15) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] *
    t7 * t13) - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t7 * t13) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 * t7 * t13 * t15)
    - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t4 * t7 * t13 *
    t15) + quadrotor_EKF_U.params[2] * t3 * t5 * t6 * t7 * quadrotor_EKF_U.u_k[1])
    + quadrotor_EKF_U.params[2] * t3 * t5 * t7 * t9 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * t4 * t6 * t7 * t12 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[2] * t5 * t6 * t7 * t13 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * t4 * t7 * t9 * t12 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * t5 * t7 * t9 * t13 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[3] * t3 * t5 * t6 * t7 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[3] * t3 * t5 * t7 * t9 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[3] * t3 * t4 * t6 * t12 * quadrotor_EKF_U.u_k[0]) -
                       quadrotor_EKF_U.params[3] * t3 * t4 * t9 * t12 *
                       quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] * t4 *
                      t6 * t7 * t12 * quadrotor_EKF_U.u_k[0]) -
                     quadrotor_EKF_U.params[3] * t5 * t6 * t7 * t13 *
                     quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t4 *
                    t7 * t9 * t12 * quadrotor_EKF_U.u_k[1]) -
                   quadrotor_EKF_U.params[3] * t5 * t7 * t9 * t13 *
                   quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_DW.x[11] *
                  quadrotor_EKF_DW.x[10] * t3 * t6 * t8 * t9) +
                 quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t3 * t6 * t9 *
                 t14) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
                quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t3 * t6 * t9)
               - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
               quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t3 * t6 * t9) +
              quadrotor_EKF_U.params[2] * t3 * t4 * t5 * t6 * t9 *
              quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[2] * t3 * t4 * t6
             * t7 * t12 * quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] *
            t3 * t4 * t7 * t9 * t12 * quadrotor_EKF_U.u_k[1]) -
           quadrotor_EKF_U.params[3] * t3 * t4 * t5 * t6 * t9 *
           quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[3] * t3 * t4 * t6 *
          t7 * t12 * quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] * t3 *
         t4 * t7 * t9 * t12 * quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_DW.x[9] *
        quadrotor_EKF_DW.x[10] * t3 * t4 * t6 * t8 * t9) + quadrotor_EKF_DW.x[9]
       * quadrotor_EKF_DW.x[10] * t3 * t4 * t6 * t9 * t14) +
      quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
      quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t4 * t6 * t9) -
     quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x
     [9] * quadrotor_EKF_DW.x[10] * t3 * t4 * t6 * t9) * (t16 * t17) * 0.005 /
    t3 + quadrotor_EKF_DW.x[10];
  rtb_f[11] =
    (((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((
    (((((((((((((((((((((((((((((quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t7 * quadrotor_EKF_U.u_k[2] +
    -quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t13 * quadrotor_EKF_U.u_k[2]) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t7 *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t5 * quadrotor_EKF_U.u_k[2]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t6 * t12 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t7 * t13 * quadrotor_EKF_U.u_k[2]) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t9 * t12 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t7 * t13 * quadrotor_EKF_U.u_k[2]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t3 * t19) + quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[11] *
    quadrotor_EKF_DW.x[10] * t11 * t19 * 0.5) - quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t8) -
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] *
    t8 * t11 * 0.5) - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t3) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] *
    t11 * 0.5) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] * t3 * t5
    * t13 * quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * t6 * t12 * t13 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] * t9 * t12 * t13 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t6 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 * t5 * t6 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t9 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t5 * t7 *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t4 * t5 * t9 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t6 * t12 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t5 * t13 * quadrotor_EKF_U.u_k[2]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t6 * t7 * t12 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t6 * t12 * t13 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t9 * t12 * t13 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t9 * t12 * t20 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t5 * t7 *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t3 * t6 * t12 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t6 * t7 * t12 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t9 * t12 * t20 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t3 * t7 * t19) + quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] *
    quadrotor_EKF_DW.x[10] * t3 * t13 * t14) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t13 * t19) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t3 * t8 * t13) + quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] *
    quadrotor_EKF_DW.x[10] * t3 * t7 * t19) + quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t7 * t8) -
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t3 * t7 * t14) + quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] *
    quadrotor_EKF_DW.x[10] * t3 * t8 * t13) + quadrotor_EKF_U.params[2] * t6 *
    t9 * t13 * t15 * t19) - quadrotor_EKF_U.params[2] * t6 * t9 * t13 * t14 *
    t22) + quadrotor_EKF_U.params[2] * t6 * t9 * t14 * t15 * t21) -
    quadrotor_EKF_U.params[2] * t6 * t9 * t15 * t19 * t21) +
    quadrotor_EKF_U.params[3] * t6 * t8 * t9 * t13 * t22) -
    quadrotor_EKF_U.params[3] * t6 * t8 * t9 * t15 * t21) -
    quadrotor_EKF_U.params[3] * t6 * t9 * t13 * t15 * t19) +
    quadrotor_EKF_U.params[3] * t6 * t9 * t15 * t19 * t21) -
    quadrotor_EKF_U.params[4] * t6 * t8 * t9 * t13 * t15) +
    quadrotor_EKF_U.params[4] * t6 * t9 * t13 * t14 * t15) +
    quadrotor_EKF_U.params[4] * t6 * t8 * t9 * t15 * t21) -
    quadrotor_EKF_U.params[4] * t6 * t9 * t14 * t15 * t21) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] * t4 * t5 * t6 * t13 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * t4 * t5 * t9 * t13 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t4 * t5 * t6 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t6 * t7 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 * t5 * t6 * t7 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t7 * t9 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 * t5 * t7 * t9 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t4 * t5 * t6 * t13 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t6 * t7 * t12 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t5 * t7 * t13 * quadrotor_EKF_U.u_k[2]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t7 * t9 * t12 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t4 * t5 * t9 * t13 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t6 * t12 * t13 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t6 * t7 * t12 * t13 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t7 * t9 * t12 * t13 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t5 * t6 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t4 * t6 * t7 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t4 * t5 * t6 * t7 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t4 * t7 * t9 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t4 * t5 * t7 * t9 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t6 * t7 * t12 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t3 * t5 * t7 * t13 * quadrotor_EKF_U.u_k[2]) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t7 * t9 * t12 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t3 * t6 * t12 * t13 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t6 * t7 * t12 * t13 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t7 * t9 * t12 * t13 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t3 * t7 * t13 * t14 * 2.0) + quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x
    [9] * quadrotor_EKF_DW.x[10] * t3 * t7 * t13 * t19) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] *
    t3 * t4 * t7 * t19) + quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] *
    quadrotor_EKF_DW.x[10] * t3 * t7 * t8 * t13 * 2.0) - quadrotor_EKF_U.params
    [3] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t7 * t13 * t19)
    + quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10]
    * t3 * t4 * t7 * t19) - quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] *
    quadrotor_EKF_DW.x[10] * t3 * t7 * t8 * t13) + quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t7 * t13 * t14) +
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] *
    t3 * t4 * t7 * t8) - quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] *
                       quadrotor_EKF_DW.x[10] * t3 * t4 * t7 * t14) -
                      quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3
                      * t4 * t5 * t6 * t7 * quadrotor_EKF_U.u_k[0]) +
                     quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 *
                     t4 * t5 * t7 * t9 * quadrotor_EKF_U.u_k[1]) -
                    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 *
                    t5 * t6 * t7 * t13 * quadrotor_EKF_U.u_k[0]) +
                   quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 *
                   t5 * t7 * t9 * t13 * quadrotor_EKF_U.u_k[1]) -
                  quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 *
                  t6 * t7 * t12 * t13 * quadrotor_EKF_U.u_k[1]) -
                 quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t7
                 * t9 * t12 * t13 * quadrotor_EKF_U.u_k[0]) -
                quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 * t6 *
                t9 * t12 * t13 * quadrotor_EKF_U.u_k[2]) +
               quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t4 *
               t5 * t6 * t7 * quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3]
              * quadrotor_EKF_U.params[4] * t3 * t4 * t5 * t7 * t9 *
              quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] *
             quadrotor_EKF_U.params[4] * t4 * t5 * t6 * t7 * t13 *
             quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] *
            quadrotor_EKF_U.params[4] * t4 * t5 * t7 * t9 * t13 *
            quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] *
           quadrotor_EKF_U.params[4] * t3 * t6 * t7 * t12 * t13 *
           quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] *
          quadrotor_EKF_U.params[4] * t3 * t7 * t9 * t12 * t13 *
          quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] *
         quadrotor_EKF_U.params[4] * t4 * t6 * t9 * t12 * t13 *
         quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[2] *
        quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t4 * t6 * t9 * t13 *
        t19) - quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] *
       quadrotor_EKF_DW.x[11] * t4 * t6 * t9 * t13 * t19) -
      quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11]
      * t4 * t6 * t8 * t9 * t13) + quadrotor_EKF_U.params[4] *
     quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t4 * t6 * t9 * t13 * t14) *
    (t16 * t17 * t18) * 0.005 / quadrotor_EKF_U.params[4] + quadrotor_EKF_DW.x
    [11];
  memcpy(&rtb_f[12], &quadrotor_EKF_DW.x[12], 9U * sizeof(real_T));

  // MATLAB Function: '<S12>/F_k-1=dF//dx|x=x_k-1' incorporates:
  //   Delay: '<Root>/Resettable Delay1'
  //   Inport: '<Root>/params'
  //   Inport: '<Root>/u_k'

  // MATLAB Function 'predicted covariance estimate/linearized state transition matrix/F_k-1=dF//dx|x=x_k-1': '<S13>:1' 
  // F_K
  //     F_K = F_K(X1,X2,X3,PHI,THETA,PSI,DX1,DX2,DX3,DPHI,DTHETA,DPSI,DDX1,DDX2,DDX3,B_A1,B_A2,B_A3,B_G1,B_G2,B_G3,G,M,JXX,JYY,JZZ,TAU_X,TAU_Y,TAU_Z,THRUST) 
  //     This function was generated by the Symbolic Math Toolbox version 6.3.
  //     03-Oct-2016 13:50:46
  // '<S13>:1:8'
  t2 = 1.0 / quadrotor_EKF_U.params[1];

  // '<S13>:1:9'
  t3 = cos(quadrotor_EKF_DW.x[4]);

  // '<S13>:1:10'
  t4 = sin(quadrotor_EKF_DW.x[3]);

  // '<S13>:1:11'
  t5 = cos(quadrotor_EKF_DW.x[3]);

  // '<S13>:1:12'
  t6 = sin(quadrotor_EKF_DW.x[4]);

  // '<S13>:1:13'
  t7 = cos(quadrotor_EKF_DW.x[5]);

  // '<S13>:1:14'
  t8 = t7 * t7;

  // '<S13>:1:15'
  t9 = sin(quadrotor_EKF_DW.x[5]);

  // '<S13>:1:16'
  t10 = t3 * t3;

  // '<S13>:1:17'
  t11 = 1.0 / quadrotor_EKF_U.params[2];

  // '<S13>:1:18'
  t12 = 1.0 / quadrotor_EKF_U.params[3];

  // '<S13>:1:19'
  t13 = 1.0 / (t3 * t3);

  // '<S13>:1:20'
  t14 = quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[2];

  // '<S13>:1:21'
  t15 = t6 * t6;

  // '<S13>:1:22'
  t16 = quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[3];

  // '<S13>:1:23'
  t17 = quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[9];

  // '<S13>:1:24'
  t18 = quadrotor_EKF_DW.x[4] * 2.0;

  // '<S13>:1:25'
  t19 = cos(t18);

  // '<S13>:1:26'
  t20 = sin(t18);

  // '<S13>:1:27'
  t21 = t9 * t9;

  // '<S13>:1:28'
  t22 = quadrotor_EKF_U.params[2] * t4 * t7 * t10 * quadrotor_EKF_U.u_k[1];

  // '<S13>:1:29'
  t18 = quadrotor_EKF_U.params[2] * t3 * t4 * t7 * t8 * quadrotor_EKF_U.u_k[1];

  // '<S13>:1:30'
  t24 = quadrotor_EKF_U.params[3] * t5 * t6 * t7 * t8 * quadrotor_EKF_U.u_k[0];

  // '<S13>:1:31'
  t25 = quadrotor_EKF_U.params[2] * t5 * t8 * t10 * quadrotor_EKF_U.u_k[2];

  // '<S13>:1:32'
  t26 = quadrotor_EKF_U.params[3] * t4 * t7 * t8 * t10 * quadrotor_EKF_U.u_k[1];

  // '<S13>:1:33'
  t27 = quadrotor_EKF_U.params[2] * t5 * t6 * t7 * quadrotor_EKF_U.u_k[0];

  // '<S13>:1:34'
  t28 = quadrotor_EKF_U.params[3] * t4 * t8 * t9 * t10 * quadrotor_EKF_U.u_k[0];

  // '<S13>:1:35'
  t29 = quadrotor_EKF_U.params[2] * t3 * t4 * t8 * t9 * quadrotor_EKF_U.u_k[0];

  // '<S13>:1:36'
  t30 = quadrotor_EKF_U.params[2] * t5 * t6 * t8 * t9 * quadrotor_EKF_U.u_k[1];

  // '<S13>:1:37'
  t31 = 1.0 / t3;

  // '<S13>:1:38'
  t32 = quadrotor_EKF_U.params[2] * t3 * t4 * t7 * t8 * quadrotor_EKF_U.u_k[0];

  // '<S13>:1:39'
  t33 = quadrotor_EKF_U.params[3] * t5 * t6 * t7 * t8 * quadrotor_EKF_U.u_k[1];

  // '<S13>:1:40'
  t34 = quadrotor_EKF_U.params[2] * t4 * t7 * t8 * t10 * quadrotor_EKF_U.u_k[0];

  // '<S13>:1:41'
  t35 = quadrotor_EKF_U.params[3] * t3 * t4 * t7 * quadrotor_EKF_U.u_k[0];

  // '<S13>:1:42'
  t36 = quadrotor_EKF_U.params[3] * t4 * t8 * t9 * t10 * quadrotor_EKF_U.u_k[1];

  // '<S13>:1:43'
  t37 = quadrotor_EKF_U.params[3] * t5 * t7 * t9 * t10 * quadrotor_EKF_U.u_k[2];

  // '<S13>:1:44'
  t38 = quadrotor_EKF_U.params[3] * t5 * t6 * t8 * t9 * quadrotor_EKF_U.u_k[0];

  // '<S13>:1:45'
  t39 = quadrotor_EKF_U.params[3] * t3 * t4 * t8 * t9 * quadrotor_EKF_U.u_k[1];

  // '<S13>:1:46'
  t40 = quadrotor_EKF_U.params[3] * t5 * t7 * t8 * quadrotor_EKF_U.u_k[1];

  // '<S13>:1:47'
  t41 = quadrotor_EKF_U.params[3] * t3 * t7 * t8 * quadrotor_EKF_U.u_k[1];

  // '<S13>:1:48'
  t42 = quadrotor_EKF_U.params[2] * t5 * t7 * quadrotor_EKF_U.u_k[1];

  // '<S13>:1:49'
  t43 = quadrotor_EKF_U.params[2] * t3 * t7 * quadrotor_EKF_U.u_k[1];

  // '<S13>:1:50'
  t44 = quadrotor_EKF_U.params[3] * t4 * t6 * t7 * t8 * quadrotor_EKF_U.u_k[0];

  // '<S13>:1:51'
  t45 = t6 * t8 * t10 * t16 * t17;

  // '<S13>:1:52'
  t46 = quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t8 * t10 * t16;

  // '<S13>:1:53'
  t47 = quadrotor_EKF_U.params[2] * t5 * t7 * t8 * t10 * quadrotor_EKF_U.u_k[1];

  // '<S13>:1:54'
  t48 = quadrotor_EKF_U.params[3] * t4 * t8 * t10 * quadrotor_EKF_U.u_k[2];

  // '<S13>:1:55'
  t49 = quadrotor_EKF_U.params[3] * t3 * t5 * t9 * quadrotor_EKF_U.u_k[0];

  // '<S13>:1:56'
  t50 = quadrotor_EKF_U.params[2] * t4 * t6 * t7 * quadrotor_EKF_U.u_k[0];

  // '<S13>:1:57'
  t51 = quadrotor_EKF_U.params[2] * t3 * t5 * t7 * t8 * quadrotor_EKF_U.u_k[1];

  // '<S13>:1:58'
  t52 = quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t6 * t8 * t10 *
    t17;

  // '<S13>:1:59'
  t53 = quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t8 * t10;

  // '<S13>:1:60'
  t54 = quadrotor_EKF_U.params[2] * t3 * t4 * t6 * t7 * t8 *
    quadrotor_EKF_U.u_k[0];

  // '<S13>:1:61'
  t55 = quadrotor_EKF_U.params[2] * t5 * t7 * t8 * quadrotor_EKF_U.u_k[0];

  // '<S13>:1:62'
  t56 = quadrotor_EKF_U.params[2] * t3 * t7 * t8 * quadrotor_EKF_U.u_k[0];

  // '<S13>:1:63'
  t57 = quadrotor_EKF_U.params[2] * t6 * t8 * quadrotor_EKF_U.u_k[2];

  // '<S13>:1:64'
  t58 = quadrotor_EKF_U.params[2] * t5 * t9 * quadrotor_EKF_U.u_k[1];

  // '<S13>:1:65'
  t59 = quadrotor_EKF_U.params[2] * t3 * t9 * quadrotor_EKF_U.u_k[1];

  // '<S13>:1:66'
  t60 = quadrotor_EKF_U.params[3] * t4 * t6 * t7 * t8 * quadrotor_EKF_U.u_k[1];

  // '<S13>:1:67'
  t61 = quadrotor_EKF_U.params[3] * t5 * t7 * t8 * t10 * quadrotor_EKF_U.u_k[0];

  // '<S13>:1:68'
  t62 = quadrotor_EKF_U.params[2] * t4 * t6 * t9 * quadrotor_EKF_U.u_k[0];

  // '<S13>:1:69'
  t63 = quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t3 * t8 * t14;

  // '<S13>:1:70'
  t64 = quadrotor_EKF_U.params[3] * t3 * t5 * t7 * t8 * quadrotor_EKF_U.u_k[0];

  // '<S13>:1:71'
  t65 = quadrotor_EKF_U.params[3] * t3 * t4 * t6 * t7 * quadrotor_EKF_U.u_k[1];

  // '<S13>:1:72'
  t66 = quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t6 * t8 * t14;

  // '<S13>:1:73'
  t67 = quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t3 * t8;

  // '<S13>:1:74'
  t68 = quadrotor_EKF_U.params[3] * t3 * t5 * t6 * t8 * quadrotor_EKF_U.u_k[2];

  // '<S13>:1:75'
  t69 = quadrotor_EKF_U.params[2] * t3 * t4 * t6 * t7 * t8 *
    quadrotor_EKF_U.u_k[1];

  // '<S13>:1:76'
  t70 = quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t6 * t8;

  // '<S13>:1:77'
  t71 = 1.0 / quadrotor_EKF_U.params[4];

  // '<S13>:1:78'
  t72 = quadrotor_EKF_U.params[4] * quadrotor_EKF_U.params[4];

  // '<S13>:1:79'
  t73 = quadrotor_EKF_DW.x[10] * quadrotor_EKF_DW.x[10];

  // '<S13>:1:80'
  t74 = 1.0 / rt_powd_snf(t3, 3.0);

  // '<S13>:1:81'
  t75 = t10 * t10;

  // '<S13>:1:82'
  rtb_F_k[0] = 1.0;
  rtb_F_k[1] = 0.0;
  rtb_F_k[2] = 0.0;
  rtb_F_k[3] = 0.0;
  rtb_F_k[4] = 0.0;
  rtb_F_k[5] = 0.0;
  rtb_F_k[6] = 0.0;
  rtb_F_k[7] = 0.0;
  rtb_F_k[8] = 0.0;
  rtb_F_k[9] = 0.0;
  rtb_F_k[10] = 0.0;
  rtb_F_k[11] = 0.0;
  rtb_F_k[12] = 0.0;
  rtb_F_k[13] = 0.0;
  rtb_F_k[14] = 0.0;
  rtb_F_k[15] = 0.0;
  rtb_F_k[16] = 0.0;
  rtb_F_k[17] = 0.0;
  rtb_F_k[18] = 0.0;
  rtb_F_k[19] = 0.0;
  rtb_F_k[20] = 0.0;
  rtb_F_k[21] = 0.0;
  rtb_F_k[22] = 1.0;
  rtb_F_k[23] = 0.0;
  rtb_F_k[24] = 0.0;
  rtb_F_k[25] = 0.0;
  rtb_F_k[26] = 0.0;
  rtb_F_k[27] = 0.0;
  rtb_F_k[28] = 0.0;
  rtb_F_k[29] = 0.0;
  rtb_F_k[30] = 0.0;
  rtb_F_k[31] = 0.0;
  rtb_F_k[32] = 0.0;
  rtb_F_k[33] = 0.0;
  rtb_F_k[34] = 0.0;
  rtb_F_k[35] = 0.0;
  rtb_F_k[36] = 0.0;
  rtb_F_k[37] = 0.0;
  rtb_F_k[38] = 0.0;
  rtb_F_k[39] = 0.0;
  rtb_F_k[40] = 0.0;
  rtb_F_k[41] = 0.0;
  rtb_F_k[42] = 0.0;
  rtb_F_k[43] = 0.0;
  rtb_F_k[44] = 1.0;
  rtb_F_k[45] = 0.0;
  rtb_F_k[46] = 0.0;
  rtb_F_k[47] = 0.0;
  rtb_F_k[48] = 0.0;
  rtb_F_k[49] = 0.0;
  rtb_F_k[50] = 0.0;
  rtb_F_k[51] = 0.0;
  rtb_F_k[52] = 0.0;
  rtb_F_k[53] = 0.0;
  rtb_F_k[54] = 0.0;
  rtb_F_k[55] = 0.0;
  rtb_F_k[56] = 0.0;
  rtb_F_k[57] = 0.0;
  rtb_F_k[58] = 0.0;
  rtb_F_k[59] = 0.0;
  rtb_F_k[60] = 0.0;
  rtb_F_k[61] = 0.0;
  rtb_F_k[62] = 0.0;
  rtb_F_k[63] = 0.0;
  rtb_F_k[64] = 0.0;
  rtb_F_k[65] = 0.0;
  rtb_F_k[66] = 1.0;
  rtb_F_k[67] = 0.0;
  rtb_F_k[68] = 0.0;
  rtb_F_k[69] = 0.0;
  rtb_F_k[70] = t2 * t3 * t5 * quadrotor_EKF_U.u_k[3] * -0.005;
  rtb_F_k[71] = t2 * t3 * t4 * quadrotor_EKF_U.u_k[3] * -0.005;
  rtb_F_k[72] = ((((((((((((((((((((((((((((((((((t32 + t33) + t34) + t35) + t36)
    + t37) + t38) + t39) + quadrotor_EKF_U.params[2] * t4 * t7 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] * t4 * t9 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t3 * t4 * t7 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] * t3 * t4 * t6 *
    quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[2] * t4 * t7 * t8 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] * t5 * t6 * t7 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] * t5 * t6 * t9 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] * t4 * t7 * t10 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] * t4 * t8 * t9 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] * t4 * t9 * t10 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] * t4 * t7 * t8 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] * t4 * t8 * t9 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t3 * t5 * t6 * t7 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] * t3 * t4 * t6 * t8 *
    quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[2] * t3 * t4 * t8 * t9 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t5 * t6 * t7 * t8 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t5 * t6 * t8 * t9 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] * t4 * t8 * t9 * t10 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t5 * t7 * t9 * t10 *
    quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[3] * t3 * t4 * t7 * t8 *
                        quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] * t3
                       * t5 * t6 * t7 * quadrotor_EKF_U.u_k[1]) -
                      quadrotor_EKF_U.params[3] * t3 * t4 * t6 * t8 *
                      quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[3] * t4 *
                     t7 * t8 * t10 * quadrotor_EKF_U.u_k[0]) +
                    quadrotor_EKF_U.params[2] * t3 * t5 * t6 * t7 * t8 *
                    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] * t3 *
                   t5 * t6 * t8 * t9 * quadrotor_EKF_U.u_k[0]) -
                  quadrotor_EKF_U.params[3] * t3 * t5 * t6 * t7 * t8 *
                  quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t3 * t5 *
                 t6 * t8 * t9 * quadrotor_EKF_U.u_k[0]) * (t11 * t12 * t13) *
    -0.005;
  rtb_F_k[73] = ((((((((((((((((((((((((((((((((((-t22 + t18) - t24) + t25) -
    t26) - t27) - t28) + t29) - t30) + quadrotor_EKF_U.params[2] * t4 * t7 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t4 * t7 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] * t5 * t10 *
    quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[2] * t4 * t7 * t8 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t4 * t8 * t9 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] * t3 * t4 * t7 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] * t3 * t4 * t9 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] * t5 * t6 * t7 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] * t4 * t7 * t8 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] * t4 * t8 * t9 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] * t4 * t7 * t10 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t5 * t8 * t10 *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[2] * t5 * t6 * t7 * t8 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] * t4 * t7 * t8 * t10 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] * t4 * t8 * t9 * t10 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] * t3 * t5 * t6 * t7 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] * t3 * t4 * t7 * t8 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t3 * t4 * t8 * t9 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] * t3 * t5 * t6 * t9 *
                        quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] * t5
                       * t6 * t8 * t9 * quadrotor_EKF_U.u_k[1]) -
                      quadrotor_EKF_U.params[2] * t3 * t5 * t6 * t7 * t8 *
                      quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] * t3 *
                     t4 * t6 * t7 * t9 * quadrotor_EKF_U.u_k[2]) +
                    quadrotor_EKF_U.params[2] * t3 * t5 * t6 * t8 * t9 *
                    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] * t3 *
                   t5 * t6 * t7 * t8 * quadrotor_EKF_U.u_k[0]) -
                  quadrotor_EKF_U.params[3] * t3 * t4 * t6 * t7 * t9 *
                  quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[3] * t3 * t5 *
                 t6 * t8 * t9 * quadrotor_EKF_U.u_k[1]) * (t11 * t12 * t31) *
    -0.005;
  rtb_F_k[74] =
    ((((((((((((((((((((((((((((((((((((((((((((((((((((((-quadrotor_EKF_U.params
    [2] * quadrotor_EKF_U.params[4] * t3 * t4 * quadrotor_EKF_U.u_k[2] +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t5 * t7 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t5 * t9 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] * t3 * t4 * t10 *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * t5 * t7 * t10 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] * t5 * t9 * t10 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t5 * t7 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t4 * t8 *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t4 * t6 * t7 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t4 * t10 *
    quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t4 * t6 * t9 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t5 * t7 * t8 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t5 * t7 * t10 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t5 * t9 * t10 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t5 * t9 * t21 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t5 * t7 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t8 * quadrotor_EKF_U.u_k[2]) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t5 * t7 * t8 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t5 * t9 * t21 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] * t4 * t6 * t7 * t10 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * t4 * t6 * t9 * t10 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t4 * t6 * t7 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t5 * t7 * t8 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t5 * t8 * t9 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t4 * t6 * t7 * t8 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t5 * t7 * t10 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t8 * t10 * quadrotor_EKF_U.u_k[2]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 * t6 * t7 * t10 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t4 * t6 * t8 * t9 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 * t6 * t9 * t10 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t5 * t7 * t8 * t10 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t5 * t8 * t9 * t10 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t6 * t7 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t5 * t7 * t8 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t3 * t5 * t8 * t9 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t4 * t6 * t7 * t8 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] *
                       quadrotor_EKF_U.params[4] * t3 * t5 * t7 * t10 *
                       quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] *
                      quadrotor_EKF_U.params[4] * t3 * t4 * t8 * t10 *
                      quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[3] *
                     quadrotor_EKF_U.params[4] * t4 * t6 * t8 * t9 *
                     quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] *
                    quadrotor_EKF_U.params[4] * t5 * t7 * t8 * t10 *
                    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] *
                   quadrotor_EKF_U.params[4] * t5 * t8 * t9 * t10 *
                   quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] *
                  quadrotor_EKF_U.params[4] * t3 * t4 * t6 * t7 * t8 *
                  quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] *
                 quadrotor_EKF_U.params[4] * t3 * t4 * t6 * t8 * t9 *
                 quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] *
                quadrotor_EKF_U.params[4] * t3 * t5 * t7 * t8 * t10 *
                quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] *
               quadrotor_EKF_U.params[4] * t3 * t5 * t8 * t9 * t10 *
               quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] *
              quadrotor_EKF_U.params[4] * t4 * t6 * t7 * t8 * t10 *
              quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] *
             quadrotor_EKF_U.params[4] * t4 * t6 * t8 * t9 * t10 *
             quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] *
            quadrotor_EKF_U.params[4] * t5 * t6 * t7 * t9 * t10 *
            quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[3] *
           quadrotor_EKF_U.params[4] * t3 * t4 * t6 * t7 * t8 *
           quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] *
          quadrotor_EKF_U.params[4] * t3 * t4 * t6 * t8 * t9 *
          quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] *
         quadrotor_EKF_U.params[4] * t3 * t5 * t7 * t8 * t10 *
         quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] *
        quadrotor_EKF_U.params[4] * t3 * t5 * t8 * t9 * t10 *
        quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] *
       quadrotor_EKF_U.params[4] * t4 * t6 * t7 * t8 * t10 *
       quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] *
      quadrotor_EKF_U.params[4] * t4 * t6 * t8 * t9 * t10 * quadrotor_EKF_U.u_k
      [1]) + quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t5 * t6 *
     t7 * t9 * t10 * quadrotor_EKF_U.u_k[2]) * (t11 * t12 * t13 * t71) * 0.005;
  rtb_F_k[75] = 0.0;
  rtb_F_k[76] = 0.0;
  rtb_F_k[77] = 0.0;
  rtb_F_k[78] = 0.0;
  rtb_F_k[79] = 0.0;
  rtb_F_k[80] = 0.0;
  rtb_F_k[81] = 0.0;
  rtb_F_k[82] = 0.0;
  rtb_F_k[83] = 0.0;
  rtb_F_k[84] = 0.0;
  rtb_F_k[85] = 0.0;
  rtb_F_k[86] = 0.0;
  rtb_F_k[87] = 0.0;
  rtb_F_k[88] = 1.0;
  rtb_F_k[89] = 0.0;
  rtb_F_k[90] = t2 * t3 * quadrotor_EKF_U.u_k[3] * 0.005;
  rtb_F_k[91] = t2 * t4 * t6 * quadrotor_EKF_U.u_k[3] * 0.005;
  rtb_F_k[92] = t2 * t5 * t6 * quadrotor_EKF_U.u_k[3] * -0.005;
  rtb_F_k[93] =
    ((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((t22
    + t18) + t24) + t25) + t26) + t27) + t28) + t29) + t30) +
    quadrotor_EKF_U.params[2] * t3 * quadrotor_EKF_U.u_k[2]) -
    quadrotor_EKF_U.params[2] * t3 * t8 * quadrotor_EKF_U.u_k[2]) -
    quadrotor_EKF_U.params[2] * t6 * t7 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[2] * t6 * t9 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * t5 * t10 * quadrotor_EKF_U.u_k[2]) +
    quadrotor_EKF_U.params[2] * t5 * t15 * quadrotor_EKF_U.u_k[2]) +
    quadrotor_EKF_U.params[3] * t3 * t8 * quadrotor_EKF_U.u_k[2]) +
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t14 * t19) -
    quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t6 * t14) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9]
    * quadrotor_EKF_DW.x[10] * t19) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] *
    t6) - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t19) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x
    [11] * quadrotor_EKF_DW.x[10] * t6) - quadrotor_EKF_U.params[2] * t3 * t4 *
    t7 * quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t3 * t4 * t9 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] * t6 * t7 * t8 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] * t6 * t8 * t9 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t4 * t7 * t15 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t5 * t8 * t15 *
    quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[3] * t5 * t6 * t7 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] * t6 * t7 * t8 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] * t4 * t7 * t10 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] * t6 * t8 * t9 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t5 * t8 * t10 *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[3] * t4 * t7 * t15 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] * t5 * t8 * t15 *
    quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t8 * t10 * t14) + quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t8 * t10 *
    t16) + quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t8 * t14 * t15) -
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t8 * t15 * t16) +
    quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t6 * t8 * t14) -
    quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t6 * t8 * t16) + t3 * t7 *
    t9 * t10 * t14 * t17) - t3 * t7 * t9 * t10 * t16 * t17) - t3 * t7 * t9 * t14
    * t15 * t17 * 2.0) + t3 * t7 * t9 * t15 * t16 * t17 * 2.0) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9]
    * quadrotor_EKF_DW.x[10] * t8 * t10) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t8 * t15) - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t6 * t8) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9]
    * quadrotor_EKF_DW.x[10] * t8 * t10) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t8 * t15) + quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t6 * t8) +
    quadrotor_EKF_U.params[2] * t3 * t5 * t6 * t7 * quadrotor_EKF_U.u_k[0] * 2.0)
    - quadrotor_EKF_U.params[2] * t3 * t5 * t6 * t9 * quadrotor_EKF_U.u_k[1] *
    2.0) - quadrotor_EKF_U.params[2] * t5 * t6 * t7 * t8 * quadrotor_EKF_U.u_k[0])
    - quadrotor_EKF_U.params[2] * t4 * t7 * t8 * t10 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * t4 * t8 * t9 * t10 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[2] * t4 * t7 * t8 * t15 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * t4 * t8 * t9 * t15 * quadrotor_EKF_U.u_k[0]) -
                       quadrotor_EKF_U.params[3] * t3 * t4 * t7 * t8 *
                       quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t3 *
                      t4 * t8 * t9 * quadrotor_EKF_U.u_k[0]) -
                     quadrotor_EKF_U.params[3] * t5 * t6 * t8 * t9 *
                     quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t4 *
                    t7 * t8 * t15 * quadrotor_EKF_U.u_k[1]) -
                   quadrotor_EKF_U.params[3] * t4 * t8 * t9 * t15 *
                   quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] *
                  quadrotor_EKF_U.params[4] * t3 * t7 * t9 * t10 * t17) +
                 quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t7
                 * t9 * t15 * t17 * 2.0) + quadrotor_EKF_U.params[3] *
                quadrotor_EKF_U.params[4] * t3 * t7 * t9 * t10 * t17) -
               quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t7 *
               t9 * t15 * t17 * 2.0) - quadrotor_EKF_U.params[2] * t3 * t5 * t6 *
              t7 * t8 * quadrotor_EKF_U.u_k[0] * 2.0) - quadrotor_EKF_U.params[2]
             * t3 * t4 * t6 * t7 * t9 * quadrotor_EKF_U.u_k[2] * 2.0) +
            quadrotor_EKF_U.params[2] * t3 * t5 * t6 * t8 * t9 *
            quadrotor_EKF_U.u_k[1] * 2.0) + quadrotor_EKF_U.params[3] * t3 * t5 *
           t6 * t7 * t8 * quadrotor_EKF_U.u_k[0] * 2.0) +
          quadrotor_EKF_U.params[3] * t3 * t4 * t6 * t7 * t9 *
          quadrotor_EKF_U.u_k[2] * 2.0) - quadrotor_EKF_U.params[3] * t3 * t5 *
         t6 * t8 * t9 * quadrotor_EKF_U.u_k[1] * 2.0) - quadrotor_EKF_DW.x[9] *
        quadrotor_EKF_DW.x[11] * t3 * t6 * t7 * t9 * t14 * 2.0) +
       quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t3 * t6 * t7 * t9 * t16 *
       2.0) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
      quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t3 * t6 * t7 * t9 * 2.0)
     - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
     quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t3 * t6 * t7 * t9 * 2.0) *
    (t11 * t12 * t13) * 0.005 -
    (((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((t55 + t56)
    + t57) + t58) + t59) + t60) + t61) + t62) + t63) + t64) + t65) + t66) + t67)
    + t68) + t69) + t70) - quadrotor_EKF_U.params[2] * t6 * quadrotor_EKF_U.u_k
    [2]) - quadrotor_EKF_U.params[2] * t3 * t7 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * t5 * t7 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[3] * t6 * t8 * quadrotor_EKF_U.u_k[2]) -
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t14 * t20 * 0.5) -
    quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t3 * t14) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9]
    * quadrotor_EKF_DW.x[10] * t20 * 0.5) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] *
    t3) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t20 * 0.5) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x
    [11] * quadrotor_EKF_DW.x[10] * t3) + quadrotor_EKF_U.params[2] * t3 * t5 *
    t7 * quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] * t3 * t5 * t6 *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[2] * t4 * t6 * t7 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t3 * t8 * t9 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] * t5 * t7 * t10 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] * t5 * t8 * t9 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t5 * t9 * t10 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t3 * t5 * t7 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] * t3 * t7 * t8 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] * t5 * t7 * t8 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] * t3 * t8 * t9 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] * t5 * t8 * t9 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] *
    t3 * t8 * t16) - t6 * t7 * t9 * t10 * t14 * t17) + t6 * t7 * t9 * t10 * t16 *
    t17) - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t3 * t8) -
    quadrotor_EKF_U.params[2] * t3 * t4 * t6 * t7 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * t3 * t5 * t7 * t8 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * t3 * t5 * t6 * t8 * quadrotor_EKF_U.u_k[2]) +
    quadrotor_EKF_U.params[2] * t3 * t5 * t8 * t9 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * t4 * t6 * t7 * t8 * quadrotor_EKF_U.u_k[1]) -
                       quadrotor_EKF_U.params[2] * t4 * t6 * t8 * t9 *
                       quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] * t5 *
                      t7 * t8 * t10 * quadrotor_EKF_U.u_k[0]) -
                     quadrotor_EKF_U.params[2] * t4 * t7 * t9 * t10 *
                     quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[2] * t5 *
                    t8 * t9 * t10 * quadrotor_EKF_U.u_k[1]) -
                   quadrotor_EKF_U.params[3] * t3 * t5 * t8 * t9 *
                   quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] * t4 * t6
                  * t8 * t9 * quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3]
                 * t4 * t7 * t9 * t10 * quadrotor_EKF_U.u_k[2]) -
                quadrotor_EKF_U.params[3] * t5 * t8 * t9 * t10 *
                quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_DW.x[9] *
               quadrotor_EKF_DW.x[11] * t7 * t9 * t10 * t14) +
              quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t7 * t9 * t10 *
              t16) - quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t6 *
             t8 * t16) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
            quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t7 * t9 * t10) -
           quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
           quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t7 * t9 * t10) -
          quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
          quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t6 * t8) +
         quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t6 * t7 * t9 *
         t10 * t17) - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t6
        * t7 * t9 * t10 * t17) + quadrotor_EKF_U.params[2] * t3 * t4 * t6 * t8 *
       t9 * quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] * t3 * t4 * t6 *
      t7 * t8 * quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t3 * t4 *
     t6 * t8 * t9 * quadrotor_EKF_U.u_k[0]) * (t6 * t11 * t12 * t74) * 0.01;
  rtb_F_k[94] =
    (((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((t32
    - t33) - t34) + t35) - t36) + t37) - t38) + t39) + quadrotor_EKF_U.params[2]
    * t6 * t7 * quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t6 * t7 *
    quadrotor_EKF_U.u_k[1]) + t3 * t10 * t16 * t17) - t3 * t15 * t16 * t17 * 2.0)
    - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t10 * t17) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t15 * t17 * 2.0)
    - quadrotor_EKF_U.params[2] * t3 * t4 * t7 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[2] * t3 * t7 * t9 * quadrotor_EKF_U.u_k[2]) -
    quadrotor_EKF_U.params[2] * t6 * t7 * t8 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * t6 * t8 * t9 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[3] * t3 * t4 * t6 * quadrotor_EKF_U.u_k[2] * 2.0) +
    quadrotor_EKF_U.params[3] * t5 * t6 * t7 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[3] * t5 * t6 * t9 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[3] * t3 * t7 * t9 * quadrotor_EKF_U.u_k[2]) -
    quadrotor_EKF_U.params[3] * t4 * t7 * t10 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[3] * t6 * t7 * t8 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[3] * t6 * t8 * t9 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[3] * t4 * t9 * t10 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[3] * t4 * t7 * t15 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[3] * t4 * t9 * t15 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t3 * t6 * t16 * 2.0) + t3 *
    t8 * t10 * t14 * t17) - t3 * t8 * t10 * t16 * t17) - t3 * t8 * t14 * t15 *
    t17 * 2.0) + t3 * t8 * t15 * t16 * t17 * 2.0) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] *
    t3 * t6 * 2.0) + quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t3 * t6 * 2.0) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t8 * t10 * t17)
    + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t8 * t15 *
    t17 * 2.0) + quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t8
    * t10 * t17) - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 *
    t8 * t15 * t17 * 2.0) - quadrotor_EKF_U.params[2] * t3 * t5 * t6 * t7 *
    quadrotor_EKF_U.u_k[1] * 2.0) - quadrotor_EKF_U.params[2] * t3 * t4 * t6 *
    t8 * quadrotor_EKF_U.u_k[2] * 2.0) - quadrotor_EKF_U.params[2] * t3 * t4 *
    t8 * t9 * quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] * t5 * t6 * t7
    * t8 * quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] * t5 * t6 * t8 *
    t9 * quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] * t4 * t8 * t9 *
    t10 * quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t5 * t7 * t9 *
    t10 * quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[2] * t4 * t7 * t8 *
    t15 * quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] * t4 * t8 * t9 *
    t15 * quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] * t5 * t7 * t9 *
    t15 * quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[3] * t3 * t4 * t7 *
    t8 * quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] * t3 * t5 * t6 * t7
    * quadrotor_EKF_U.u_k[1] * 2.0) + quadrotor_EKF_U.params[3] * t3 * t4 * t6 *
    t8 * quadrotor_EKF_U.u_k[2] * 2.0) + quadrotor_EKF_U.params[3] * t4 * t7 *
    t8 * t10 * quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] * t4 * t7 *
    t8 * t15 * quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] * t4 * t8 *
    t9 * t15 * quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t5 * t7 *
    t9 * t15 * quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_DW.x[9] *
    quadrotor_EKF_DW.x[11] * t3 * t6 * t8 * t14 * 2.0) + quadrotor_EKF_DW.x[9] *
                       quadrotor_EKF_DW.x[11] * t3 * t6 * t8 * t16 * 2.0) +
                      quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t7 * t9 *
                      t10 * t14) - quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10]
                     * t7 * t9 * t10 * t16) - quadrotor_EKF_DW.x[9] *
                    quadrotor_EKF_DW.x[10] * t7 * t9 * t14 * t15) +
                   quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t7 * t9 *
                   t15 * t16) - quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] *
                  t6 * t7 * t9 * t14) + quadrotor_EKF_DW.x[11] *
                 quadrotor_EKF_DW.x[10] * t6 * t7 * t9 * t16) +
                quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
                quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t3 * t6 * t8 *
                2.0) - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
               quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t3 * t6 * t8 *
               2.0) - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
              quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t7 * t9 * t10) +
             quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
             quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t7 * t9 * t15) +
            quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
            quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t6 * t7 * t9) +
           quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
           quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t7 * t9 * t10) -
          quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
          quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t7 * t9 * t15) -
         quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
         quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t6 * t7 * t9) +
        quadrotor_EKF_U.params[2] * t3 * t5 * t6 * t7 * t8 *
        quadrotor_EKF_U.u_k[1] * 2.0) + quadrotor_EKF_U.params[2] * t3 * t5 * t6
       * t8 * t9 * quadrotor_EKF_U.u_k[0] * 2.0) - quadrotor_EKF_U.params[3] *
      t3 * t5 * t6 * t7 * t8 * quadrotor_EKF_U.u_k[1] * 2.0) -
     quadrotor_EKF_U.params[3] * t3 * t5 * t6 * t8 * t9 * quadrotor_EKF_U.u_k[0]
     * 2.0) * (t11 * t12 * t31) * -0.005 +
    (((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((t40 + t41) +
    t42) + t43) + t44) + t45) + t46) + t47) + t48) + t49) + t50) + t51) + t52) +
    t53) + t54) - quadrotor_EKF_U.params[3] * t3 * t7 * quadrotor_EKF_U.u_k[1])
    - quadrotor_EKF_U.params[3] * t5 * t7 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[3] * t4 * t10 * quadrotor_EKF_U.u_k[2]) -
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t10 * t16) - t6 * t10 * t16
    * t17) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t10) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9]
    * quadrotor_EKF_DW.x[11] * t10) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t6 * t10 * t17) - quadrotor_EKF_U.params[2] * t3
    * t7 * t8 * quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t3 * t8 *
    t9 * quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] * t5 * t7 * t8 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t5 * t8 * t9 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] * t5 * t7 * t10 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t4 * t8 * t10 *
    quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[2] * t6 * t7 * t9 *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[3] * t3 * t5 * t7 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t4 * t6 * t7 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] * t3 * t8 * t9 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] * t5 * t8 * t9 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] * t5 * t7 * t10 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] * t6 * t7 * t9 *
    quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] *
    t8 * t10 * t14) - t6 * t8 * t10 * t14 * t17) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] *
    t8 * t10) - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t6 * t8 *
    t10 * t17) + quadrotor_EKF_U.params[2] * t3 * t5 * t8 * t9 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] * t4 * t6 * t7 * t8 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] * t4 * t6 * t8 * t9 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] * t5 * t8 * t9 * t10 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] * t3 * t4 * t6 * t7 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] * t3 * t4 * t6 * t9 *
                       quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t3 *
                      t5 * t7 * t8 * quadrotor_EKF_U.u_k[1]) -
                     quadrotor_EKF_U.params[3] * t3 * t5 * t8 * t9 *
                     quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] * t4 *
                    t6 * t8 * t9 * quadrotor_EKF_U.u_k[1]) -
                   quadrotor_EKF_U.params[3] * t5 * t7 * t8 * t10 *
                   quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t5 * t8
                  * t9 * t10 * quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_DW.x[11] *
                 quadrotor_EKF_DW.x[10] * t3 * t7 * t9 * t14) +
                quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t3 * t7 * t9 *
                t16) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
               quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t3 * t7 * t9) -
              quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
              quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t3 * t7 * t9) -
             quadrotor_EKF_U.params[2] * t3 * t4 * t6 * t8 * t9 *
             quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] * t3 * t5 * t6 *
            t7 * t9 * quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[3] * t3 *
           t4 * t6 * t7 * t8 * quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params
          [3] * t3 * t4 * t6 * t8 * t9 * quadrotor_EKF_U.u_k[1]) -
         quadrotor_EKF_U.params[3] * t3 * t5 * t6 * t7 * t9 *
         quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10]
        * t3 * t6 * t7 * t9 * t14) + quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x
       [10] * t3 * t6 * t7 * t9 * t16) + quadrotor_EKF_U.params[2] *
      quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10]
      * t3 * t6 * t7 * t9) - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params
     [4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t6 * t7 * t9) *
    (t6 * t11 * t12 * t13) * 0.005;
  rtb_F_k[95] =
    (((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((
    (((((((((((((((((((((((((((((quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t8 * quadrotor_EKF_U.u_k[2] +
    -quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t10 * quadrotor_EKF_U.u_k[2]) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t8 *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t5 * quadrotor_EKF_U.u_k[2]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 * t7 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t8 * t10 * quadrotor_EKF_U.u_k[2]) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t4 * t9 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t8 * t10 * quadrotor_EKF_U.u_k[2]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t3 * t72) + quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[11] *
    quadrotor_EKF_DW.x[10] * t20 * t72 * 0.5) - quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t14) -
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] *
    t14 * t20 * 0.5) - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t3) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] *
    t20 * 0.5) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] * t3 * t5
    * t10 * quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * t4 * t7 * t10 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] * t4 * t9 * t10 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t7 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t6 * t7 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t5 * t8 * quadrotor_EKF_U.u_k[2]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t5 * t6 * t7 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t6 * t9 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t5 * t10 *
    quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t4 * t7 * t8 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t5 * t6 * t9 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t4 * t7 * t10 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 * t9 * t10 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t4 * t9 * t21 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t4 * t7 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t3 * t5 * t8 * quadrotor_EKF_U.u_k[2]) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t4 * t7 * t8 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t4 * t9 * t21 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t3 * t10 * t16) - quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] *
    quadrotor_EKF_DW.x[10] * t3 * t8 * t72) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t10 * t72) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t3 * t10 * t14) + quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] *
    quadrotor_EKF_DW.x[10] * t3 * t8 * t72) + quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t8 * t14) -
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t3 * t8 * t16) + quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] *
    quadrotor_EKF_DW.x[10] * t3 * t10 * t14) - quadrotor_EKF_U.params[2] * t7 *
    t9 * t10 * t16 * t73) + quadrotor_EKF_U.params[2] * t7 * t9 * t10 * t17 *
    t72) + quadrotor_EKF_U.params[2] * t7 * t9 * t16 * t17 * t75) -
    quadrotor_EKF_U.params[2] * t7 * t9 * t17 * t72 * t75) +
    quadrotor_EKF_U.params[3] * t7 * t9 * t10 * t14 * t73) -
    quadrotor_EKF_U.params[3] * t7 * t9 * t10 * t17 * t72) -
    quadrotor_EKF_U.params[3] * t7 * t9 * t14 * t17 * t75) +
    quadrotor_EKF_U.params[3] * t7 * t9 * t17 * t72 * t75) -
    quadrotor_EKF_U.params[4] * t7 * t9 * t10 * t14 * t17) +
    quadrotor_EKF_U.params[4] * t7 * t9 * t10 * t16 * t17) +
    quadrotor_EKF_U.params[4] * t7 * t9 * t14 * t17 * t75) -
    quadrotor_EKF_U.params[4] * t7 * t9 * t16 * t17 * t75) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] * t5 * t6 * t7 * t10 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * t5 * t6 * t9 * t10 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t5 * t6 * t7 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t7 * t8 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t4 * t8 * t9 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t6 * t7 * t8 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t4 * t7 * t10 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t5 * t6 * t7 * t8 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t6 * t8 * t9 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t5 * t8 * t10 * quadrotor_EKF_U.u_k[2]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t5 * t6 * t7 * t10 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t5 * t6 * t8 * t9 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 * t7 * t8 * t10 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t4 * t8 * t9 * t10 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t5 * t6 * t9 * t10 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t3 * t5 * t6 * t7 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t4 * t7 * t8 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t8 * t9 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t6 * t7 * t8 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t7 * t10 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t5 * t6 * t7 * t8 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t3 * t6 * t8 * t9 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t5 * t8 * t10 *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t5 * t6 * t8 * t9 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t4 * t7 * t8 * t10 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t4 * t8 * t9 * t10 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t3 * t8 * t10 * t16 * 2.0) + quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x
    [9] * quadrotor_EKF_DW.x[10] * t3 * t8 * t10 * t72) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] *
    t3 * t6 * t8 * t72) + quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] *
    quadrotor_EKF_DW.x[10] * t3 * t8 * t10 * t14 * 2.0) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t3 * t8 * t10 * t72) + quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[11] *
    quadrotor_EKF_DW.x[10] * t3 * t6 * t8 * t72) - quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t8 * t10 * t14) +
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t3 * t8 * t10 * t16) + quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] *
    quadrotor_EKF_DW.x[10] * t3 * t6 * t8 * t14) - quadrotor_EKF_U.params[4] *
                       quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t3 * t6
                       * t8 * t16) - quadrotor_EKF_U.params[2] *
                      quadrotor_EKF_U.params[4] * t3 * t5 * t6 * t7 * t8 *
                      quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] *
                     quadrotor_EKF_U.params[4] * t3 * t5 * t6 * t8 * t9 *
                     quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] *
                    quadrotor_EKF_U.params[4] * t3 * t4 * t7 * t8 * t10 *
                    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] *
                   quadrotor_EKF_U.params[4] * t3 * t4 * t8 * t9 * t10 *
                   quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] *
                  quadrotor_EKF_U.params[4] * t5 * t6 * t7 * t8 * t10 *
                  quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] *
                 quadrotor_EKF_U.params[4] * t4 * t6 * t7 * t9 * t10 *
                 quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[2] *
                quadrotor_EKF_U.params[4] * t5 * t6 * t8 * t9 * t10 *
                quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] *
               quadrotor_EKF_U.params[4] * t3 * t5 * t6 * t7 * t8 *
               quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] *
              quadrotor_EKF_U.params[4] * t3 * t5 * t6 * t8 * t9 *
              quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] *
             quadrotor_EKF_U.params[4] * t3 * t4 * t7 * t8 * t10 *
             quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] *
            quadrotor_EKF_U.params[4] * t3 * t4 * t8 * t9 * t10 *
            quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] *
           quadrotor_EKF_U.params[4] * t5 * t6 * t7 * t8 * t10 *
           quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] *
          quadrotor_EKF_U.params[4] * t4 * t6 * t7 * t9 * t10 *
          quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[3] *
         quadrotor_EKF_U.params[4] * t5 * t6 * t8 * t9 * t10 *
         quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] *
        quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t6 * t7 * t9 * t10 *
        t72) - quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] *
       quadrotor_EKF_DW.x[11] * t6 * t7 * t9 * t10 * t72) -
      quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11]
      * t6 * t7 * t9 * t10 * t14) + quadrotor_EKF_U.params[4] *
     quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t6 * t7 * t9 * t10 * t16) *
    (t6 * t11 * t12 * t71 * t74) * 0.01 +
    (((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((
    ((((((((((((((((((((((((((((((((((((((((((((((((((quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t6 * quadrotor_EKF_U.u_k[2] * 2.0 +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t5 * t6 *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t7 * t10 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t9 * t10 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t7 * t15 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t9 * t15 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] *
    quadrotor_EKF_DW.x[10] * t6 * t72) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t19 * t72) -
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t6 * t14) + quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] *
    quadrotor_EKF_DW.x[10] * t14 * t19) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9]
    * quadrotor_EKF_DW.x[10] * t6) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x
    [11] * quadrotor_EKF_DW.x[10] * t19) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * t5 * t6 * t10 * quadrotor_EKF_U.u_k[2] * 3.0) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t5 * t7 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t5 * t9 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 * t6 * t7 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t6 * t8 * quadrotor_EKF_U.u_k[2] * 2.0) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t5 * t6 * t8 *
    quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t5 * t7 * t10 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t5 * t6 * t10 *
    quadrotor_EKF_U.u_k[2] * 3.0) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t7 * t8 * t10 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t5 * t7 * t15 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t8 * t9 * t10 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t7 * t8 * t15 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t8 * t9 * t15 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t4 * t6 * t7 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t3 * t6 * t8 * quadrotor_EKF_U.u_k[2] * 2.0) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t5 * t6 * t8 *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t5 * t7 * t10 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t7 * t8 * t10 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t5 * t7 * t15 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t8 * t9 * t10 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t7 * t8 * t15 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t8 * t9 * t15 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] *
    quadrotor_EKF_DW.x[10] * t6 * t10 * t16 * 3.0) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t6 * t8 * t72) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t6 * t10 * t72 * 3.0) + quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[11] *
    quadrotor_EKF_DW.x[10] * t8 * t10 * t72) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t8 * t15 * t72) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t6 * t10 * t14 * 3.0) + quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] *
    quadrotor_EKF_DW.x[10] * t6 * t8 * t72) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t8 * t10 * t72) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] *
    t8 * t15 * t72) + quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] *
    quadrotor_EKF_DW.x[10] * t6 * t8 * t14) - quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t6 * t8 * t16) +
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t6 * t10 * t14 * 3.0) - quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] *
    quadrotor_EKF_DW.x[10] * t8 * t10 * t14) + quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t8 * t10 * t16) +
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] *
    t8 * t14 * t15) - quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] *
    quadrotor_EKF_DW.x[10] * t8 * t15 * t16) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * t3 * t4 * t6 * t7 * quadrotor_EKF_U.u_k[1] * 2.0)
    + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] * t3 * t4 * t6 * t9 *
    quadrotor_EKF_U.u_k[0] * 2.0) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * t3 * t5 * t7 * t10 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] * t3 * t5 * t9 * t10 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * t3 * t5 * t7 * t15 * quadrotor_EKF_U.u_k[0] *
    2.0) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] * t3 * t5 * t9 *
    t15 * quadrotor_EKF_U.u_k[1] * 2.0) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t6 * t7 * quadrotor_EKF_U.u_k[1] * 2.0)
    - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t4 * t6 * t9 *
    quadrotor_EKF_U.u_k[0] * 2.0) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t5 * t7 * t8 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t5 * t7 * t10 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t5 * t8 * t9 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 * t6 * t7 * t8 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t4 * t6 * t8 * t9 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t5 * t9 * t10 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t4 * t6 * t7 * t10 * quadrotor_EKF_U.u_k[1] *
    3.0) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t5 * t7 *
    t15 * quadrotor_EKF_U.u_k[0] * 2.0) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t5 * t7 * t8 * t10 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t5 * t6 * t8 * t10 *
    quadrotor_EKF_U.u_k[2] * 3.0) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t5 * t9 * t15 * quadrotor_EKF_U.u_k[1] *
    2.0) - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t5 * t8 * t9 *
    t10 * quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t5 * t7 * t8 * t15 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t5 * t8 * t9 * t15 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t3 * t5 * t7 * t8 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t5 * t8 * t9 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t4 * t6 * t7 * t8 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t4 * t6 * t8 * t9 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t4 * t6 * t7 * t10 * quadrotor_EKF_U.u_k[1] *
    3.0) - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t5 * t7 * t8 *
    t10 * quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t5 * t6 * t8 * t10 * quadrotor_EKF_U.u_k[2] *
    3.0) + quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t5 * t8 * t9 *
    t10 * quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t5 * t7 * t8 * t15 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t5 * t8 * t9 * t15 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] *
    quadrotor_EKF_DW.x[10] * t6 * t8 * t10 * t16 * 6.0) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t6 * t8 * t10 * t72 * 3.0) + quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x
    [9] * quadrotor_EKF_DW.x[10] * t6 * t8 * t10 * t14 * 6.0) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t6 * t8 * t10 * t72 * 3.0) - quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x
    [9] * quadrotor_EKF_DW.x[10] * t6 * t8 * t10 * t14 * 3.0) +
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] *
    t6 * t8 * t10 * t16 * 3.0) - quadrotor_EKF_U.params[2] * t3 * t6 * t7 * t9 *
    t16 * t73 * 2.0) + quadrotor_EKF_U.params[2] * t3 * t6 * t7 * t9 * t17 * t72
    * 2.0) + quadrotor_EKF_U.params[3] * t3 * t6 * t7 * t9 * t14 * t73 * 2.0) -
    quadrotor_EKF_U.params[3] * t3 * t6 * t7 * t9 * t17 * t72 * 2.0) -
    quadrotor_EKF_U.params[4] * t3 * t6 * t7 * t9 * t14 * t17 * 2.0) +
    quadrotor_EKF_U.params[4] * t3 * t6 * t7 * t9 * t16 * t17 * 2.0) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t4 * t6 * t7 *
    t8 * quadrotor_EKF_U.u_k[1] * 2.0) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t6 * t8 * t9 * quadrotor_EKF_U.u_k[0] *
    2.0) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t5 * t7 *
    t8 * t10 * quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t7 * t9 * t10 * quadrotor_EKF_U.u_k[2])
    - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t5 * t8 * t9 *
    t10 * quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t4 * t6 * t7 * t8 * t10 * quadrotor_EKF_U.u_k[1]
    * 3.0) - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 * t6 *
    t8 * t9 * t10 * quadrotor_EKF_U.u_k[0] * 3.0) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t5 * t7 * t8 * t15 * quadrotor_EKF_U.u_k[0]
    * 2.0) - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t4 *
    t7 * t9 * t15 * quadrotor_EKF_U.u_k[2] * 2.0) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t5 * t8 * t9 * t15 * quadrotor_EKF_U.u_k[1]
    * 2.0) - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t4 *
    t6 * t7 * t8 * quadrotor_EKF_U.u_k[1] * 2.0) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t6 * t8 * t9 * quadrotor_EKF_U.u_k[0] *
    2.0) - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t5 * t7 *
    t8 * t10 * quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t7 * t9 * t10 * quadrotor_EKF_U.u_k[2])
    + quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t5 * t8 * t9 *
    t10 * quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] *
                       quadrotor_EKF_U.params[4] * t4 * t6 * t7 * t8 * t10 *
                       quadrotor_EKF_U.u_k[1] * 3.0) + quadrotor_EKF_U.params[3]
                      * quadrotor_EKF_U.params[4] * t4 * t6 * t8 * t9 * t10 *
                      quadrotor_EKF_U.u_k[0] * 3.0) + quadrotor_EKF_U.params[3] *
                     quadrotor_EKF_U.params[4] * t3 * t5 * t7 * t8 * t15 *
                     quadrotor_EKF_U.u_k[0] * 2.0) + quadrotor_EKF_U.params[3] *
                    quadrotor_EKF_U.params[4] * t3 * t4 * t7 * t9 * t15 *
                    quadrotor_EKF_U.u_k[2] * 2.0) - quadrotor_EKF_U.params[3] *
                   quadrotor_EKF_U.params[4] * t3 * t5 * t8 * t9 * t15 *
                   quadrotor_EKF_U.u_k[1] * 2.0) - quadrotor_EKF_U.params[2] *
                  quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t3 * t7 * t9 *
                  t10 * t72) + quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9]
                 * quadrotor_EKF_DW.x[11] * t3 * t7 * t9 * t15 * t72 * 2.0) +
                quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] *
                quadrotor_EKF_DW.x[11] * t3 * t7 * t9 * t10 * t72) -
               quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] *
               quadrotor_EKF_DW.x[11] * t3 * t7 * t9 * t15 * t72 * 2.0) +
              quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] *
              quadrotor_EKF_DW.x[11] * t3 * t7 * t9 * t10 * t14) -
             quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] *
             quadrotor_EKF_DW.x[11] * t3 * t7 * t9 * t10 * t16) -
            quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] *
            quadrotor_EKF_DW.x[11] * t3 * t7 * t9 * t14 * t15 * 2.0) +
           quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] *
           quadrotor_EKF_DW.x[11] * t3 * t7 * t9 * t15 * t16 * 2.0) +
          quadrotor_EKF_U.params[2] * t3 * t6 * t7 * t9 * t10 * t16 * t17 * 4.0)
         - quadrotor_EKF_U.params[2] * t3 * t6 * t7 * t9 * t10 * t17 * t72 * 4.0)
        - quadrotor_EKF_U.params[3] * t3 * t6 * t7 * t9 * t10 * t14 * t17 * 4.0)
       + quadrotor_EKF_U.params[3] * t3 * t6 * t7 * t9 * t10 * t17 * t72 * 4.0)
      + quadrotor_EKF_U.params[4] * t3 * t6 * t7 * t9 * t10 * t14 * t17 * 4.0) -
     quadrotor_EKF_U.params[4] * t3 * t6 * t7 * t9 * t10 * t16 * t17 * 4.0) *
    (t11 * t12 * t13 * t71) * -0.005;
  rtb_F_k[96] = 0.0;
  rtb_F_k[97] = 0.0;
  rtb_F_k[98] = 0.0;
  rtb_F_k[99] = 0.0;
  rtb_F_k[100] = 0.0;
  rtb_F_k[101] = 0.0;
  rtb_F_k[102] = 0.0;
  rtb_F_k[103] = 0.0;
  rtb_F_k[104] = 0.0;
  rtb_F_k[105] = 0.0;
  rtb_F_k[106] = 0.0;
  rtb_F_k[107] = 0.0;
  rtb_F_k[108] = 0.0;
  rtb_F_k[109] = 0.0;
  rtb_F_k[110] = 1.0;
  rtb_F_k[111] = 0.0;
  rtb_F_k[112] = 0.0;
  rtb_F_k[113] = 0.0;
  rtb_F_k[114] =
    (((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((
    ((t40 + t41) + t42) + t43) + t44) + t45) + t46) + t47) + t48) + t49) + t50)
    + t51) + t52) + t53) + t54) + quadrotor_EKF_U.params[2] * t3 * t9 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] * t5 * t9 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] * t3 * t5 * t9 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] * t3 * t7 * t8 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t3 * t8 * t9 *
    quadrotor_EKF_U.u_k[0] * 3.0) - quadrotor_EKF_U.params[2] * t4 * t6 * t9 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t5 * t7 * t8 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t5 * t8 * t9 *
    quadrotor_EKF_U.u_k[0] * 3.0) - quadrotor_EKF_U.params[2] * t5 * t7 * t10 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t4 * t8 * t10 *
    quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[2] * t5 * t9 * t10 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] * t6 * t7 * t9 *
    quadrotor_EKF_U.u_k[2] * 2.0) + quadrotor_EKF_U.params[2] * t3 * t7 * t21 *
    quadrotor_EKF_U.u_k[1] * 2.0) + quadrotor_EKF_U.params[2] * t5 * t7 * t21 *
    quadrotor_EKF_U.u_k[1] * 2.0) + quadrotor_EKF_U.params[2] * t4 * t10 * t21 *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[3] * t3 * t8 * t9 *
    quadrotor_EKF_U.u_k[0] * 3.0) + quadrotor_EKF_U.params[3] * t5 * t8 * t9 *
    quadrotor_EKF_U.u_k[0] * 3.0) + quadrotor_EKF_U.params[3] * t6 * t7 * t9 *
    quadrotor_EKF_U.u_k[2] * 2.0) - quadrotor_EKF_U.params[3] * t3 * t7 * t21 *
    quadrotor_EKF_U.u_k[1] * 2.0) - quadrotor_EKF_U.params[3] * t5 * t7 * t21 *
    quadrotor_EKF_U.u_k[1] * 2.0) - quadrotor_EKF_U.params[3] * t4 * t10 * t21 *
    quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] *
    t8 * t10 * t14) + quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t10 * t14
    * t21) - quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t10 * t16 * t21) -
    t6 * t8 * t10 * t14 * t17) + t6 * t10 * t14 * t17 * t21) - t6 * t10 * t16 *
    t17 * t21) - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t10 * t21) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9]
    * quadrotor_EKF_DW.x[11] * t8 * t10) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] *
    t10 * t21) - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t6 *
    t10 * t17 * t21) - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
    t6 * t8 * t10 * t17) + quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4]
    * t6 * t10 * t17 * t21) + quadrotor_EKF_U.params[2] * t3 * t4 * t6 * t9 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] * t3 * t5 * t8 * t9 *
    quadrotor_EKF_U.u_k[0] * 3.0) - quadrotor_EKF_U.params[2] * t4 * t6 * t7 *
    t8 * quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] * t4 * t6 * t8 * t9
    * quadrotor_EKF_U.u_k[1] * 3.0) + quadrotor_EKF_U.params[2] * t5 * t8 * t9 *
    t10 * quadrotor_EKF_U.u_k[0] * 3.0) - quadrotor_EKF_U.params[2] * t3 * t5 *
    t7 * t21 * quadrotor_EKF_U.u_k[1] * 2.0) + quadrotor_EKF_U.params[2] * t4 *
    t6 * t7 * t21 * quadrotor_EKF_U.u_k[0] * 2.0) - quadrotor_EKF_U.params[2] *
    t5 * t7 * t10 * t21 * quadrotor_EKF_U.u_k[1] * 2.0) -
    quadrotor_EKF_U.params[3] * t3 * t4 * t6 * t9 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[3] * t3 * t5 * t7 * t8 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[3] * t3 * t5 * t8 * t9 * quadrotor_EKF_U.u_k[0] * 3.0)
    - quadrotor_EKF_U.params[3] * t4 * t6 * t8 * t9 * quadrotor_EKF_U.u_k[1] *
    3.0) - quadrotor_EKF_U.params[3] * t5 * t7 * t8 * t10 * quadrotor_EKF_U.u_k
    [1]) - quadrotor_EKF_U.params[3] * t5 * t8 * t9 * t10 * quadrotor_EKF_U.u_k
                       [0] * 3.0) + quadrotor_EKF_U.params[3] * t3 * t5 * t7 *
                      t21 * quadrotor_EKF_U.u_k[1] * 2.0) -
                     quadrotor_EKF_U.params[3] * t4 * t6 * t7 * t21 *
                     quadrotor_EKF_U.u_k[0] * 2.0) + quadrotor_EKF_U.params[3] *
                    t5 * t7 * t10 * t21 * quadrotor_EKF_U.u_k[1] * 2.0) -
                   quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t3 * t7 *
                   t9 * t14 * 2.0) + quadrotor_EKF_DW.x[11] *
                  quadrotor_EKF_DW.x[10] * t3 * t7 * t9 * t16 * 2.0) +
                 quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
                 quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t3 * t7 * t9 *
                 2.0) - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
                quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t3 * t7 * t9 *
                2.0) - quadrotor_EKF_U.params[2] * t3 * t4 * t6 * t8 * t9 *
               quadrotor_EKF_U.u_k[1] * 3.0) + quadrotor_EKF_U.params[2] * t3 *
              t5 * t6 * t7 * t9 * quadrotor_EKF_U.u_k[2] * 2.0) -
             quadrotor_EKF_U.params[2] * t3 * t4 * t6 * t7 * t21 *
             quadrotor_EKF_U.u_k[0] * 2.0) - quadrotor_EKF_U.params[3] * t3 * t4
            * t6 * t7 * t8 * quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3]
           * t3 * t4 * t6 * t8 * t9 * quadrotor_EKF_U.u_k[1] * 3.0) -
          quadrotor_EKF_U.params[3] * t3 * t5 * t6 * t7 * t9 *
          quadrotor_EKF_U.u_k[2] * 2.0) + quadrotor_EKF_U.params[3] * t3 * t4 *
         t6 * t7 * t21 * quadrotor_EKF_U.u_k[0] * 2.0) - quadrotor_EKF_DW.x[9] *
        quadrotor_EKF_DW.x[10] * t3 * t6 * t7 * t9 * t14 * 2.0) +
       quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t6 * t7 * t9 * t16 *
       2.0) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
      quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t6 * t7 * t9 * 2.0)
     - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
     quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t6 * t7 * t9 * 2.0) *
    (t11 * t12 * t13) * -0.005;
  rtb_F_k[115] =
    (((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((
    ((((t55 + t56) + t57) + t58) + t59) + t60) + t61) + t62) + t63) + t64) + t65)
    + t66) + t67) + t68) + t69) + t70) - quadrotor_EKF_U.params[2] * t6 * t21 *
    quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[3] * t3 * t9 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t5 * t9 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t6 * t8 *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[3] * t6 * t21 *
    quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[2] * t3 * t8 * t9 *
    quadrotor_EKF_U.u_k[1] * 3.0) - quadrotor_EKF_U.params[2] * t5 * t8 * t9 *
    quadrotor_EKF_U.u_k[1] * 3.0) - quadrotor_EKF_U.params[2] * t5 * t9 * t10 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t3 * t7 * t21 *
    quadrotor_EKF_U.u_k[0] * 2.0) - quadrotor_EKF_U.params[2] * t5 * t7 * t21 *
    quadrotor_EKF_U.u_k[0] * 2.0) - quadrotor_EKF_U.params[3] * t3 * t5 * t7 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] * t3 * t5 * t9 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] * t3 * t7 * t8 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] * t4 * t6 * t9 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] * t5 * t7 * t8 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] * t3 * t8 * t9 *
    quadrotor_EKF_U.u_k[1] * 3.0) + quadrotor_EKF_U.params[3] * t5 * t8 * t9 *
    quadrotor_EKF_U.u_k[1] * 3.0) + quadrotor_EKF_U.params[3] * t5 * t9 * t10 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] * t3 * t7 * t21 *
    quadrotor_EKF_U.u_k[0] * 2.0) + quadrotor_EKF_U.params[3] * t5 * t7 * t21 *
    quadrotor_EKF_U.u_k[0] * 2.0) - quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x
    [10] * t3 * t8 * t16) - quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t3
    * t14 * t21) + quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t3 * t16 *
    t21) - t6 * t7 * t9 * t10 * t14 * t17 * 2.0) + t6 * t7 * t9 * t10 * t16 *
    t17 * 2.0) - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] * t3 * t8) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x
    [11] * quadrotor_EKF_DW.x[10] * t3 * t21) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10] *
    t3 * t21) - quadrotor_EKF_U.params[2] * t3 * t5 * t7 * t8 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] * t3 * t5 * t6 * t8 *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[2] * t3 * t5 * t8 * t9 *
    quadrotor_EKF_U.u_k[1] * 3.0) - quadrotor_EKF_U.params[2] * t4 * t6 * t7 *
    t8 * quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] * t4 * t6 * t8 * t9
    * quadrotor_EKF_U.u_k[0] * 3.0) - quadrotor_EKF_U.params[2] * t5 * t7 * t8 *
    t10 * quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] * t4 * t7 * t9 *
    t10 * quadrotor_EKF_U.u_k[2] * 2.0) + quadrotor_EKF_U.params[2] * t5 * t8 *
    t9 * t10 * quadrotor_EKF_U.u_k[1] * 3.0) + quadrotor_EKF_U.params[2] * t3 *
    t5 * t7 * t21 * quadrotor_EKF_U.u_k[0] * 2.0) + quadrotor_EKF_U.params[2] *
    t3 * t5 * t6 * t21 * quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[2] *
    t4 * t6 * t7 * t21 * quadrotor_EKF_U.u_k[1] * 2.0) + quadrotor_EKF_U.params
    [2] * t5 * t7 * t10 * t21 * quadrotor_EKF_U.u_k[0] * 2.0) +
    quadrotor_EKF_U.params[3] * t3 * t4 * t6 * t9 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[3] * t3 * t5 * t8 * t9 * quadrotor_EKF_U.u_k[1] * 3.0)
    + quadrotor_EKF_U.params[3] * t4 * t6 * t8 * t9 * quadrotor_EKF_U.u_k[0] *
    3.0) + quadrotor_EKF_U.params[3] * t4 * t7 * t9 * t10 * quadrotor_EKF_U.u_k
    [2] * 2.0) - quadrotor_EKF_U.params[3] * t5 * t8 * t9 * t10 *
    quadrotor_EKF_U.u_k[1] * 3.0) - quadrotor_EKF_U.params[3] * t3 * t5 * t7 *
    t21 * quadrotor_EKF_U.u_k[0] * 2.0) - quadrotor_EKF_U.params[3] * t3 * t5 *
    t6 * t21 * quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[3] * t4 * t6 *
                       t7 * t21 * quadrotor_EKF_U.u_k[1] * 2.0) -
                      quadrotor_EKF_U.params[3] * t5 * t7 * t10 * t21 *
                      quadrotor_EKF_U.u_k[0] * 2.0) - quadrotor_EKF_DW.x[9] *
                     quadrotor_EKF_DW.x[11] * t7 * t9 * t10 * t14 * 2.0) +
                    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t7 * t9 *
                    t10 * t16 * 2.0) - quadrotor_EKF_DW.x[9] *
                   quadrotor_EKF_DW.x[10] * t3 * t6 * t8 * t16) -
                  quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t6 * t14
                  * t21) + quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 *
                 t6 * t16 * t21) + quadrotor_EKF_U.params[2] *
                quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] *
                quadrotor_EKF_DW.x[11] * t7 * t9 * t10 * 2.0) -
               quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
               quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t7 * t9 * t10 *
               2.0) - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
              quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t6 * t8) +
             quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
             quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t6 * t21) -
            quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
            quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t6 * t21) +
           quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t6 * t7 * t9 *
           t10 * t17 * 2.0) - quadrotor_EKF_U.params[3] *
          quadrotor_EKF_U.params[4] * t6 * t7 * t9 * t10 * t17 * 2.0) +
         quadrotor_EKF_U.params[2] * t3 * t4 * t6 * t8 * t9 *
         quadrotor_EKF_U.u_k[0] * 3.0) - quadrotor_EKF_U.params[2] * t3 * t4 *
        t6 * t7 * t21 * quadrotor_EKF_U.u_k[1] * 2.0) - quadrotor_EKF_U.params[3]
       * t3 * t4 * t6 * t7 * t8 * quadrotor_EKF_U.u_k[1]) -
      quadrotor_EKF_U.params[3] * t3 * t4 * t6 * t8 * t9 * quadrotor_EKF_U.u_k[0]
      * 3.0) + quadrotor_EKF_U.params[3] * t3 * t4 * t6 * t7 * t21 *
     quadrotor_EKF_U.u_k[1] * 2.0) * (t11 * t12 * t31) * -0.005;
  rtb_F_k[116] = t11 * t12 * t13 * t71 *
    (((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((
    ((((((((((((((((((((((((((((((((((((((((((((((-quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t4 * t9 * quadrotor_EKF_U.u_k[1] -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t7 * t9 *
    quadrotor_EKF_U.u_k[2] * 2.0) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t4 * t7 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t7 * t9 *
    quadrotor_EKF_U.u_k[2] * 2.0) - quadrotor_EKF_U.params[2] * t8 * t10 * t16 *
    t73) + quadrotor_EKF_U.params[2] * t8 * t10 * t17 * t72) +
    quadrotor_EKF_U.params[2] * t8 * t16 * t17 * t75) + quadrotor_EKF_U.params[2]
    * t10 * t16 * t21 * t73) - quadrotor_EKF_U.params[2] * t10 * t17 * t21 * t72)
    - quadrotor_EKF_U.params[2] * t16 * t17 * t21 * t75) -
    quadrotor_EKF_U.params[2] * t8 * t17 * t72 * t75) + quadrotor_EKF_U.params[2]
    * t17 * t21 * t72 * t75) + quadrotor_EKF_U.params[3] * t8 * t10 * t14 * t73)
    - quadrotor_EKF_U.params[3] * t8 * t10 * t17 * t72) -
    quadrotor_EKF_U.params[3] * t8 * t14 * t17 * t75) - quadrotor_EKF_U.params[3]
    * t10 * t14 * t21 * t73) + quadrotor_EKF_U.params[3] * t10 * t17 * t21 * t72)
    + quadrotor_EKF_U.params[3] * t14 * t17 * t21 * t75) +
    quadrotor_EKF_U.params[3] * t8 * t17 * t72 * t75) - quadrotor_EKF_U.params[3]
    * t17 * t21 * t72 * t75) - quadrotor_EKF_U.params[4] * t8 * t10 * t14 * t17)
    + quadrotor_EKF_U.params[4] * t8 * t10 * t16 * t17) +
    quadrotor_EKF_U.params[4] * t10 * t14 * t17 * t21) - quadrotor_EKF_U.params
    [4] * t10 * t16 * t17 * t21) + quadrotor_EKF_U.params[4] * t8 * t14 * t17 *
    t75) - quadrotor_EKF_U.params[4] * t8 * t16 * t17 * t75) -
    quadrotor_EKF_U.params[4] * t14 * t17 * t21 * t75) + quadrotor_EKF_U.params
    [4] * t16 * t17 * t21 * t75) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * t4 * t7 * t10 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] * t4 * t9 * t10 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t9 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t6 * t7 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t6 * t9 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t5 * t6 * t7 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t5 * t6 * t9 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 * t7 * t10 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t4 * t8 * t9 * quadrotor_EKF_U.u_k[1] * 3.0) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 * t9 * t10 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t7 * t9 * t10 * quadrotor_EKF_U.u_k[2] * 2.0) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 * t7 * t21 *
    quadrotor_EKF_U.u_k[0] * 3.0) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t9 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t4 * t8 * t9 *
    quadrotor_EKF_U.u_k[1] * 3.0) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t7 * t9 * t10 * quadrotor_EKF_U.u_k[2] * 2.0) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t4 * t7 * t21 *
    quadrotor_EKF_U.u_k[0] * 3.0) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * t5 * t6 * t7 * t10 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] * t5 * t6 * t9 * t10 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t7 * t8 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t5 * t6 * t9 *
    quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t8 * t9 * quadrotor_EKF_U.u_k[1] * 3.0)
    - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t6 * t7 * t8 *
    quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t5 * t7 * t9 * quadrotor_EKF_U.u_k[2] * 2.0)
    - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t6 * t8 * t9 *
    quadrotor_EKF_U.u_k[0] * 3.0) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t9 * t10 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t5 * t6 * t7 * t8 *
    quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t5 * t6 * t8 * t9 * quadrotor_EKF_U.u_k[0] * 3.0)
    + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 * t7 * t8 * t10
    * quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t5 * t6 * t7 * t10 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 * t6 * t8 * t10 *
    quadrotor_EKF_U.u_k[2]) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t5 * t6 * t9 * t10 * quadrotor_EKF_U.u_k[0]) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 * t8 * t9 * t10 *
    quadrotor_EKF_U.u_k[1] * 3.0) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t7 * t21 * quadrotor_EKF_U.u_k[0] *
    2.0) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t6 * t7 *
    t21 * quadrotor_EKF_U.u_k[1] * 2.0) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t5 * t6 * t7 * t21 * quadrotor_EKF_U.u_k[1] *
    2.0) - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t4 * t7 * t10
    * t21 * quadrotor_EKF_U.u_k[0] * 2.0) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t4 * t6 * t10 * t21 * quadrotor_EKF_U.u_k[2]) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t4 * t7 * t8 *
    quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t3 * t5 * t6 * t9 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t4 * t8 * t9 *
    quadrotor_EKF_U.u_k[1] * 3.0) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t3 * t6 * t7 * t8 * quadrotor_EKF_U.u_k[1]) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t5 * t7 * t9 *
    quadrotor_EKF_U.u_k[2] * 2.0) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t3 * t6 * t8 * t9 * quadrotor_EKF_U.u_k[0] * 3.0)
    + quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t4 * t9 * t10
    * quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t5 * t6 * t7 * t8 * quadrotor_EKF_U.u_k[1]) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t5 * t6 * t8 * t9 *
    quadrotor_EKF_U.u_k[0] * 3.0) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t4 * t7 * t8 * t10 * quadrotor_EKF_U.u_k[0]) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t4 * t6 * t8 * t10 *
    quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t4 * t8 * t9 * t10 * quadrotor_EKF_U.u_k[1] *
    3.0) + quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t4 * t7 *
    t21 * quadrotor_EKF_U.u_k[0] * 2.0) - quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t3 * t6 * t7 * t21 * quadrotor_EKF_U.u_k[1] *
    2.0) - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t5 * t6 * t7 *
    t21 * quadrotor_EKF_U.u_k[1] * 2.0) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * t4 * t7 * t10 * t21 * quadrotor_EKF_U.u_k[0] *
    2.0) - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t4 * t6 * t10
    * t21 * quadrotor_EKF_U.u_k[2]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t6 * t8 * t10 * t72) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] *
    t6 * t10 * t21 * t72) - quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] *
    quadrotor_EKF_DW.x[11] * t6 * t8 * t10 * t72) + quadrotor_EKF_U.params[3] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t6 * t10 * t21 * t72) -
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] *
    t6 * t8 * t10 * t14) + quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] *
    quadrotor_EKF_DW.x[11] * t6 * t8 * t10 * t16) + quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] * t6 * t10 * t14 * t21) -
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[11] *
    t6 * t10 * t16 * t21) + quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] *
    quadrotor_EKF_DW.x[10] * t3 * t7 * t9 * t72 * 2.0) - quadrotor_EKF_U.params
    [3] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t7 * t9 * t72 *
    2.0) - quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] *
    quadrotor_EKF_DW.x[10] * t3 * t7 * t9 * t14 * 2.0) + quadrotor_EKF_U.params
    [4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t7 * t9 * t16 *
    2.0) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t5 * t6 *
    t7 * t8 * quadrotor_EKF_U.u_k[1]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t5 * t6 * t8 * t9 * quadrotor_EKF_U.u_k[0] *
    3.0) - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t4 * t7 *
    t8 * t10 * quadrotor_EKF_U.u_k[0]) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t4 * t8 * t9 * t10 * quadrotor_EKF_U.u_k[1]
    * 3.0) - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t5 *
    t7 * t9 * t10 * quadrotor_EKF_U.u_k[2] * 2.0) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t5 * t6 * t7 * t8 * t10 * quadrotor_EKF_U.u_k[1])
    + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t5 * t6 * t8 * t9 *
    t10 * quadrotor_EKF_U.u_k[0] * 3.0) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t3 * t5 * t6 * t7 * t21 * quadrotor_EKF_U.u_k[1]
    * 2.0) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] * t3 * t4 *
    t7 * t10 * t21 * quadrotor_EKF_U.u_k[0] * 2.0) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * t5 * t6 * t7 * t10 * t21 * quadrotor_EKF_U.u_k[1]
    * 2.0) - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * t3 * t5 *
    t6 * t7 * t8 * quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] *
                       quadrotor_EKF_U.params[4] * t3 * t5 * t6 * t8 * t9 *
                       quadrotor_EKF_U.u_k[0] * 3.0) + quadrotor_EKF_U.params[3]
                      * quadrotor_EKF_U.params[4] * t3 * t4 * t7 * t8 * t10 *
                      quadrotor_EKF_U.u_k[0]) - quadrotor_EKF_U.params[3] *
                     quadrotor_EKF_U.params[4] * t3 * t4 * t8 * t9 * t10 *
                     quadrotor_EKF_U.u_k[1] * 3.0) + quadrotor_EKF_U.params[3] *
                    quadrotor_EKF_U.params[4] * t3 * t5 * t7 * t9 * t10 *
                    quadrotor_EKF_U.u_k[2] * 2.0) - quadrotor_EKF_U.params[3] *
                   quadrotor_EKF_U.params[4] * t5 * t6 * t7 * t8 * t10 *
                   quadrotor_EKF_U.u_k[1]) - quadrotor_EKF_U.params[3] *
                  quadrotor_EKF_U.params[4] * t5 * t6 * t8 * t9 * t10 *
                  quadrotor_EKF_U.u_k[0] * 3.0) + quadrotor_EKF_U.params[3] *
                 quadrotor_EKF_U.params[4] * t3 * t5 * t6 * t7 * t21 *
                 quadrotor_EKF_U.u_k[1] * 2.0) - quadrotor_EKF_U.params[3] *
                quadrotor_EKF_U.params[4] * t3 * t4 * t7 * t10 * t21 *
                quadrotor_EKF_U.u_k[0] * 2.0) + quadrotor_EKF_U.params[3] *
               quadrotor_EKF_U.params[4] * t5 * t6 * t7 * t10 * t21 *
               quadrotor_EKF_U.u_k[1] * 2.0) + quadrotor_EKF_U.params[2] *
              quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t7 * t9 *
              t10 * t16 * 4.0) - quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x
             [9] * quadrotor_EKF_DW.x[10] * t3 * t7 * t9 * t10 * t72 * 2.0) +
            quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[11] *
            quadrotor_EKF_DW.x[10] * t3 * t6 * t7 * t9 * t72 * 2.0) -
           quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] *
           quadrotor_EKF_DW.x[10] * t3 * t7 * t9 * t10 * t14 * 4.0) +
          quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] *
          quadrotor_EKF_DW.x[10] * t3 * t7 * t9 * t10 * t72 * 2.0) -
         quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[11] *
         quadrotor_EKF_DW.x[10] * t3 * t6 * t7 * t9 * t72 * 2.0) +
        quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x
        [10] * t3 * t7 * t9 * t10 * t14 * 2.0) - quadrotor_EKF_U.params[4] *
       quadrotor_EKF_DW.x[9] * quadrotor_EKF_DW.x[10] * t3 * t7 * t9 * t10 * t16
       * 2.0) - quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] *
      quadrotor_EKF_DW.x[10] * t3 * t6 * t7 * t9 * t14 * 2.0) +
     quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] * quadrotor_EKF_DW.x[10]
     * t3 * t6 * t7 * t9 * t16 * 2.0) * 0.005;
  rtb_F_k[117] = 0.0;
  rtb_F_k[118] = 0.0;
  rtb_F_k[119] = 0.0;
  rtb_F_k[120] = 0.0;
  rtb_F_k[121] = 0.0;
  rtb_F_k[122] = 0.0;
  rtb_F_k[123] = 0.0;
  rtb_F_k[124] = 0.0;
  rtb_F_k[125] = 0.0;
  rtb_F_k[126] = 0.005;
  rtb_F_k[127] = 0.0;
  rtb_F_k[128] = 0.0;
  rtb_F_k[129] = 0.0;
  rtb_F_k[130] = 0.0;
  rtb_F_k[131] = 0.0;
  rtb_F_k[132] = 1.0;
  rtb_F_k[133] = 0.0;
  rtb_F_k[134] = 0.0;
  rtb_F_k[135] = 0.0;
  rtb_F_k[136] = 0.0;
  rtb_F_k[137] = 0.0;
  rtb_F_k[138] = 0.0;
  rtb_F_k[139] = 0.0;
  rtb_F_k[140] = 0.0;
  rtb_F_k[141] = 0.0;
  rtb_F_k[142] = 0.0;
  rtb_F_k[143] = 0.0;
  rtb_F_k[144] = 0.0;
  rtb_F_k[145] = 0.0;
  rtb_F_k[146] = 0.0;
  rtb_F_k[147] = 0.0;
  rtb_F_k[148] = 0.005;
  rtb_F_k[149] = 0.0;
  rtb_F_k[150] = 0.0;
  rtb_F_k[151] = 0.0;
  rtb_F_k[152] = 0.0;
  rtb_F_k[153] = 0.0;
  rtb_F_k[154] = 1.0;
  rtb_F_k[155] = 0.0;
  rtb_F_k[156] = 0.0;
  rtb_F_k[157] = 0.0;
  rtb_F_k[158] = 0.0;
  rtb_F_k[159] = 0.0;
  rtb_F_k[160] = 0.0;
  rtb_F_k[161] = 0.0;
  rtb_F_k[162] = 0.0;
  rtb_F_k[163] = 0.0;
  rtb_F_k[164] = 0.0;
  rtb_F_k[165] = 0.0;
  rtb_F_k[166] = 0.0;
  rtb_F_k[167] = 0.0;
  rtb_F_k[168] = 0.0;
  rtb_F_k[169] = 0.0;
  rtb_F_k[170] = 0.005;
  rtb_F_k[171] = 0.0;
  rtb_F_k[172] = 0.0;
  rtb_F_k[173] = 0.0;
  rtb_F_k[174] = 0.0;
  rtb_F_k[175] = 0.0;
  rtb_F_k[176] = 1.0;
  rtb_F_k[177] = 0.0;
  rtb_F_k[178] = 0.0;
  rtb_F_k[179] = 0.0;
  rtb_F_k[180] = 0.0;
  rtb_F_k[181] = 0.0;
  rtb_F_k[182] = 0.0;
  rtb_F_k[183] = 0.0;
  rtb_F_k[184] = 0.0;
  rtb_F_k[185] = 0.0;
  rtb_F_k[186] = 0.0;
  rtb_F_k[187] = 0.0;
  rtb_F_k[188] = 0.0;
  rtb_F_k[189] = 0.0;
  rtb_F_k[190] = 0.0;
  rtb_F_k[191] = 0.0;
  rtb_F_k[192] = 0.005;
  rtb_F_k[193] = 0.0;
  rtb_F_k[194] = 0.0;
  rtb_F_k[195] = 0.0;
  rtb_F_k[196] = 0.0;
  rtb_F_k[197] = 0.0;
  rtb_F_k[198] = ((((((((((((((quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[10] * t20 * 0.5 +
    quadrotor_EKF_DW.x[10] * t14 * t20 * 0.5) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[10] * t20 * 0.5) +
    quadrotor_EKF_DW.x[11] * t7 * t9 * t10 * t14) - quadrotor_EKF_DW.x[11] * t7 *
    t9 * t10 * t16) - quadrotor_EKF_DW.x[10] * t3 * t6 * t8 * t14) +
    quadrotor_EKF_DW.x[10] * t3 * t6 * t8 * t16) + quadrotor_EKF_DW.x[9] * t6 *
    t7 * t9 * t10 * t14 * 2.0) - quadrotor_EKF_DW.x[9] * t6 * t7 * t9 * t10 *
                        t16 * 2.0) - quadrotor_EKF_U.params[2] *
                       quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] * t7 *
                       t9 * t10) + quadrotor_EKF_U.params[3] *
                      quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] * t7 *
                      t9 * t10) + quadrotor_EKF_U.params[2] *
                     quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[10] * t3 *
                     t6 * t8) - quadrotor_EKF_U.params[3] *
                    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[10] * t3 * t6
                    * t8) - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4]
                   * quadrotor_EKF_DW.x[9] * t6 * t7 * t9 * t10 * 2.0) +
                  quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
                  quadrotor_EKF_DW.x[9] * t6 * t7 * t9 * t10 * 2.0) * (t11 * t12
    * t13) * 0.005 + 1.0;
  rtb_F_k[199] = ((((((((((((((((-quadrotor_EKF_DW.x[11] * t10 * t16 -
    quadrotor_EKF_DW.x[9] * t6 * t10 * t16 * 2.0) - quadrotor_EKF_DW.x[11] * t8 *
    t10 * t14) + quadrotor_EKF_DW.x[11] * t8 * t10 * t16) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x
    [11] * t10) + quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[11] * t10) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] * t8 * t10) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9]
    * t6 * t10 * 2.0) - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[11] * t8 * t10) - quadrotor_EKF_DW.x[9] * t6 * t8 * t10 *
    t14 * 2.0) + quadrotor_EKF_DW.x[9] * t6 * t8 * t10 * t16 * 2.0) -
                       quadrotor_EKF_DW.x[10] * t3 * t6 * t7 * t9 * t14) +
                      quadrotor_EKF_DW.x[10] * t3 * t6 * t7 * t9 * t16) +
                     quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
                     quadrotor_EKF_DW.x[9] * t6 * t8 * t10 * 2.0) -
                    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
                    quadrotor_EKF_DW.x[9] * t6 * t8 * t10 * 2.0) +
                   quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
                   quadrotor_EKF_DW.x[10] * t3 * t6 * t7 * t9) -
                  quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
                  quadrotor_EKF_DW.x[10] * t3 * t6 * t7 * t9) * (t11 * t12 * t31)
    * 0.005;
  rtb_F_k[200] = ((((((((((((((((((((((((((((((-quadrotor_EKF_U.params[2] *
    quadrotor_EKF_DW.x[10] * t3 * t72 + quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[10] * t3 * t14) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x
    [10] * t3) - quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[10] * t3 * t10 *
    t16) + quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[10] * t3 * t8 * t72) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[10] * t3 * t10 * t72) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[10] * t3 * t10 * t14) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[10] * t3 * t8 * t72) -
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[10] * t3 * t8 * t14) +
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[10] * t3 * t8 * t16) -
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[10] * t3 * t10 * t14) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] * t7 * t9 * t10 * t72 *
    2.0) - quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] * t7 * t9 * t16 *
    t75 * 2.0) + quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] * t7 * t9 *
    t72 * t75 * 2.0) + quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] * t7 *
    t9 * t10 * t72 * 2.0) + quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] *
    t7 * t9 * t14 * t75 * 2.0) - quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x
    [9] * t7 * t9 * t72 * t75 * 2.0) + quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[9] * t7 * t9 * t10 * t14 * 2.0) - quadrotor_EKF_U.params
    [4] * quadrotor_EKF_DW.x[9] * t7 * t9 * t10 * t16 * 2.0) -
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * t7 * t9 * t14 * t75 *
    2.0) + quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * t7 * t9 * t16 *
    t75 * 2.0) + quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[10] * t3 * t8 *
    t10 * t16 * 2.0) - quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[10] * t3 *
    t8 * t10 * t72) - quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[10] * t3 *
    t8 * t10 * t14 * 2.0) + quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[10] *
                        t3 * t8 * t10 * t72) + quadrotor_EKF_U.params[4] *
                       quadrotor_EKF_DW.x[10] * t3 * t8 * t10 * t14) -
                      quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[10] * t3 *
                      t8 * t10 * t16) - quadrotor_EKF_U.params[2] *
                     quadrotor_EKF_DW.x[11] * t6 * t7 * t9 * t10 * t72) +
                    quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[11] * t6 * t7
                    * t9 * t10 * t72) + quadrotor_EKF_U.params[4] *
                   quadrotor_EKF_DW.x[11] * t6 * t7 * t9 * t10 * t14) -
                  quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] * t6 * t7 *
                  t9 * t10 * t16) * (t11 * t12 * t13 * t71) * -0.005;
  rtb_F_k[201] = 0.0;
  rtb_F_k[202] = 0.0;
  rtb_F_k[203] = 0.0;
  rtb_F_k[204] = 0.0;
  rtb_F_k[205] = 0.0;
  rtb_F_k[206] = 0.0;
  rtb_F_k[207] = 0.0;
  rtb_F_k[208] = 0.0;
  rtb_F_k[209] = 0.0;
  rtb_F_k[210] = 0.0;
  rtb_F_k[211] = 0.0;
  rtb_F_k[212] = 0.0;
  rtb_F_k[213] = 0.0;
  rtb_F_k[214] = 0.005;
  rtb_F_k[215] = 0.0;
  rtb_F_k[216] = 0.0;
  rtb_F_k[217] = 0.0;
  rtb_F_k[218] = 0.0;
  rtb_F_k[219] = (((((((((((((quadrotor_EKF_DW.x[9] * t14 * t20 * 0.5 +
    quadrotor_EKF_DW.x[11] * t3 * t14) - quadrotor_EKF_DW.x[11] * t3 * t8 * t14)
    + quadrotor_EKF_DW.x[11] * t3 * t8 * t16) + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] * t20 * 0.5) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x
    [11] * t3) - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[9] * t20 * 0.5) - quadrotor_EKF_U.params[2] *
                        quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] * t3)
                       + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
                       quadrotor_EKF_DW.x[11] * t3 * t8) -
                      quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
                      quadrotor_EKF_DW.x[11] * t3 * t8) - quadrotor_EKF_DW.x[9] *
                     t3 * t6 * t8 * t14) + quadrotor_EKF_DW.x[9] * t3 * t6 * t8 *
                    t16) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4]
                   * quadrotor_EKF_DW.x[9] * t3 * t6 * t8) -
                  quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
                  quadrotor_EKF_DW.x[9] * t3 * t6 * t8) * (t11 * t12 * t13) *
    0.005;
  rtb_F_k[220] = (((((((quadrotor_EKF_DW.x[11] * t3 * t7 * t9 * t14 -
                        quadrotor_EKF_DW.x[11] * t3 * t7 * t9 * t16) +
                       quadrotor_EKF_DW.x[9] * t3 * t6 * t7 * t9 * t14) -
                      quadrotor_EKF_DW.x[9] * t3 * t6 * t7 * t9 * t16) -
                     quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
                     quadrotor_EKF_DW.x[11] * t3 * t7 * t9) +
                    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
                    quadrotor_EKF_DW.x[11] * t3 * t7 * t9) -
                   quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
                   quadrotor_EKF_DW.x[9] * t3 * t6 * t7 * t9) +
                  quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
                  quadrotor_EKF_DW.x[9] * t3 * t6 * t7 * t9) * (t11 * t12 * t31)
    * -0.005 + 1.0;
  rtb_F_k[221] = (((((((((((((((((((((((((quadrotor_EKF_U.params[2] *
    quadrotor_EKF_DW.x[11] * t20 * t72 * 0.5 + quadrotor_EKF_U.params[2] *
    quadrotor_EKF_DW.x[9] * t3 * t72) - quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[9] * t3 * t14) - quadrotor_EKF_U.params[4] *
    quadrotor_EKF_DW.x[11] * t14 * t20 * 0.5) - quadrotor_EKF_U.params[2] *
    quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9]
    * t3) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] * t20 * 0.5) +
    quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] * t3 * t10 * t16) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] * t3 * t8 * t72) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] * t3 * t10 * t72) -
    quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] * t3 * t10 * t14) +
    quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] * t3 * t8 * t72) +
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * t3 * t8 * t14) -
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * t3 * t8 * t16) +
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * t3 * t10 * t14) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] * t3 * t8 * t10 * t16 *
    2.0) + quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[9] * t3 * t8 * t10 *
    t72) - quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[11] * t3 * t6 * t8 *
    t72) + quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] * t3 * t8 * t10 *
    t14 * 2.0) - quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] * t3 * t8 *
    t10 * t72) + quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[11] * t3 * t6 *
                        t8 * t72) - quadrotor_EKF_U.params[4] *
                       quadrotor_EKF_DW.x[9] * t3 * t8 * t10 * t14) +
                      quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * t3 *
                      t8 * t10 * t16) + quadrotor_EKF_U.params[4] *
                     quadrotor_EKF_DW.x[11] * t3 * t6 * t8 * t14) -
                    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[11] * t3 * t6
                    * t8 * t16) - quadrotor_EKF_U.params[2] *
                   quadrotor_EKF_DW.x[10] * t7 * t9 * t10 * t16 * 2.0) +
                  quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[10] * t7 * t9 *
                  t10 * t14 * 2.0) * (t11 * t12 * t13 * t71) * 0.005;
  rtb_F_k[222] = 0.0;
  rtb_F_k[223] = 0.0;
  rtb_F_k[224] = 0.0;
  rtb_F_k[225] = 0.0;
  rtb_F_k[226] = 0.0;
  rtb_F_k[227] = 0.0;
  rtb_F_k[228] = 0.0;
  rtb_F_k[229] = 0.0;
  rtb_F_k[230] = 0.0;
  rtb_F_k[231] = 0.0;
  rtb_F_k[232] = 0.0;
  rtb_F_k[233] = 0.0;
  rtb_F_k[234] = 0.0;
  rtb_F_k[235] = 0.0;
  rtb_F_k[236] = 0.005;
  rtb_F_k[237] = 0.0;
  rtb_F_k[238] = 0.0;
  rtb_F_k[239] = 0.0;
  rtb_F_k[240] = ((((((((((quadrotor_EKF_DW.x[10] * t3 * t8 * t14 +
    -quadrotor_EKF_DW.x[10] * t3 * t14) - quadrotor_EKF_DW.x[10] * t3 * t8 * t16)
    + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] *
    quadrotor_EKF_DW.x[10] * t3) + quadrotor_EKF_U.params[2] *
                        quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[10] * t3)
                       - quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
                       quadrotor_EKF_DW.x[10] * t3 * t8) +
                      quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
                      quadrotor_EKF_DW.x[10] * t3 * t8) - quadrotor_EKF_DW.x[9] *
                     t7 * t9 * t10 * t14) + quadrotor_EKF_DW.x[9] * t7 * t9 *
                    t10 * t16) + quadrotor_EKF_U.params[2] *
                   quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * t7 * t9 *
                   t10) - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
                  quadrotor_EKF_DW.x[9] * t7 * t9 * t10) * (t11 * t12 * t13) *
    -0.005;
  rtb_F_k[241] = ((((((((((-quadrotor_EKF_DW.x[9] * t10 * t16 -
    quadrotor_EKF_DW.x[9] * t8 * t10 * t14) + quadrotor_EKF_DW.x[9] * t8 * t10 *
    t16) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] *
    quadrotor_EKF_DW.x[9] * t10) + quadrotor_EKF_U.params[3] *
                        quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * t10)
                       + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[4] *
                       quadrotor_EKF_DW.x[9] * t8 * t10) -
                      quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
                      quadrotor_EKF_DW.x[9] * t8 * t10) - quadrotor_EKF_DW.x[10]
                     * t3 * t7 * t9 * t14) + quadrotor_EKF_DW.x[10] * t3 * t7 *
                    t9 * t16) + quadrotor_EKF_U.params[2] *
                   quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[10] * t3 * t7 *
                   t9) - quadrotor_EKF_U.params[3] * quadrotor_EKF_U.params[4] *
                  quadrotor_EKF_DW.x[10] * t3 * t7 * t9) * (t11 * t12 * t31) *
    0.005;
  rtb_F_k[242] = ((((((((((quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[10] *
    t20 * t72 * 0.5 - quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[10] * t14 *
    t20 * 0.5) + quadrotor_EKF_U.params[2] * quadrotor_EKF_U.params[3] *
    quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[10] * t20 * 0.5) -
    quadrotor_EKF_U.params[2] * quadrotor_EKF_DW.x[10] * t3 * t6 * t8 * t72) +
                        quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[10] * t3 *
                        t6 * t8 * t72) + quadrotor_EKF_U.params[4] *
                       quadrotor_EKF_DW.x[10] * t3 * t6 * t8 * t14) -
                      quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[10] * t3 *
                      t6 * t8 * t16) + quadrotor_EKF_U.params[2] *
                     quadrotor_EKF_DW.x[9] * t6 * t7 * t9 * t10 * t72) -
                    quadrotor_EKF_U.params[3] * quadrotor_EKF_DW.x[9] * t6 * t7 *
                    t9 * t10 * t72) - quadrotor_EKF_U.params[4] *
                   quadrotor_EKF_DW.x[9] * t6 * t7 * t9 * t10 * t14) +
                  quadrotor_EKF_U.params[4] * quadrotor_EKF_DW.x[9] * t6 * t7 *
                  t9 * t10 * t16) * (t11 * t12 * t13 * t71) * 0.005 + 1.0;
  rtb_F_k[243] = 0.0;
  rtb_F_k[244] = 0.0;
  rtb_F_k[245] = 0.0;
  rtb_F_k[246] = 0.0;
  rtb_F_k[247] = 0.0;
  rtb_F_k[248] = 0.0;
  rtb_F_k[249] = 0.0;
  rtb_F_k[250] = 0.0;
  rtb_F_k[251] = 0.0;
  rtb_F_k[252] = 0.0;
  rtb_F_k[253] = 0.0;
  rtb_F_k[254] = 0.0;
  rtb_F_k[255] = 0.0;
  rtb_F_k[256] = 0.0;
  rtb_F_k[257] = 0.0;
  rtb_F_k[258] = 0.0;
  rtb_F_k[259] = 0.0;
  rtb_F_k[260] = 0.0;
  rtb_F_k[261] = 0.0;
  rtb_F_k[262] = 0.0;
  rtb_F_k[263] = 0.0;
  rtb_F_k[264] = 1.0;
  rtb_F_k[265] = 0.0;
  rtb_F_k[266] = 0.0;
  rtb_F_k[267] = 0.0;
  rtb_F_k[268] = 0.0;
  rtb_F_k[269] = 0.0;
  rtb_F_k[270] = 0.0;
  rtb_F_k[271] = 0.0;
  rtb_F_k[272] = 0.0;
  rtb_F_k[273] = 0.0;
  rtb_F_k[274] = 0.0;
  rtb_F_k[275] = 0.0;
  rtb_F_k[276] = 0.0;
  rtb_F_k[277] = 0.0;
  rtb_F_k[278] = 0.0;
  rtb_F_k[279] = 0.0;
  rtb_F_k[280] = 0.0;
  rtb_F_k[281] = 0.0;
  rtb_F_k[282] = 0.0;
  rtb_F_k[283] = 0.0;
  rtb_F_k[284] = 0.0;
  rtb_F_k[285] = 0.0;
  rtb_F_k[286] = 1.0;
  rtb_F_k[287] = 0.0;
  rtb_F_k[288] = 0.0;
  rtb_F_k[289] = 0.0;
  rtb_F_k[290] = 0.0;
  rtb_F_k[291] = 0.0;
  rtb_F_k[292] = 0.0;
  rtb_F_k[293] = 0.0;
  rtb_F_k[294] = 0.0;
  rtb_F_k[295] = 0.0;
  rtb_F_k[296] = 0.0;
  rtb_F_k[297] = 0.0;
  rtb_F_k[298] = 0.0;
  rtb_F_k[299] = 0.0;
  rtb_F_k[300] = 0.0;
  rtb_F_k[301] = 0.0;
  rtb_F_k[302] = 0.0;
  rtb_F_k[303] = 0.0;
  rtb_F_k[304] = 0.0;
  rtb_F_k[305] = 0.0;
  rtb_F_k[306] = 0.0;
  rtb_F_k[307] = 0.0;
  rtb_F_k[308] = 1.0;
  rtb_F_k[309] = 0.0;
  rtb_F_k[310] = 0.0;
  rtb_F_k[311] = 0.0;
  rtb_F_k[312] = 0.0;
  rtb_F_k[313] = 0.0;
  rtb_F_k[314] = 0.0;
  rtb_F_k[315] = 0.0;
  rtb_F_k[316] = 0.0;
  rtb_F_k[317] = 0.0;
  rtb_F_k[318] = 0.0;
  rtb_F_k[319] = 0.0;
  rtb_F_k[320] = 0.0;
  rtb_F_k[321] = 0.0;
  rtb_F_k[322] = 0.0;
  rtb_F_k[323] = 0.0;
  rtb_F_k[324] = 0.0;
  rtb_F_k[325] = 0.0;
  rtb_F_k[326] = 0.0;
  rtb_F_k[327] = 0.0;
  rtb_F_k[328] = 0.0;
  rtb_F_k[329] = 0.0;
  rtb_F_k[330] = 1.0;
  rtb_F_k[331] = 0.0;
  rtb_F_k[332] = 0.0;
  rtb_F_k[333] = 0.0;
  rtb_F_k[334] = 0.0;
  rtb_F_k[335] = 0.0;
  rtb_F_k[336] = 0.0;
  rtb_F_k[337] = 0.0;
  rtb_F_k[338] = 0.0;
  rtb_F_k[339] = 0.0;
  rtb_F_k[340] = 0.0;
  rtb_F_k[341] = 0.0;
  rtb_F_k[342] = 0.0;
  rtb_F_k[343] = 0.0;
  rtb_F_k[344] = 0.0;
  rtb_F_k[345] = 0.0;
  rtb_F_k[346] = 0.0;
  rtb_F_k[347] = 0.0;
  rtb_F_k[348] = 0.0;
  rtb_F_k[349] = 0.0;
  rtb_F_k[350] = 0.0;
  rtb_F_k[351] = 0.0;
  rtb_F_k[352] = 1.0;
  rtb_F_k[353] = 0.0;
  rtb_F_k[354] = 0.0;
  rtb_F_k[355] = 0.0;
  rtb_F_k[356] = 0.0;
  rtb_F_k[357] = 0.0;
  rtb_F_k[358] = 0.0;
  rtb_F_k[359] = 0.0;
  rtb_F_k[360] = 0.0;
  rtb_F_k[361] = 0.0;
  rtb_F_k[362] = 0.0;
  rtb_F_k[363] = 0.0;
  rtb_F_k[364] = 0.0;
  rtb_F_k[365] = 0.0;
  rtb_F_k[366] = 0.0;
  rtb_F_k[367] = 0.0;
  rtb_F_k[368] = 0.0;
  rtb_F_k[369] = 0.0;
  rtb_F_k[370] = 0.0;
  rtb_F_k[371] = 0.0;
  rtb_F_k[372] = 0.0;
  rtb_F_k[373] = 0.0;
  rtb_F_k[374] = 1.0;
  rtb_F_k[375] = 0.0;
  rtb_F_k[376] = 0.0;
  rtb_F_k[377] = 0.0;
  rtb_F_k[378] = 0.0;
  rtb_F_k[379] = 0.0;
  rtb_F_k[380] = 0.0;
  rtb_F_k[381] = 0.0;
  rtb_F_k[382] = 0.0;
  rtb_F_k[383] = 0.0;
  rtb_F_k[384] = 0.0;
  rtb_F_k[385] = 0.0;
  rtb_F_k[386] = 0.0;
  rtb_F_k[387] = 0.0;
  rtb_F_k[388] = 0.0;
  rtb_F_k[389] = 0.0;
  rtb_F_k[390] = 0.0;
  rtb_F_k[391] = 0.0;
  rtb_F_k[392] = 0.0;
  rtb_F_k[393] = 0.0;
  rtb_F_k[394] = 0.0;
  rtb_F_k[395] = 0.0;
  rtb_F_k[396] = 1.0;
  rtb_F_k[397] = 0.0;
  rtb_F_k[398] = 0.0;
  rtb_F_k[399] = 0.0;
  rtb_F_k[400] = 0.0;
  rtb_F_k[401] = 0.0;
  rtb_F_k[402] = 0.0;
  rtb_F_k[403] = 0.0;
  rtb_F_k[404] = 0.0;
  rtb_F_k[405] = 0.0;
  rtb_F_k[406] = 0.0;
  rtb_F_k[407] = 0.0;
  rtb_F_k[408] = 0.0;
  rtb_F_k[409] = 0.0;
  rtb_F_k[410] = 0.0;
  rtb_F_k[411] = 0.0;
  rtb_F_k[412] = 0.0;
  rtb_F_k[413] = 0.0;
  rtb_F_k[414] = 0.0;
  rtb_F_k[415] = 0.0;
  rtb_F_k[416] = 0.0;
  rtb_F_k[417] = 0.0;
  rtb_F_k[418] = 1.0;
  rtb_F_k[419] = 0.0;
  rtb_F_k[420] = 0.0;
  rtb_F_k[421] = 0.0;
  rtb_F_k[422] = 0.0;
  rtb_F_k[423] = 0.0;
  rtb_F_k[424] = 0.0;
  rtb_F_k[425] = 0.0;
  rtb_F_k[426] = 0.0;
  rtb_F_k[427] = 0.0;
  rtb_F_k[428] = 0.0;
  rtb_F_k[429] = 0.0;
  rtb_F_k[430] = 0.0;
  rtb_F_k[431] = 0.0;
  rtb_F_k[432] = 0.0;
  rtb_F_k[433] = 0.0;
  rtb_F_k[434] = 0.0;
  rtb_F_k[435] = 0.0;
  rtb_F_k[436] = 0.0;
  rtb_F_k[437] = 0.0;
  rtb_F_k[438] = 0.0;
  rtb_F_k[439] = 0.0;
  rtb_F_k[440] = 1.0;

  // Delay: '<Root>/Resettable Delay' incorporates:
  //   Inport: '<Root>/NReset'
  //   Inport: '<Root>/P_0'

  if (rt_ZCFcn(RISING_ZERO_CROSSING,
               &quadrotor_EKF_PrevZCX.ResettableDelay_Reset_ZCE,
               (quadrotor_EKF_U.NReset)) != NO_ZCEVENT) {
    quadrotor_EKF_DW.icLoad_i = 1U;
  }

  if (quadrotor_EKF_DW.icLoad_i != 0) {
    memcpy(&quadrotor_EKF_DW.ResettableDelay_DSTATE[0], &quadrotor_EKF_U.P_0[0],
           441U * sizeof(real_T));
  }

  // PermuteDimensions: '<S4>/Transpose'
  yElIdx = 0;
  uElOffset1 = 0;
  for (ntIdx1 = 0; ntIdx1 < 21; ntIdx1++) {
    uElOffset0 = uElOffset1;
    for (ntIdx0 = 0; ntIdx0 < 21; ntIdx0++) {
      rtb_Product_e[yElIdx] = rtb_F_k[uElOffset0];
      yElIdx++;
      uElOffset0 += 21;
    }

    uElOffset1++;
  }

  // End of PermuteDimensions: '<S4>/Transpose'

  // Product: '<S4>/Product' incorporates:
  //   Delay: '<Root>/Resettable Delay'

  for (yElIdx = 0; yElIdx < 21; yElIdx++) {
    for (uElOffset1 = 0; uElOffset1 < 21; uElOffset1++) {
      tmp[yElIdx + 21 * uElOffset1] = 0.0;
      for (ntIdx1 = 0; ntIdx1 < 21; ntIdx1++) {
        tmp[yElIdx + 21 * uElOffset1] +=
          quadrotor_EKF_DW.ResettableDelay_DSTATE[21 * ntIdx1 + yElIdx] *
          rtb_Product_e[21 * uElOffset1 + ntIdx1];
      }
    }
  }

  // Sum: '<S4>/Add' incorporates:
  //   Inport: '<Root>/Q_k'
  //   Product: '<S4>/Product'

  for (yElIdx = 0; yElIdx < 21; yElIdx++) {
    for (uElOffset1 = 0; uElOffset1 < 21; uElOffset1++) {
      t2 = 0.0;
      for (ntIdx1 = 0; ntIdx1 < 21; ntIdx1++) {
        t2 += rtb_F_k[21 * ntIdx1 + yElIdx] * tmp[21 * uElOffset1 + ntIdx1];
      }

      rtb_Add_a[yElIdx + 21 * uElOffset1] = quadrotor_EKF_U.Q_k[21 * uElOffset1
        + yElIdx] + t2;
    }
  }

  // End of Sum: '<S4>/Add'

  // MATLAB Function: '<S10>/linearized output model matrix' incorporates:
  //   Delay: '<Root>/Resettable Delay1'
  //   Inport: '<Root>/B_earth'
  //   MATLAB Function: '<S5>/nonlinear model'

  // MATLAB Function 'innovation//residual covariance/linearized output model/linearized output model matrix': '<S11>:1' 
  // H_K
  //     H_K = H_K(X1,X2,X3,PHI,THETA,PSI,DX1,DX2,DX3,DPHI,DTHETA,DPSI,DDX1,DDX2,DDX3,B_A1,B_A2,B_A3,B_G1,B_G2,B_G3,B_EARTH1,B_EARTH2,B_EARTH3) 
  //     This function was generated by the Symbolic Math Toolbox version 6.3.
  //     02-Oct-2016 22:52:35
  // '<S11>:1:8'
  t2 = sin(rtb_f[4]);

  // '<S11>:1:9'
  t3 = cos(rtb_f[5]);

  // '<S11>:1:10'
  t4 = cos(rtb_f[4]);

  // '<S11>:1:11'
  t5 = sin(rtb_f[5]);

  // '<S11>:1:12'
  t6 = sin(rtb_f[3]);

  // '<S11>:1:13'
  t7 = cos(rtb_f[3]);

  // '<S11>:1:14'
  // '<S11>:1:15'
  // '<S11>:1:16'
  t10 = t2 * t3 * t6 + t5 * t7;

  // '<S11>:1:17'
  // '<S11>:1:18'
  // '<S11>:1:19'
  t12 = t3 * t7 - t2 * t5 * t6;

  // '<S11>:1:20'
  // '<S11>:1:21'
  // '<S11>:1:22'
  t16 = t2 * t5 * t7 + t3 * t6;

  // '<S11>:1:23'
  // '<S11>:1:24'
  // '<S11>:1:25'
  t18 = t5 * t6 - t2 * t3 * t7;

  // '<S11>:1:26'
  rtb_H_k[0] = 1.0;
  rtb_H_k[1] = 0.0;
  rtb_H_k[2] = 0.0;
  rtb_H_k[3] = 0.0;
  rtb_H_k[4] = 0.0;
  rtb_H_k[5] = 0.0;
  rtb_H_k[6] = 0.0;
  rtb_H_k[7] = 0.0;
  rtb_H_k[8] = 0.0;
  rtb_H_k[9] = 0.0;
  rtb_H_k[10] = 0.0;
  rtb_H_k[11] = 0.0;
  rtb_H_k[12] = 0.0;
  rtb_H_k[13] = 1.0;
  rtb_H_k[14] = 0.0;
  rtb_H_k[15] = 0.0;
  rtb_H_k[16] = 0.0;
  rtb_H_k[17] = 0.0;
  rtb_H_k[18] = 0.0;
  rtb_H_k[19] = 0.0;
  rtb_H_k[20] = 0.0;
  rtb_H_k[21] = 0.0;
  rtb_H_k[22] = 0.0;
  rtb_H_k[23] = 0.0;
  rtb_H_k[24] = 0.0;
  rtb_H_k[25] = 0.0;
  rtb_H_k[26] = 1.0;
  rtb_H_k[27] = 0.0;
  rtb_H_k[28] = 0.0;
  rtb_H_k[29] = 0.0;
  rtb_H_k[30] = 0.0;
  rtb_H_k[31] = 0.0;
  rtb_H_k[32] = 0.0;
  rtb_H_k[33] = 0.0;
  rtb_H_k[34] = 0.0;
  rtb_H_k[35] = 0.0;
  rtb_H_k[36] = 0.0;
  rtb_H_k[37] = 0.0;
  rtb_H_k[38] = 0.0;
  rtb_H_k[39] = 0.0;
  rtb_H_k[40] = (-quadrotor_EKF_DW.x[13] * t16 - quadrotor_EKF_DW.x[12] * t18) -
    quadrotor_EKF_DW.x[14] * t4 * t7;
  rtb_H_k[41] = (quadrotor_EKF_DW.x[12] * t10 + quadrotor_EKF_DW.x[13] * t12) -
    quadrotor_EKF_DW.x[14] * t4 * t6;
  rtb_H_k[42] = 0.0;
  rtb_H_k[43] = 0.0;
  rtb_H_k[44] = 0.0;
  rtb_H_k[45] = 0.0;
  rtb_H_k[46] = (-quadrotor_EKF_U.B_earth[1] * t16 - quadrotor_EKF_U.B_earth[0] *
                 t18) - quadrotor_EKF_U.B_earth[2] * t4 * t7;
  rtb_H_k[47] = (quadrotor_EKF_U.B_earth[0] * t10 + quadrotor_EKF_U.B_earth[1] *
                 t12) - quadrotor_EKF_U.B_earth[2] * t4 * t6;
  rtb_H_k[48] = 0.0;
  rtb_H_k[49] = 0.0;
  rtb_H_k[50] = 0.0;
  rtb_H_k[51] = (quadrotor_EKF_DW.x[14] * t4 - quadrotor_EKF_DW.x[12] * t2 * t3)
    + quadrotor_EKF_DW.x[13] * t2 * t5;
  rtb_H_k[52] = (quadrotor_EKF_DW.x[12] * t3 * t4 * t6 + quadrotor_EKF_DW.x[14] *
                 t2 * t6) - quadrotor_EKF_DW.x[13] * t4 * t5 * t6;
  rtb_H_k[53] = (-quadrotor_EKF_DW.x[14] * t2 * t7 - quadrotor_EKF_DW.x[12] * t3
                 * t4 * t7) + quadrotor_EKF_DW.x[13] * t4 * t5 * t7;
  rtb_H_k[54] = 0.0;
  rtb_H_k[55] = 0.0;
  rtb_H_k[56] = 0.0;
  rtb_H_k[57] = (quadrotor_EKF_U.B_earth[2] * t4 - quadrotor_EKF_U.B_earth[0] *
                 t2 * t3) + quadrotor_EKF_U.B_earth[1] * t2 * t5;
  rtb_H_k[58] = (quadrotor_EKF_U.B_earth[0] * t3 * t4 * t6 +
                 quadrotor_EKF_U.B_earth[2] * t2 * t6) -
    quadrotor_EKF_U.B_earth[1] * t4 * t5 * t6;
  rtb_H_k[59] = (-quadrotor_EKF_U.B_earth[2] * t2 * t7 -
                 quadrotor_EKF_U.B_earth[0] * t3 * t4 * t7) +
    quadrotor_EKF_U.B_earth[1] * t4 * t5 * t7;
  rtb_H_k[60] = 0.0;
  rtb_H_k[61] = 0.0;
  rtb_H_k[62] = 0.0;
  rtb_H_k[63] = -quadrotor_EKF_DW.x[13] * t3 * t4 - quadrotor_EKF_DW.x[12] * t4 *
    t5;
  rtb_H_k[64] = -quadrotor_EKF_DW.x[13] * t10 + quadrotor_EKF_DW.x[12] * t12;
  rtb_H_k[65] = quadrotor_EKF_DW.x[12] * t16 - quadrotor_EKF_DW.x[13] * t18;
  rtb_H_k[66] = 0.0;
  rtb_H_k[67] = 0.0;
  rtb_H_k[68] = 0.0;
  rtb_H_k[69] = -quadrotor_EKF_U.B_earth[1] * t3 * t4 - quadrotor_EKF_U.B_earth
    [0] * t4 * t5;
  rtb_H_k[70] = -quadrotor_EKF_U.B_earth[1] * t10 + quadrotor_EKF_U.B_earth[0] *
    t12;
  rtb_H_k[71] = quadrotor_EKF_U.B_earth[0] * t16 - quadrotor_EKF_U.B_earth[1] *
    t18;
  rtb_H_k[72] = 0.0;
  rtb_H_k[73] = 0.0;
  rtb_H_k[74] = 0.0;
  rtb_H_k[75] = 0.0;
  rtb_H_k[76] = 0.0;
  rtb_H_k[77] = 0.0;
  rtb_H_k[78] = 0.0;
  rtb_H_k[79] = 0.0;
  rtb_H_k[80] = 0.0;
  rtb_H_k[81] = 0.0;
  rtb_H_k[82] = 0.0;
  rtb_H_k[83] = 0.0;
  rtb_H_k[84] = 0.0;
  rtb_H_k[85] = 0.0;
  rtb_H_k[86] = 0.0;
  rtb_H_k[87] = 0.0;
  rtb_H_k[88] = 0.0;
  rtb_H_k[89] = 0.0;
  rtb_H_k[90] = 0.0;
  rtb_H_k[91] = 0.0;
  rtb_H_k[92] = 0.0;
  rtb_H_k[93] = 0.0;
  rtb_H_k[94] = 0.0;
  rtb_H_k[95] = 0.0;
  rtb_H_k[96] = 0.0;
  rtb_H_k[97] = 0.0;
  rtb_H_k[98] = 0.0;
  rtb_H_k[99] = 0.0;
  rtb_H_k[100] = 0.0;
  rtb_H_k[101] = 0.0;
  rtb_H_k[102] = 0.0;
  rtb_H_k[103] = 0.0;
  rtb_H_k[104] = 0.0;
  rtb_H_k[105] = 0.0;
  rtb_H_k[106] = 0.0;
  rtb_H_k[107] = 0.0;
  rtb_H_k[108] = 0.0;
  rtb_H_k[109] = 0.0;
  rtb_H_k[110] = 0.0;
  rtb_H_k[111] = 0.0;
  rtb_H_k[112] = 0.0;
  rtb_H_k[113] = 0.0;
  rtb_H_k[114] = 1.0;
  rtb_H_k[115] = 0.0;
  rtb_H_k[116] = 0.0;
  rtb_H_k[117] = 0.0;
  rtb_H_k[118] = 0.0;
  rtb_H_k[119] = 0.0;
  rtb_H_k[120] = 0.0;
  rtb_H_k[121] = 0.0;
  rtb_H_k[122] = 0.0;
  rtb_H_k[123] = 0.0;
  rtb_H_k[124] = 0.0;
  rtb_H_k[125] = 0.0;
  rtb_H_k[126] = 0.0;
  rtb_H_k[127] = 1.0;
  rtb_H_k[128] = 0.0;
  rtb_H_k[129] = 0.0;
  rtb_H_k[130] = 0.0;
  rtb_H_k[131] = 0.0;
  rtb_H_k[132] = 0.0;
  rtb_H_k[133] = 0.0;
  rtb_H_k[134] = 0.0;
  rtb_H_k[135] = 0.0;
  rtb_H_k[136] = 0.0;
  rtb_H_k[137] = 0.0;
  rtb_H_k[138] = 0.0;
  rtb_H_k[139] = 0.0;
  rtb_H_k[140] = 1.0;
  rtb_H_k[141] = 0.0;
  rtb_H_k[142] = 0.0;
  rtb_H_k[143] = 0.0;
  rtb_H_k[144] = 0.0;
  rtb_H_k[145] = 0.0;
  rtb_H_k[146] = 0.0;
  rtb_H_k[147] = t3 * t4;
  rtb_H_k[148] = t10;
  rtb_H_k[149] = t18;
  rtb_H_k[150] = 0.0;
  rtb_H_k[151] = 0.0;
  rtb_H_k[152] = 0.0;
  rtb_H_k[153] = 0.0;
  rtb_H_k[154] = 0.0;
  rtb_H_k[155] = 0.0;
  rtb_H_k[156] = 0.0;
  rtb_H_k[157] = 0.0;
  rtb_H_k[158] = 0.0;
  rtb_H_k[159] = -t4 * t5;
  rtb_H_k[160] = t12;
  rtb_H_k[161] = t16;
  rtb_H_k[162] = 0.0;
  rtb_H_k[163] = 0.0;
  rtb_H_k[164] = 0.0;
  rtb_H_k[165] = 0.0;
  rtb_H_k[166] = 0.0;
  rtb_H_k[167] = 0.0;
  rtb_H_k[168] = 0.0;
  rtb_H_k[169] = 0.0;
  rtb_H_k[170] = 0.0;
  rtb_H_k[171] = t2;
  rtb_H_k[172] = -t4 * t6;
  rtb_H_k[173] = t4 * t7;
  rtb_H_k[174] = 0.0;
  rtb_H_k[175] = 0.0;
  rtb_H_k[176] = 0.0;
  rtb_H_k[177] = 0.0;
  rtb_H_k[178] = 0.0;
  rtb_H_k[179] = 0.0;
  rtb_H_k[180] = 0.0;
  rtb_H_k[181] = 0.0;
  rtb_H_k[182] = 0.0;
  rtb_H_k[183] = -1.0;
  rtb_H_k[184] = 0.0;
  rtb_H_k[185] = 0.0;
  rtb_H_k[186] = 0.0;
  rtb_H_k[187] = 0.0;
  rtb_H_k[188] = 0.0;
  rtb_H_k[189] = 0.0;
  rtb_H_k[190] = 0.0;
  rtb_H_k[191] = 0.0;
  rtb_H_k[192] = 0.0;
  rtb_H_k[193] = 0.0;
  rtb_H_k[194] = 0.0;
  rtb_H_k[195] = 0.0;
  rtb_H_k[196] = -1.0;
  rtb_H_k[197] = 0.0;
  rtb_H_k[198] = 0.0;
  rtb_H_k[199] = 0.0;
  rtb_H_k[200] = 0.0;
  rtb_H_k[201] = 0.0;
  rtb_H_k[202] = 0.0;
  rtb_H_k[203] = 0.0;
  rtb_H_k[204] = 0.0;
  rtb_H_k[205] = 0.0;
  rtb_H_k[206] = 0.0;
  rtb_H_k[207] = 0.0;
  rtb_H_k[208] = 0.0;
  rtb_H_k[209] = -1.0;
  rtb_H_k[210] = 0.0;
  rtb_H_k[211] = 0.0;
  rtb_H_k[212] = 0.0;
  rtb_H_k[213] = 0.0;
  rtb_H_k[214] = 0.0;
  rtb_H_k[215] = 0.0;
  rtb_H_k[216] = 0.0;
  rtb_H_k[217] = 0.0;
  rtb_H_k[218] = 0.0;
  rtb_H_k[219] = 0.0;
  rtb_H_k[220] = 0.0;
  rtb_H_k[221] = 0.0;
  rtb_H_k[222] = -1.0;
  rtb_H_k[223] = 0.0;
  rtb_H_k[224] = 0.0;
  rtb_H_k[225] = 0.0;
  rtb_H_k[226] = 0.0;
  rtb_H_k[227] = 0.0;
  rtb_H_k[228] = 0.0;
  rtb_H_k[229] = 0.0;
  rtb_H_k[230] = 0.0;
  rtb_H_k[231] = 0.0;
  rtb_H_k[232] = 0.0;
  rtb_H_k[233] = 0.0;
  rtb_H_k[234] = 0.0;
  rtb_H_k[235] = -1.0;
  rtb_H_k[236] = 0.0;
  rtb_H_k[237] = 0.0;
  rtb_H_k[238] = 0.0;
  rtb_H_k[239] = 0.0;
  rtb_H_k[240] = 0.0;
  rtb_H_k[241] = 0.0;
  rtb_H_k[242] = 0.0;
  rtb_H_k[243] = 0.0;
  rtb_H_k[244] = 0.0;
  rtb_H_k[245] = 0.0;
  rtb_H_k[246] = 0.0;
  rtb_H_k[247] = 0.0;
  rtb_H_k[248] = -1.0;
  rtb_H_k[249] = 0.0;
  rtb_H_k[250] = 0.0;
  rtb_H_k[251] = 0.0;

  // PermuteDimensions: '<S3>/Transpose'
  yElIdx = 0;
  uElOffset1 = 0;
  for (ntIdx1 = 0; ntIdx1 < 12; ntIdx1++) {
    uElOffset0 = uElOffset1;
    for (ntIdx0 = 0; ntIdx0 < 21; ntIdx0++) {
      rtb_Transpose[yElIdx] = rtb_H_k[uElOffset0];
      yElIdx++;
      uElOffset0 += 12;
    }

    uElOffset1++;
  }

  // End of PermuteDimensions: '<S3>/Transpose'

  // PermuteDimensions: '<S2>/Transpose'
  yElIdx = 0;
  uElOffset1 = 0;
  for (ntIdx1 = 0; ntIdx1 < 12; ntIdx1++) {
    uElOffset0 = uElOffset1;
    for (ntIdx0 = 0; ntIdx0 < 21; ntIdx0++) {
      rtb_Product_a[yElIdx] = rtb_H_k[uElOffset0];
      yElIdx++;
      uElOffset0 += 12;
    }

    uElOffset1++;
  }

  // End of PermuteDimensions: '<S2>/Transpose'

  // Product: '<S2>/Product'
  // Copy input to temporary space
  for (yElIdx = 0; yElIdx < 21; yElIdx++) {
    for (uElOffset1 = 0; uElOffset1 < 12; uElOffset1++) {
      rtb_Add_hg[yElIdx + 21 * uElOffset1] = 0.0;
      for (ntIdx1 = 0; ntIdx1 < 21; ntIdx1++) {
        rtb_Add_hg[yElIdx + 21 * uElOffset1] += rtb_Add_a[21 * ntIdx1 + yElIdx] *
          rtb_Product_a[21 * uElOffset1 + ntIdx1];
      }
    }
  }

  // S-Function (sdsppinv): '<S3>/Pseudoinverse' incorporates:
  //   Inport: '<Root>/R_k'
  //   Product: '<S2>/Product'
  //   Sum: '<S2>/Add'

  for (yElIdx = 0; yElIdx < 12; yElIdx++) {
    for (uElOffset1 = 0; uElOffset1 < 12; uElOffset1++) {
      t2 = 0.0;
      for (ntIdx1 = 0; ntIdx1 < 21; ntIdx1++) {
        t2 += rtb_H_k[12 * ntIdx1 + yElIdx] * rtb_Add_hg[21 * uElOffset1 +
          ntIdx1];
      }

      Pseudoinverse_X[yElIdx + 12 * uElOffset1] = quadrotor_EKF_U.R_k[12 *
        uElOffset1 + yElIdx] + t2;
    }
  }

  // Call SVD
  yElIdx = MWDSP_SVD_D(&Pseudoinverse_X[0U], 12, 12, &Pseudoinverse_S[0U],
                       &Pseudoinverse_E[0U], &Pseudoinverse_WORK[0U],
                       &Pseudoinverse_U[0U], &Pseudoinverse_V[0U], 1);

  // Only call 'pinv' function if SVD succeeded
  if (yElIdx == 0) {
    MWDSPCG_Pseudoinverse_D(&Pseudoinverse_S[0U], &Pseudoinverse_U[0U],
      &Pseudoinverse_V[0U], &rtb_Pseudoinverse[0U], 12, 12);
  }

  // End of S-Function (sdsppinv): '<S3>/Pseudoinverse'

  // Product: '<S3>/Product'
  for (yElIdx = 0; yElIdx < 21; yElIdx++) {
    for (uElOffset1 = 0; uElOffset1 < 12; uElOffset1++) {
      rtb_Add_hg[yElIdx + 21 * uElOffset1] = 0.0;
      for (ntIdx1 = 0; ntIdx1 < 12; ntIdx1++) {
        rtb_Add_hg[yElIdx + 21 * uElOffset1] += rtb_Transpose[21 * ntIdx1 +
          yElIdx] * rtb_Pseudoinverse[12 * uElOffset1 + ntIdx1];
      }
    }
  }

  for (yElIdx = 0; yElIdx < 21; yElIdx++) {
    for (uElOffset1 = 0; uElOffset1 < 12; uElOffset1++) {
      rtb_Product_a[yElIdx + 21 * uElOffset1] = 0.0;
      for (ntIdx1 = 0; ntIdx1 < 21; ntIdx1++) {
        rtb_Product_a[yElIdx + 21 * uElOffset1] += rtb_Add_a[21 * ntIdx1 +
          yElIdx] * rtb_Add_hg[21 * uElOffset1 + ntIdx1];
      }
    }
  }

  // End of Product: '<S3>/Product'

  // MATLAB Function: '<S8>/nonlinear output model' incorporates:
  //   Inport: '<Root>/B_earth'

  // MATLAB Function 'innovation//measurement residual/nonlinear output model/nonlinear output model': '<S9>:1' 
  // H
  //     H = H(X1,X2,X3,PHI,THETA,PSI,DX1,DX2,DX3,DPHI,DTHETA,DPSI,DDX1,DDX2,DDX3,B_A1,B_A2,B_A3,B_G1,B_G2,B_G3,B_EARTH1,B_EARTH2,B_EARTH3) 
  //     This function was generated by the Symbolic Math Toolbox version 6.3.
  //     02-Oct-2016 21:09:48
  // '<S9>:1:8'
  t2 = cos(rtb_f[4]);

  // '<S9>:1:9'
  t3 = sin(rtb_f[5]);

  // '<S9>:1:10'
  t4 = cos(rtb_f[5]);

  // '<S9>:1:11'
  t5 = sin(rtb_f[4]);

  // '<S9>:1:12'
  t6 = cos(rtb_f[3]);

  // '<S9>:1:13'
  t7 = sin(rtb_f[3]);

  // '<S9>:1:14'
  // '<S9>:1:15'
  // '<S9>:1:16'
  t10 = t4 * t5 * t7 + t3 * t6;

  // '<S9>:1:17'
  // '<S9>:1:18'
  t12 = t4 * t6 - t3 * t5 * t7;

  // '<S9>:1:19'
  // '<S9>:1:20'
  t14 = t3 * t7 - t4 * t5 * t6;

  // '<S9>:1:21'
  // '<S9>:1:22'
  // '<S9>:1:23'
  t17 = t3 * t5 * t6 + t4 * t7;

  // '<S9>:1:24'
  rtb_f_0[0] = rtb_f[0];
  rtb_f_0[1] = rtb_f[1];
  rtb_f_0[2] = rtb_f[2];
  rtb_f_0[3] = ((rtb_f[14] * t5 + -rtb_f[15]) + rtb_f[12] * t2 * t4) - rtb_f[13]
    * t2 * t3;
  rtb_f_0[4] = ((rtb_f[12] * t10 + -rtb_f[16]) + rtb_f[13] * t12) - rtb_f[14] *
    t2 * t7;
  rtb_f_0[5] = ((rtb_f[12] * t14 + -rtb_f[17]) + rtb_f[13] * t17) + rtb_f[14] *
    t2 * t6;
  rtb_f_0[6] = -rtb_f[18] + rtb_f[9];
  rtb_f_0[7] = -rtb_f[19] + rtb_f[10];
  rtb_f_0[8] = -rtb_f[20] + rtb_f[11];
  rtb_f_0[9] = (quadrotor_EKF_U.B_earth[0] * t2 * t4 + quadrotor_EKF_U.B_earth[2]
                * t5) - quadrotor_EKF_U.B_earth[1] * t2 * t3;
  rtb_f_0[10] = (quadrotor_EKF_U.B_earth[0] * t10 + quadrotor_EKF_U.B_earth[1] *
                 t12) - quadrotor_EKF_U.B_earth[2] * t2 * t7;
  rtb_f_0[11] = (quadrotor_EKF_U.B_earth[0] * t14 + quadrotor_EKF_U.B_earth[1] *
                 t17) + quadrotor_EKF_U.B_earth[2] * t2 * t6;

  // Sum: '<S1>/Add' incorporates:
  //   Inport: '<Root>/z_k'
  //   Product: '<S7>/Product'

  for (yElIdx = 0; yElIdx < 12; yElIdx++) {
    tmp_0[yElIdx] = quadrotor_EKF_U.z_k[yElIdx] - rtb_f_0[yElIdx];
  }

  // End of Sum: '<S1>/Add'
  for (uElOffset1 = 0; uElOffset1 < 21; uElOffset1++) {
    // Sum: '<S7>/Add' incorporates:
    //   Product: '<S7>/Product'

    t2 = 0.0;
    for (yElIdx = 0; yElIdx < 12; yElIdx++) {
      t2 += rtb_Product_a[21 * yElIdx + uElOffset1] * tmp_0[yElIdx];
    }

    rtb_Add[uElOffset1] = rtb_f[uElOffset1] + t2;

    // End of Sum: '<S7>/Add'

    // Update for Delay: '<Root>/Resettable Delay1'
    quadrotor_EKF_DW.x[uElOffset1] = rtb_Add[uElOffset1];
  }

  // Outport: '<Root>/x'
  quadrotor_EKF_Y.x[0] = rtb_Add[0];

  // Outport: '<Root>/a'
  quadrotor_EKF_Y.a[0] = rtb_Add[3];

  // Outport: '<Root>/dx'
  quadrotor_EKF_Y.dx[0] = rtb_Add[6];

  // Outport: '<Root>/da'
  quadrotor_EKF_Y.da[0] = rtb_Add[9];

  // Outport: '<Root>/ddx'
  quadrotor_EKF_Y.ddx[0] = rtb_Add[12];

  // Outport: '<Root>/bias_acc'
  quadrotor_EKF_Y.bias_acc[0] = rtb_Add[15];

  // Outport: '<Root>/bias_gyro'
  quadrotor_EKF_Y.bias_gyro[0] = rtb_Add[18];

  // Outport: '<Root>/x'
  quadrotor_EKF_Y.x[1] = rtb_Add[1];

  // Outport: '<Root>/a'
  quadrotor_EKF_Y.a[1] = rtb_Add[4];

  // Outport: '<Root>/dx'
  quadrotor_EKF_Y.dx[1] = rtb_Add[7];

  // Outport: '<Root>/da'
  quadrotor_EKF_Y.da[1] = rtb_Add[10];

  // Outport: '<Root>/ddx'
  quadrotor_EKF_Y.ddx[1] = rtb_Add[13];

  // Outport: '<Root>/bias_acc'
  quadrotor_EKF_Y.bias_acc[1] = rtb_Add[16];

  // Outport: '<Root>/bias_gyro'
  quadrotor_EKF_Y.bias_gyro[1] = rtb_Add[19];

  // Outport: '<Root>/x'
  quadrotor_EKF_Y.x[2] = rtb_Add[2];

  // Outport: '<Root>/a'
  quadrotor_EKF_Y.a[2] = rtb_Add[5];

  // Outport: '<Root>/dx'
  quadrotor_EKF_Y.dx[2] = rtb_Add[8];

  // Outport: '<Root>/da'
  quadrotor_EKF_Y.da[2] = rtb_Add[11];

  // Outport: '<Root>/ddx'
  quadrotor_EKF_Y.ddx[2] = rtb_Add[14];

  // Outport: '<Root>/bias_acc'
  quadrotor_EKF_Y.bias_acc[2] = rtb_Add[17];

  // Outport: '<Root>/bias_gyro'
  quadrotor_EKF_Y.bias_gyro[2] = rtb_Add[20];

  // Update for Delay: '<Root>/Resettable Delay1'
  quadrotor_EKF_DW.icLoad = 0U;

  // Update for Delay: '<Root>/Resettable Delay'
  quadrotor_EKF_DW.icLoad_i = 0U;

  // Product: '<S6>/Product'
  for (yElIdx = 0; yElIdx < 12; yElIdx++) {
    for (uElOffset1 = 0; uElOffset1 < 21; uElOffset1++) {
      rtb_Transpose[yElIdx + 12 * uElOffset1] = 0.0;
      for (ntIdx1 = 0; ntIdx1 < 21; ntIdx1++) {
        rtb_Transpose[yElIdx + 12 * uElOffset1] += rtb_H_k[12 * ntIdx1 + yElIdx]
          * rtb_Add_a[21 * uElOffset1 + ntIdx1];
      }
    }
  }

  // Update for Delay: '<Root>/Resettable Delay' incorporates:
  //   Product: '<S6>/Product'
  //   Sum: '<S6>/Add'

  for (yElIdx = 0; yElIdx < 21; yElIdx++) {
    for (uElOffset1 = 0; uElOffset1 < 21; uElOffset1++) {
      t2 = 0.0;
      for (ntIdx1 = 0; ntIdx1 < 12; ntIdx1++) {
        t2 += rtb_Product_a[21 * ntIdx1 + yElIdx] * rtb_Transpose[12 *
          uElOffset1 + ntIdx1];
      }

      quadrotor_EKF_DW.ResettableDelay_DSTATE[yElIdx + 21 * uElOffset1] =
        rtb_Add_a[21 * uElOffset1 + yElIdx] - t2;
    }
  }
}

// Model initialize function
void quadrotor_EKFModelClass::initialize()
{
  // Registration code

  // initialize non-finites
  rt_InitInfAndNaN(sizeof(real_T));

  // initialize error status
  rtmSetErrorStatus((&quadrotor_EKF_M), (NULL));

  // states (dwork)
  (void) memset((void *)&quadrotor_EKF_DW, 0,
                sizeof(DW_quadrotor_EKF_T));

  // external inputs
  (void) memset((void *)&quadrotor_EKF_U, 0,
                sizeof(ExtU_quadrotor_EKF_T));

  // external outputs
  (void) memset((void *)&quadrotor_EKF_Y, 0,
                sizeof(ExtY_quadrotor_EKF_T));
  quadrotor_EKF_PrevZCX.ResettableDelay1_Reset_ZCE = UNINITIALIZED_ZCSIG;
  quadrotor_EKF_PrevZCX.ResettableDelay_Reset_ZCE = UNINITIALIZED_ZCSIG;

  // InitializeConditions for Delay: '<Root>/Resettable Delay1'
  quadrotor_EKF_DW.icLoad = 1U;

  // InitializeConditions for Delay: '<Root>/Resettable Delay'
  quadrotor_EKF_DW.icLoad_i = 1U;
}

// Model terminate function
void quadrotor_EKFModelClass::terminate()
{
  // (no terminate code required)
}

// Constructor
quadrotor_EKFModelClass::quadrotor_EKFModelClass()
{
}

// Destructor
quadrotor_EKFModelClass::~quadrotor_EKFModelClass()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
RT_MODEL_quadrotor_EKF_T * quadrotor_EKFModelClass::getRTM()
{
  return (&quadrotor_EKF_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
