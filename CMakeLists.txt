##====================================================================================================================##
##                                                                                                                    ##
##      CPOOL      Copyright ©2020 by  Malik Kirchner <kirchner@xelonic.com>                                          ##
##                                                                                                                    ##
##      This program is free software: you can redistribute it and/or modify                                          ##
##      it under the terms of the GNU General Public License as published by                                          ##
##      the Free Software Foundation, either version 3 of the License, or                                             ##
##      (at your option) any later version.                                                                           ##
##                                                                                                                    ##
##      This program is distributed in the hope that it will be useful,                                               ##
##      but WITHOUT ANY WARRANTY; without even the implied warranty of                                                ##
##      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                                                 ##
##      GNU General Public License for more details.                                                                  ##
##                                                                                                                    ##
##      You should have received a copy of the GNU General Public License                                             ##
##      along with this program.  If not, see <https://www.gnu.org/licenses/>.                                        ##
##                                                                                                                    ##
##====================================================================================================================##

cmake_minimum_required(VERSION 3.10)
project(cpool VERSION 1.0.0)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_subdirectory(src)
add_subdirectory(test)
