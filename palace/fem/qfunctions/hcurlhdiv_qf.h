// Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0

#ifndef PALACE_LIBCEED_HCURLHDIV_QF_H
#define PALACE_LIBCEED_HCURLHDIV_QF_H

#include "utils_qf.h"
#include "vecfemass_qf.h"

// libCEED QFunction for building quadrature data for a mixed H(curl)-H(div) mass operator
// with a scalar constant coefficient.
CEED_QFUNCTION(f_build_hcurlhdiv_const_scalar)(void *ctx, CeedInt Q,
                                               const CeedScalar *const *in,
                                               CeedScalar *const *out)
{
  // At every quadrature point, compute qw / det(J) adj(J) C J and store the
  // result.
  // in[0] is Jacobians with shape [dim, ncomp=space_dim, Q]
  // in[1] is quadrature weights, size (Q)
  VectorFEMassContext *bc = (VectorFEMassContext *)ctx;
  const CeedScalar coeff = bc->coeff;
  const CeedScalar *J = in[0], *qw = in[1];
  CeedScalar *qd = out[0];
  switch (10 * bc->space_dim + bc->dim)
  {
    case 11:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        qd[i] = qw[i] * coeff;
      }
      break;
    case 21:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt21(J + i, Q, &coeff, 1, 1, qw[i], Q, qd + i);
      }
      break;
    case 22:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt22(J + i, Q, &coeff, 1, 1, qw[i], Q, qd + i);
      }
      break;
    case 32:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt32(J + i, Q, &coeff, 1, 1, qw[i], Q, qd + i);
      }
      break;
    case 33:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt33(J + i, Q, &coeff, 1, 1, qw[i], Q, qd + i);
      }
      break;
  }
  return 0;
}

// libCEED QFunction for building quadrature data for a mixed H(curl)-H(div) mass operator
// with a scalar coefficient evaluated at quadrature points.
CEED_QFUNCTION(f_build_hcurlhdiv_quad_scalar)(void *ctx, CeedInt Q,
                                              const CeedScalar *const *in,
                                              CeedScalar *const *out)
{
  // At every quadrature point, compute qw / det(J) adj(J) C J and store the
  // result.
  // in[0] is coefficients with shape [ncomp=1, Q]
  // in[1] is Jacobians with shape [dim, ncomp=space_dim, Q]
  // in[2] is quadrature weights, size (Q)
  VectorFEMassContext *bc = (VectorFEMassContext *)ctx;
  const CeedScalar *c = in[0], *J = in[1], *qw = in[2];
  CeedScalar *qd = out[0];
  switch (10 * bc->space_dim + bc->dim)
  {
    case 11:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        qd[i] = qw[i] * c[i];
      }
      break;
    case 21:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt21(J + i, Q, c + i, Q, 1, qw[i], Q, qd + i);
      }
      break;
    case 22:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt22(J + i, Q, c + i, Q, 1, qw[i], Q, qd + i);
      }
      break;
    case 32:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt32(J + i, Q, c + i, Q, 1, qw[i], Q, qd + i);
      }
      break;
    case 33:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt33(J + i, Q, c + i, Q, 1, qw[i], Q, qd + i);
      }
      break;
  }
  return 0;
}

// libCEED QFunction for building quadrature data for a mixed H(curl)-H(div) mass operator
// with a vector coefficient evaluated at quadrature points.
CEED_QFUNCTION(f_build_hcurlhdiv_quad_vector)(void *ctx, CeedInt Q,
                                              const CeedScalar *const *in,
                                              CeedScalar *const *out)
{
  // At every quadrature point, compute qw / det(J) adj(J) C J and store the
  // result.
  // in[0] is coefficients with shape [ncomp=space_dim, Q]
  // in[1] is Jacobians with shape [dim, ncomp=space_dim, Q]
  // in[2] is quadrature weights, size (Q)
  VectorFEMassContext *bc = (VectorFEMassContext *)ctx;
  const CeedScalar *c = in[0], *J = in[1], *qw = in[2];
  CeedScalar *qd = out[0];
  switch (10 * bc->space_dim + bc->dim)
  {
    case 21:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt21(J + i, Q, c + i, Q, 2, qw[i], Q, qd + i);
      }
      break;
    case 22:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt22(J + i, Q, c + i, Q, 2, qw[i], Q, qd + i);
      }
      break;
    case 32:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt32(J + i, Q, c + i, Q, 3, qw[i], Q, qd + i);
      }
      break;
    case 33:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt33(J + i, Q, c + i, Q, 3, qw[i], Q, qd + i);
      }
      break;
  }
  return 0;
}

