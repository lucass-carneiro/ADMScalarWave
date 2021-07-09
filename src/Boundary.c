/*
 *  ADMScalarWave - Thorn for scalar wave evolutions in arbitrary space-times
 *  Copyright (C) 2021  Lucas Timotheo Sanches
 *
 *  This file is part of ADMScalarWave.
 *
 *  ADMScalarWave is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ADMScalarWave is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Boundary.c
 * Implements outer boundary conditions for the ADM-decomposed scalar wave
 * equation.
 */

/*******************
 * Cactus includes *
 *******************/
#include "cctk.h"
#include "cctk_Arguments.h"
#include "cctk_Parameters.h"

/**************
 * Prototypes *
 **************/
void ADMScalarWave_OuterBoundary(CCTK_ARGUMENTS);
void ADMScalarWave_Boundary(CCTK_ARGUMENTS);

/***********************************************
 * ADMScalarWave_OuterBoundary(CCTK_ARGUMENTS) *
 *                                             *
 * This function updates the grid functions    *
 * at the outer boundary points with boundary  *
 * conditions.                                 *
 *                                             *
 * Users can choose radiating or reflecting    *
 * boundary conditions                         *
 *                                             *
 * Input: CCTK_ARGUMENTS (the grid functions   *
 * from interface.ccl                          *
 *                                             *
 * Output: Nothing                             *
 ***********************************************/
void ADMScalarWave_OuterBoundary(CCTK_ARGUMENTS) {
  DECLARE_CCTK_ARGUMENTS;
  DECLARE_CCTK_PARAMETERS;

  if (CCTK_EQUALS(bc_type, "new_rad")) {
    CCTK_INT ierr = 0;

    ierr += NewRad_Apply(cctkGH, Phi, Phi_rhs, Phi0, 1.0, nPhi);
    ierr += NewRad_Apply(cctkGH, K_Phi, K_Phi_rhs, K_Phi0, 1.0, nK_Phi);

    if (ierr < 0)
      CCTK_ERROR("Failed to register NewRad boundary conditions");
  } else if (CCTK_EQUALS(bc_type, "reflecting")) {
    const CCTK_INT gz = cctk_nghostzones[2];
    const CCTK_INT gy = cctk_nghostzones[1];
    const CCTK_INT gx = cctk_nghostzones[0];

    CCTK_INT i = 0, j = 0, k = 0, ijk = 0;

    /* Lower x Boundary */
    if (cctk_bbox[0]) {
      for (k = 0; k < cctk_lsh[2]; k++) {
        for (j = 0; j < cctk_lsh[1]; j++) {
          for (i = 0; i < gx; i++) {
            ijk = CCTK_GFINDEX3D(cctkGH, i, j, k);
            Phi_rhs[ijk] = K_Phi[ijk];
            K_Phi_rhs[ijk] = 0.0;
          }
        }
      }
    }

    /* Upper x Boundary */
    if (cctk_bbox[1]) {
      for (k = 0; k < cctk_lsh[2]; k++) {
        for (j = 0; j < cctk_lsh[1]; j++) {
          for (i = cctk_lsh[0] - gx; i < cctk_lsh[0]; i++) {
            ijk = CCTK_GFINDEX3D(cctkGH, i, j, k);
            Phi_rhs[ijk] = K_Phi[ijk];
            K_Phi_rhs[ijk] = 0.0;
          }
        }
      }
    }

    /* Lower y Boundary */
    if (cctk_bbox[2]) {
      for (k = 0; k < cctk_lsh[2]; k++) {
        for (j = 0; j < gy; j++) {
          for (i = 0; i < cctk_lsh[0]; i++) {
            ijk = CCTK_GFINDEX3D(cctkGH, i, j, k);
            Phi_rhs[ijk] = K_Phi[ijk];
            K_Phi_rhs[ijk] = 0.0;
          }
        }
      }
    }

    /* Upper y Boundary */
    if (cctk_bbox[3]) {
      for (k = 0; k < cctk_lsh[2]; k++) {
        for (j = cctk_lsh[1] - gy; j < cctk_lsh[1]; j++) {
          for (i = 0; i < cctk_lsh[0]; i++) {
            ijk = CCTK_GFINDEX3D(cctkGH, i, j, k);
            Phi_rhs[ijk] = K_Phi[ijk];
            K_Phi_rhs[ijk] = 0.0;
          }
        }
      }
    }

    /* Lower z Boundary */
    if (cctk_bbox[4]) {
      for (k = 0; k < gz; k++) {
        for (j = 0; j < cctk_lsh[1]; j++) {
          for (i = 0; i < cctk_lsh[0]; i++) {
            ijk = CCTK_GFINDEX3D(cctkGH, i, j, k);
            Phi_rhs[ijk] = K_Phi[ijk];
            K_Phi_rhs[ijk] = 0.0;
          }
        }
      }
    }

    /* Upper z Boundary */
    if (cctk_bbox[5]) {
      for (k = cctk_lsh[2] - gz; k < cctk_lsh[2]; k++) {
        for (j = 0; j < cctk_lsh[1]; j++) {
          for (i = 0; i < cctk_lsh[0]; i++) {
            ijk = CCTK_GFINDEX3D(cctkGH, i, j, k);
            Phi_rhs[ijk] = K_Phi[ijk];
            K_Phi_rhs[ijk] = 0.0;
          }
        }
      }
    }
  }
}

/***********************************************
 * ADMScalarWave_Boundary(void)                *
 *                                             *
 * This function is a no-op (it does nothing). *
 * After taking a time step Carpet might apply *
 * wrong boundary conditions in the refinament *
 * boundaries.                                 *
 *                                             *
 * Scheduling this function in MoL_PostStep,   *
 * postrestrict and postregrid solves this.    *
 *                                             *
 * Fuerthermore, we register all BCs as 'none' *
 * to enforces the possible symmetry BCs       *
 *                                             *
 * Input: CCTK_ARGUMENTS (the grid functions   *
 * from interface.ccl                          *
 *
 * Output: Nothing                             *
 ***********************************************/
void ADMScalarWave_Boundary(CCTK_ARGUMENTS) {
  DECLARE_CCTK_ARGUMENTS;
  DECLARE_CCTK_PARAMETERS;

  CCTK_INT ierr = 0;

  if (CCTK_IsFunctionAliased("Boundary_SelectGroupForBC")) {
    ierr += Boundary_SelectGroupForBC(cctkGH, CCTK_ALL_FACES, 1, -1,
                                      "ADMScalarWave::evolved_group", "none");
  } else {
    CCTK_WARN(CCTK_WARN_ABORT, "Boundary_SelectGroupForBC not aliased !");
    ++ierr;
  }

  if (ierr < 0)
    CCTK_WARN(0, "Failed to register BC for ADMScalarWave::rhs_group");
}
