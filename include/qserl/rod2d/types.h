/**
* Copyright (c) 2012-2014 CNRS
* Author: Olivier Roussel
*
* This file is part of the qserl package.
* qserl is free software: you can redistribute it
* and/or modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation, either version
* 3 of the License, or (at your option) any later version.
*
* qserl is distributed in the hope that it will be
* useful, but WITHOUT ANY WARRANTY; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Lesser Public License for more details.  You should have
* received a copy of the GNU Lesser General Public License along with
* qserl.  If not, see
* <http://www.gnu.org/licenses/>.
**/

#ifndef QSERL_2D_TYPES_H_
#define QSERL_2D_TYPES_H_

#include "qserl/exports.h"

#include <boost/array.hpp>

namespace qserl {
namespace rod2d {

/**< For convenience, a 2-dimensional wrench (with reference to wrenches in screw theory
* for 3-dimensional bodies) is stored in an array where index storage represent:
*	0: Force along X axis
*	1: Force along Y axis
*	2: Torque
*/
typedef boost::array<double, 3>		Wrench2D;

/**< For convenience, a 2-dimensional displacement (with reference to displacement in screw theory
* for 3-dimensional bodies is stored in an array where index storage represent:
*	0: Position along X axis
*	1: Position along Y axis
*	2: Rotation
*/
typedef boost::array<double, 3>		Displacement2D;

}	// namespace rod2d
}	// namespace qserl

#endif // QSERL_2D_TYPES_H_