// libCEED QFunction for building quadrature data for a mixed H(curl)-H(div) mass operator
// with a matrix coefficient evaluated at quadrature points.
CEED_QFUNCTION(f_build_hcurlhdiv_quad_matrix)(void *ctx, CeedInt Q,
                                              const CeedScalar *const *in,
                                              CeedScalar *const *out)
{
  // At every quadrature point, compute qw / det(J) adj(J) C J and store the
  // result.
  // in[0] is coefficients with shape [ncomp=space_dim*(space_dim+1)/2, Q]
  // in[1] is Jacobians with shape [dim, ncomp=space_dim, Q]
  // in[2] is quadrature weights, size (Q)
  VectorFEMassContext *bc = (VectorFEMassContext *)ctx;
  const CeedScalar *c = in[0], *J = in[1], *qw = in[2];
  CeedScalar *qd = out[0];
  switch (10 * bc->space_dim + bc->dim)
  {
    case 21:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt21(J + i, Q, c + i, Q, 3, qw[i], Q, qd + i);
      }
      break;
    case 22:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt22(J + i, Q, c + i, Q, 3, qw[i], Q, qd + i);
      }
      break;
    case 32:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt32(J + i, Q, c + i, Q, 6, qw[i], Q, qd + i);
      }
      break;
    case 33:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt33(J + i, Q, c + i, Q, 6, qw[i], Q, qd + i);
      }
      break;
  }
  return 0;
}

// libCEED QFunction for building quadrature data for a mixed H(div)-H(curl) mass operator
// with a scalar constant coefficient.
CEED_QFUNCTION(f_build_hdivhcurl_const_scalar)(void *ctx, CeedInt Q,
                                               const CeedScalar *const *in,
                                               CeedScalar *const *out)
{
  // At every quadrature point, compute qw / det(J) adj(J) C J and store the
  // result.
  // in[0] is Jacobians with shape [dim, ncomp=space_dim, Q]
  // in[1] is quadrature weights, size (Q)
  VectorFEMassContext *bc = (VectorFEMassContext *)ctx;
  const CeedScalar coeff = bc->coeff;
  const CeedScalar *J = in[0], *qw = in[1];
  CeedScalar *qd = out[0];
  switch (10 * bc->space_dim + bc->dim)
  {
    case 11:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        qd[i] = qw[i] * coeff;
      }
      break;
    case 21:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt21<true>(J + i, Q, &coeff, 1, 1, qw[i], Q, qd + i);
      }
      break;
    case 22:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt22<true>(J + i, Q, &coeff, 1, 1, qw[i], Q, qd + i);
      }
      break;
    case 32:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt32<true>(J + i, Q, &coeff, 1, 1, qw[i], Q, qd + i);
      }
      break;
    case 33:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt33<true>(J + i, Q, &coeff, 1, 1, qw[i], Q, qd + i);
      }
      break;
  }
  return 0;
}

