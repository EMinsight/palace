// Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0

#ifndef PALACE_LIBCEED_HDIV_MASS_BUILD_32_QF_H
#define PALACE_LIBCEED_HDIV_MASS_BUILD_32_QF_H

#include "../coeff/coeff_1_qf.h"
#include "../coeff/coeff_3_qf.h"
#include "utils_32_qf.h"

CEED_QFUNCTION(f_build_hdivmass_32)(void *__restrict__ ctx, CeedInt Q,
                                    const CeedScalar *const *in, CeedScalar *const *out)
{
  const CeedScalar *attr = in[0], *wdetJ = in[0] + Q, *adjJt = in[0] + 2 * Q, *qw = in[1];
  CeedScalar *__restrict__ qd1 = out[0], *__restrict__ qd2 = out[0] + 3 * Q;

  CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
  {
    {
      CeedScalar coeff[9], adjJt_loc[6], qd_loc[4];
      CoeffUnpack3((const CeedIntScalar *)ctx, (CeedInt)attr[i], coeff);
      MatUnpack32(adjJt + i, Q, adjJt_loc);
      MultAtBA32(adjJt_loc, coeff, qd_loc);

      qd1[i + Q * 0] = wdetJ[i] * qd_loc[0];
      qd1[i + Q * 1] = wdetJ[i] * qd_loc[1];
      qd1[i + Q * 2] = wdetJ[i] * qd_loc[2];
      qd1[i + Q * 3] = wdetJ[i] * qd_loc[3];
    }
    {
      const CeedScalar coeff =
          CoeffUnpack1(CoeffPairSecond<3>((const CeedIntScalar *)ctx), (CeedInt)attr[i]);

      qd2[i] = coeff * qw[i] * qw[i] / wdetJ[i];
    }
  }
  return 0;
}

#endif  // PALACE_LIBCEED_HDIV_MASS_BUILD_32_QF_H
