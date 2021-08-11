/*
 *  KleinGordonX - Thorn for scalar wave evolutions in arbitrary space-times
 *  Copyright (C) 2021  Lucas Timotheo Sanches
 *
 *  This file is part of KleinGordonX.
 *
 *  KleinGordonX is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  KleinGordonX is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with KleinGordonX.  If not, see <https://www.gnu.org/licenses/>.
 *
 * CheckParameters.cpp
 * Check if the parameters parsed Cactus and their combinations are valid.
 */

#include "KleinGordonX.hpp"

namespace KleinGordonX {

extern "C" void KleinGordonX_CheckParameters(CCTK_ARGUMENTS) {
  DECLARE_CCTK_ARGUMENTS_KleinGordonX_CheckParameters;
  DECLARE_CCTK_PARAMETERS;

  if (gaussian_sigma * gaussian_sigma < 1.0e-3)
    CCTK_PARAMWARN("The gaussian parameter sigma is too small. Increase it in "
                   "order to avoid singularities.");

  // for(int d=0;d<3;++d) assert(cctk_nghostzones[d]>=2);
  // cassert

  switch (fd_order) {
  case 4: {
    CCTK_INFO("Using 4th order finite differencing. Make sure that you have at "
              "least 2 ghost zones");
    break;
  }

  case 6: {
    CCTK_INFO("Using 6th order finite differencing. Make sure that you have at "
              "least 3 ghost zones");
    break;
  }

  case 8: {
    CCTK_INFO("Using 8th order finite differencing. Make sure that you have at "
              "least 4 ghost zones");
    break;
  }
  }

  if (compute_error && !CCTK_Equals(initial_data, "exact_gaussian")) {
    CCTK_PARAMWARN(
        "Error computing was requested with an initial condition other than "
        "\"exact_gaussian\". The error estimate is only significant when "
        "evolving \"exact_gaussian\" data on top of a Minkowski background.");
  }
}
} // namespace KleinGordonX
