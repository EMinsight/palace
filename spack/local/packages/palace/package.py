# Copyright 2013-2024 Lawrence Livermore National Security, LLC and other
# Spack Project Developers. See the top-level COPYRIGHT file for details.
#
# SPDX-License-Identifier: (Apache-2.0 OR MIT)

from spack.package import *

class Palace(CMakePackage, CudaPackage, ROCmPackage):
    """3D finite element solver for computational electromagnetics"""

    tags = ["cem", "fem", "finite-elements", "hpc", "solver"]

    homepage = "https://github.com/awslabs/palace"
    git = "https://github.com/awslabs/palace.git"

    maintainers("hughcars", "simlap", "cameronrutherford")

    version("develop", branch="main")
    version("0.13.0", tag="v0.13.0", commit="a61c8cbe0cacf496cde3c62e93085fae0d6299ac")
    version("0.12.0", tag="v0.12.0", commit="8c192071206466638d5818048ee712e1fada386f")
    version("0.11.2", tag="v0.11.2", commit="6c3aa5f84a934a6ddd58022b2945a1bdb5fa329d")

    # Note: 'cuda' and 'cuda_arch' variants are added by the CudaPackage
    # Note: 'rocm' and 'amdgpu_target' variants are added by the ROCmPackage
    variant("shared", default=True, description="Build shared libraries")
    variant("int64", default=False, description="Use 64 bit integers")
    variant(
        "openmp", default=False, description="Use OpenMP for shared-memory parallelism"
    )
    variant(
        "superlu-dist",
        default=True,
        description="Build with SuperLU_DIST sparse direct solver",
    )
    variant(
        "strumpack",
        default=False,
        description="Build with STRUMPACK sparse direct solver",
    )
    variant(
        "sundials",
        default=True,
        description="Build with SUNDIALS differential/algebraic equations solver",
        when="@0.14:"
    )
    variant("mumps", default=False, description="Build with MUMPS sparse direct solver")
    variant("slepc", default=True, description="Build with SLEPc eigenvalue solver")
    variant("arpack", default=False, description="Build with ARPACK eigenvalue solver")
    variant(
        "libxsmm", default=True, description="Build with libxsmm backend for libCEED"
    )
    variant(
        "gslib",
        default=True,
        description="Build with GSLIB library for high-order field interpolation",
    )

    ## -- Patches for Spack Build System --
    # Fix API mismatch between libxsmm@main and internal libceed build
    patch("palace-0.12.0.patch", when="@0.12")

    ## -- Core Dependencies --
    # NOTE: We can't depend on git tagged versions here
    #       https://github.com/spack/spack/issues/50171
    #       Instead, version in environment / spec
    depends_on("c", type="build")
    depends_on("cxx", type="build")
    depends_on("cmake@3.21:", type="build")
    depends_on("pkgconfig", type="build")
    depends_on("mpi")
    depends_on("zlib-api")
    depends_on("nlohmann-json")
    depends_on("fmt+shared", when="+shared")
    depends_on("fmt~shared", when="~shared")
    depends_on("eigen")

    ## -- Sparse Direct Solvers --
    conflicts(
        "~superlu-dist~strumpack~mumps",
        msg="Need at least one sparse direct solver",
    )

    ## -- MUMPS --
    conflicts("^mumps+int64", msg="Palace requires MUMPS without 64 bit integers")
    with when("+mumps"):
        depends_on("mumps+metis+parmetis")
        depends_on("mumps+shared", when="+shared")
        depends_on("mumps~shared", when="~shared")
        depends_on("mumps+openmp", when="+openmp")
        depends_on("mumps~openmp", when="~openmp")

    ## -- SuperLU-Dist --
    with when("+superlu-dist"):
        depends_on("superlu-dist+parmetis")
        depends_on("superlu-dist+shared", when="+shared")
        depends_on("superlu-dist~shared", when="~shared")
        depends_on("superlu-dist+int64", when="+int64")
        depends_on("superlu-dist~int64", when="~int64")
        depends_on("superlu-dist+openmp", when="+openmp")
        depends_on("superlu-dist~openmp", when="~openmp")

    ## -- Strumpack --
    with when("+strumpack"):
        depends_on("strumpack+butterflypack+zfp+parmetis")
        depends_on("strumpack+shared", when="+shared")
        depends_on("strumpack~shared", when="~shared")
        depends_on("strumpack+openmp", when="+openmp")
        depends_on("strumpack~openmp", when="~openmp")

    ## -- Eigenvalue Solvers --
    conflicts("~arpack~slepc", msg="At least one eigenvalue solver is required")

    ## -- SLEPc --
    with when("+slepc"):
        depends_on("slepc~arpack")
        depends_on("petsc+mpi+double+complex")
        depends_on("petsc+shared", when="+shared")
        depends_on("petsc~shared", when="~shared")
        depends_on("petsc+int64", when="+int64")
        depends_on("petsc~int64", when="~int64")
        depends_on("petsc+openmp", when="+openmp")
        depends_on("petsc~openmp", when="~openmp")

    ## -- Arpack --
    with when("+arpack"):
        depends_on("arpack-ng+mpi+icb@develop")
        depends_on("arpack-ng+shared", when="+shared")
        depends_on("arpack-ng~shared", when="~shared")

    ## -- gslib --
    with when("+gslib @0.14:"):
        depends_on("gslib+mpi")
        depends_on("gslib+shared", when="+shared")
        depends_on("gslib~shared", when="~shared")

    ## -- METIS --
    depends_on("metis@5:")
    depends_on("metis+shared", when="+shared")
    depends_on("metis~shared", when="~shared")
    depends_on("metis+int64", when="+int64")
    depends_on("metis~int64", when="~int64")

    ## -- HYPRE --
    conflicts(
        "^hypre+int64", msg="Palace uses HYPRE's mixedint option for 64 bit integers"
    )
    depends_on("hypre~complex")
    depends_on("hypre+shared", when="+shared")
    depends_on("hypre~shared", when="~shared")
    depends_on("hypre+mixedint", when="+int64")
    depends_on("hypre~mixedint", when="~int64")
    depends_on("hypre+openmp", when="+openmp")
    depends_on("hypre~openmp", when="~openmp")
    depends_on("hypre~cuda", when="~cuda")
    depends_on("hypre~rocm", when="~rocm")

    ## -- libxsmm --
    with when("+libxsmm"):
        # NOTE: @=main != @main since libxsmm has a version main-2023-22
        depends_on("libxsmm@=main blas=0")
        depends_on("libxsmm+debug", when="build_type=Debug")
        depends_on("libceed+libxsmm", when="@0.14:")
        # NOTE: libxsmm builds on MacOS have linker issues
        # https://github.com/libxsmm/libxsmm/issues/883
        depends_on("libxsmm+shared")

    ## -- libCEED --
    depends_on("libceed@0.13:", when="@0.14:")

    ## -- Sundials --
    with when("+sundials @0.14:"):
        depends_on("sundials")
        depends_on("sundials+shared", when="+shared")
        depends_on("sundials~shared", when="~shared")
        depends_on("sundials+openmp", when="+openmp")
        depends_on("sundials~openmp", when="~openmp")

    ## -- arpack --
    with when("+arpack"):
        depends_on("arpack-ng+mpi+icb")
        depends_on("arpack-ng+shared", when="+shared")
        depends_on("arpack-ng~shared", when="~shared")

    ## -- GPU --
    conflicts('+cuda', when='@:0.12',
            msg='CUDA is only supported for Palace versions 0.13 and above')
    conflicts('+rocm', when='@:0.12',
            msg='ROCm is only supported for Palace versions 0.13 and above')
    conflicts(
        "+cuda+rocm", msg="PALACE_WITH_CUDA is not compatible with PALACE_WITH_HIP"
    )
    conflicts(
        "cuda_arch=none",
        when="+cuda",
        msg="palace: Please specify a CUDA arch value / values",
    )
    conflicts(
        "amdgpu_target=none",
        when="+rocm",
        msg="palace: Please specify an AMD GPU target / targets",
    )

    ## -- Magma --
    with when("+cuda") or when("+rocm"):
        depends_on("magma")
        depends_on("magma+shared", when="+shared")
        depends_on("magma~shared", when="~shared")
        depends_on("libceed+magma", when="@0.14:")

    # -- CUDA --
    with when("+cuda"):
        for arch in CudaPackage.cuda_arch_values:
            cuda_variant = f"+cuda cuda_arch={arch}"
            depends_on(f"hypre{cuda_variant}", when=f"{cuda_variant}")
            depends_on(f"magma{cuda_variant}", when=f"{cuda_variant}")
            depends_on(
                f"superlu-dist{cuda_variant}", when=f"+superlu-dist{cuda_variant}"
            )
            depends_on(f"libceed{cuda_variant}", when=f"{cuda_variant} @0.14:")
            depends_on(f"strumpack{cuda_variant}", when=f"+strumpack{cuda_variant}")
            depends_on(f"sundials{cuda_variant}", when=f"+sundials{cuda_variant} @0.14:")
            depends_on(f"slepc{cuda_variant}", when=f"+slepc{cuda_variant}")
            depends_on(f"petsc{cuda_variant}", when=f"+slepc{cuda_variant}")

    # -- ROCm --
    with when("+rocm"):
        for arch in ROCmPackage.amdgpu_targets:
            rocm_variant = f"+rocm amdgpu_target={arch}"
            depends_on(f"hypre{rocm_variant}", when=f"{rocm_variant}")
            depends_on(f"magma{rocm_variant}", when=f"{rocm_variant}")
            depends_on(
                f"superlu-dist{rocm_variant}", when=f"+superlu-dist{rocm_variant}"
            )
            depends_on(f"libceed{rocm_variant}", when=f"{rocm_variant} @0.14:")
            depends_on(f"strumpack{rocm_variant}", when=f"+strumpack{rocm_variant}")
            depends_on(f"sundials{rocm_variant}", when=f"+sundials{rocm_variant} @0.14:")
            depends_on(f"slepc{rocm_variant}", when=f"+slepc{rocm_variant}")
            depends_on(f"petsc{rocm_variant}", when=f"+slepc{rocm_variant}")

    def cmake_args(self):
        args = [
            self.define_from_variant("BUILD_SHARED_LIBS", "shared"),
            self.define_from_variant("PALACE_WITH_64BIT_INT", "int64"),
            self.define_from_variant("PALACE_WITH_ARPACK", "arpack"),
            self.define_from_variant("PALACE_WITH_CUDA", "cuda"),
            self.define_from_variant("PALACE_WITH_GSLIB", "gslib"),
            self.define_from_variant("PALACE_WITH_HIP", "rocm"),
            self.define_from_variant("PALACE_WITH_LIBXSMM", "libxsmm"),
            self.define_from_variant("PALACE_WITH_MUMPS", "mumps"),
            self.define_from_variant("PALACE_WITH_OPENMP", "openmp"),
            self.define_from_variant("PALACE_WITH_SLEPC", "slepc"),
            self.define_from_variant("PALACE_WITH_STRUMPACK", "strumpack"),
            self.define_from_variant("PALACE_WITH_SUNDIALS", "sundials"),
            self.define_from_variant("PALACE_WITH_SUPERLU", "superlu-dist"),
            self.define("PALACE_BUILD_EXTERNAL_DEPS", False)
        ]

        # We guarantee that there are arch specs with conflicts above
        if "+cuda" in self.spec:
            args.append(
                self.define(
                    "CMAKE_CUDA_ARCHITECTURES", ";".join(self.spec.variants["cuda_arch"].value)
                )
            )
        if "+rocm" in self.spec:
            args.append(
                self.define(
                    "CMAKE_HIP_ARCHITECTURES", ";".join(self.spec.variants["amdgpu_target"].value)
                )
            )

        # HYPRE is always built with external BLAS/LAPACK
        args.extend(
            [
                self.define("HYPRE_REQUIRED_PACKAGES", "LAPACK;BLAS"),
                self.define("BLAS_LIBRARIES", self.spec["blas"].libs.joined(";")),
                self.define("LAPACK_LIBRARIES", self.spec["lapack"].libs.joined(";")),
            ]
        )

        # MPI compiler wrappers are not required, but MFEM test builds need to know to link
        # against MPI libraries.
        if "+superlu-dist" in self.spec:
            args.append(self.define("SuperLUDist_REQUIRED_PACKAGES", "LAPACK;BLAS;MPI"))
        if "+sundials" in self.spec:
            args.append(self.define("SUNDIALS_REQUIRED_PACKAGES", "LAPACK;BLAS;MPI"))
        if "+strumpack" in self.spec:
            args.append(
                self.define(
                    "STRUMPACK_REQUIRED_PACKAGES", "LAPACK;BLAS;MPI;MPI_Fortran"
                )
            )
        if "+mumps" in self.spec:
            args.append(
                self.define("MUMPS_REQUIRED_PACKAGES", "LAPACK;BLAS;MPI;MPI_Fortran")
            )

        if self.spec.satisfies('@:0.13'):
            # In v0.13 and prior libCEED and gslib were internally built and required the libxsmm and
            # magma build information be passed in.
            if "+libxsmm" in self.spec:
                args.append(self.define("LIBXSMM_DIR", self.spec["libxsmm"].prefix))  # type: ignore
            if "+cuda" in self.spec or "+rocm" in self.spec:
                args.append(self.define("MAGMA_DIR", self.spec["magma"].prefix))  # type: ignore
        else:
            # After v 0.13 gslib and libceed is built externally and the directories passed explicitly.
            args.append(self.define("LIBCEED_DIR", self.spec["libceed"].prefix))
            if "+gslib" in self.spec:
                args.append(self.define("GSLIB_DIR", self.spec["gslib"].prefix))  # type: ignore

        return args

    def install(self, spec, prefix):
        # No install phase for Palace (always performed during build)
        pass
