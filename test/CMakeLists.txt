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

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

include(CTest)

enable_testing()

add_executable(unit-test unit_test.cpp)

target_link_libraries(unit-test cpool ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(unit-test unit-test --show_progress=yes --log_format=XML --log_sink=cpool.xml --log_level=all --report_level=no)

add_custom_target(test
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Starting to run unit tests ..."
    COMMAND ${CMAKE_CTEST_COMMAND} --schedule-random --output-on-failure
    DEPENDS unit-test USES_TERMINAL
)