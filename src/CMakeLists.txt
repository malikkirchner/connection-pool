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

add_library(cpool STATIC pool.cpp connection.cpp)
target_include_directories( cpool PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)

set_property(TARGET cpool PROPERTY CXX_STANDARD 17)
target_compile_features(cpool PUBLIC cxx_std_17)

install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" )
install(TARGETS cpool EXPORT cpool-export LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")
install(EXPORT cpool-export FILE cpool-targets.cmake NAMESPACE xelonic:: DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cpool")

configure_package_config_file(cpool-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/cpool-config.cmake INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpool)

write_basic_package_version_file( ${CMAKE_CURRENT_BINARY_DIR}/cpool-config-version.cmake VERSION ${PROJECT_VERSION} COMPATIBILITY SameMinorVersion )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cpool-config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/cpool-config-version.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpool )
