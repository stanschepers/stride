#############################################################################
#  This file is part of the Stride software.
#  It is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or any
#  later version.
#  The software is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  You should have received a copy of the GNU General Public License,
#  along with the software. If not, see <http://www.gnu.org/licenses/>.
#  see http://www.gnu.org/licenses/.
#
#  Copyright 2019 Willem L, Kuylen E & Broeckhove J
#############################################################################

#============================================================================
# Define smoke test. DO NOT BREAK THE COMMAND LINE.
#============================================================================
add_test(NAME sim_defaultpop_smoke
    WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}
    COMMAND ${BIN_INSTALL_LOCATION}/stride --override output_prefix=tests/sim_defaultpop_smoke
	)
add_test(NAME stan_defaultpop_smoke
	WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}
	COMMAND ${BIN_INSTALL_LOCATION}/stride --stan 3 --override output_prefix=tests/stan_defaultpop_smoke
	)
add_test(NAME sim_geopop_smoke
	WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}
	COMMAND ${BIN_INSTALL_LOCATION}/stride -c run_generate_default.xml --override output_prefix=tests/sim_generate_smoke
	)
add_test(NAME genpop_smoke
	WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}
	COMMAND ${BIN_INSTALL_LOCATION}/stride -e genpop -c run_generate_default.xml --override output_prefix=tests/genpop_smoke
	)

#============================================================================
# Add subdirectories:
#============================================================================
add_subdirectory(btester)
add_subdirectory(gtester)
add_subdirectory(myhayai)

#############################################################################