// libCEED QFunction for building quadrature data for a mixed H(div)-H(curl) mass operator
// with a scalar coefficient evaluated at quadrature points.
CEED_QFUNCTION(f_build_hdivhcurl_quad_scalar)(void *ctx, CeedInt Q,
                                              const CeedScalar *const *in,
                                              CeedScalar *const *out)
{
  // At every quadrature point, compute qw / det(J) adj(J) C J and store the
  // result.
  // in[0] is coefficients with shape [ncomp=1, Q]
  // in[1] is Jacobians with shape [dim, ncomp=space_dim, Q]
  // in[2] is quadrature weights, size (Q)
  VectorFEMassContext *bc = (VectorFEMassContext *)ctx;
  const CeedScalar *c = in[0], *J = in[1], *qw = in[2];
  CeedScalar *qd = out[0];
  switch (10 * bc->space_dim + bc->dim)
  {
    case 11:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        qd[i] = qw[i] * c[i];
      }
      break;
    case 21:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt21<true>(J + i, Q, c + i, Q, 1, qw[i], Q, qd + i);
      }
      break;
    case 22:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt22<true>(J + i, Q, c + i, Q, 1, qw[i], Q, qd + i);
      }
      break;
    case 32:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt32<true>(J + i, Q, c + i, Q, 1, qw[i], Q, qd + i);
      }
      break;
    case 33:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt33<true>(J + i, Q, c + i, Q, 1, qw[i], Q, qd + i);
      }
      break;
  }
  return 0;
}

// libCEED QFunction for building quadrature data for a mixed H(div)-H(curl) mass operator
// with a vector coefficient evaluated at quadrature points.
CEED_QFUNCTION(f_build_hdivhcurl_quad_vector)(void *ctx, CeedInt Q,
                                              const CeedScalar *const *in,
                                              CeedScalar *const *out)
{
  // At every quadrature point, compute qw / det(J) adj(J) C J and store the
  // result.
  // in[0] is coefficients with shape [ncomp=space_dim, Q]
  // in[1] is Jacobians with shape [dim, ncomp=space_dim, Q]
  // in[2] is quadrature weights, size (Q)
  VectorFEMassContext *bc = (VectorFEMassContext *)ctx;
  const CeedScalar *c = in[0], *J = in[1], *qw = in[2];
  CeedScalar *qd = out[0];
  switch (10 * bc->space_dim + bc->dim)
  {
    case 21:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt21<true>(J + i, Q, c + i, Q, 2, qw[i], Q, qd + i);
      }
      break;
    case 22:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt22<true>(J + i, Q, c + i, Q, 2, qw[i], Q, qd + i);
      }
      break;
    case 32:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt32<true>(J + i, Q, c + i, Q, 3, qw[i], Q, qd + i);
      }
      break;
    case 33:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt33<true>(J + i, Q, c + i, Q, 3, qw[i], Q, qd + i);
      }
      break;
  }
  return 0;
}

// libCEED QFunction for building quadrature data for a mixed H(div)-H(curl) mass operator
// with a matrix coefficient evaluated at quadrature points.
CEED_QFUNCTION(f_build_hdivhcurl_quad_matrix)(void *ctx, CeedInt Q,
                                              const CeedScalar *const *in,
                                              CeedScalar *const *out)
{
  // At every quadrature point, compute qw / det(J) adj(J) C J and store the
  // result.
  // in[0] is coefficients with shape [ncomp=space_dim*(space_dim+1)/2, Q]
  // in[1] is Jacobians with shape [dim, ncomp=space_dim, Q]
  // in[2] is quadrature weights, size (Q)
  VectorFEMassContext *bc = (VectorFEMassContext *)ctx;
  const CeedScalar *c = in[0], *J = in[1], *qw = in[2];
  CeedScalar *qd = out[0];
  switch (10 * bc->space_dim + bc->dim)
  {
    case 21:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt21<true>(J + i, Q, c + i, Q, 3, qw[i], Q, qd + i);
      }
      break;
    case 22:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt22<true>(J + i, Q, c + i, Q, 3, qw[i], Q, qd + i);
      }
      break;
    case 32:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt32<true>(J + i, Q, c + i, Q, 6, qw[i], Q, qd + i);
      }
      break;
    case 33:
      CeedPragmaSIMD for (CeedInt i = 0; i < Q; i++)
      {
        MultJtCAdjJt33<true>(J + i, Q, c + i, Q, 6, qw[i], Q, qd + i);
      }
      break;
  }
  return 0;
}

#endif  // PALACE_LIBCEED_HCURLHDIV_QF_H