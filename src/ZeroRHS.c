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
 * ZeroRHS.c
 * Zeros the RHS variables to prevent spurious NaNs.
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
void ADMScalarWave_ZeroRHS(CCTK_ARGUMENTS);

/**********************************************
 * ADMScalarWave_ZeroRHS(CCTK_ARGUMENTS)      *
 *                                            *
 * This function zeros the RHS variables in   *
 * order to prevent sporious NaNs.            *
 *                                            *
 * Input: CCTK_ARGUMENTS (the grid functions  *
 * from interface.ccl                         *
 *                                            *
 * Output: Nothing                            *
 **********************************************/
void ADMScalarWave_ZeroRHS(CCTK_ARGUMENTS) {
  DECLARE_CCTK_ARGUMENTS;
  DECLARE_CCTK_PARAMETERS;

  /* Loop indexes */
  int i = 0;
  int j = 0;
  int k = 0;
  int ijk = 0;

  /* Loop over all points (ghostzones included) */
  for (k = 0; k < cctk_lsh[2]; k++) {
    for (j = 0; j < cctk_lsh[1]; j++) {
      for (i = 0; i < cctk_lsh[0]; i++) {
	ijk = CCTK_GFINDEX3D(cctkGH, i, j, k);
	
	Phi_rhs[ijk] = 0.0;
        K_Phi_rhs[ijk] = 0.0;
      }
    }
  }
}

