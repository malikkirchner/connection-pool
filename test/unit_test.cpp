/*====================================================================================================================*/
/*                                                                                                                    */
/*      CPOOL      Copyright ©2020 by  Malik Kirchner <kirchner@xelonic.com>                                          */
/*                                                                                                                    */
/*      This program is free software: you can redistribute it and/or modify                                          */
/*      it under the terms of the GNU General Public License as published by                                          */
/*      the Free Software Foundation, either version 3 of the License, or                                             */
/*      (at your option) any later version.                                                                           */
/*                                                                                                                    */
/*      This program is distributed in the hope that it will be useful,                                               */
/*      but WITHOUT ANY WARRANTY; without even the implied warranty of                                                */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                                                 */
/*      GNU General Public License for more details.                                                                  */
/*                                                                                                                    */
/*      You should have received a copy of the GNU General Public License                                             */
/*      along with this program.  If not, see <https://www.gnu.org/licenses/>.                                        */
/*                                                                                                                    */
/*====================================================================================================================*/

#define BOOST_TEST_MODULE cpool

#include <connection-pool/pool.h>

#include <boost/test/unit_test.hpp>


namespace cpool {

class TestConnection final : public Connection {
public:
    bool heart_beat() override { return connected; }

    bool is_healthy() override { return connected; }

    bool connect() override {
        connected = true;
        return connected;
    }

    void disconnect() override { connected = false; }

private:
    TestConnection() = default;
    friend ConnectionPoolFactory< TestConnection >;

    bool connected = false;
};

template <>
class ConnectionPoolFactory< TestConnection > {
public:
    static std::unique_ptr< ConnectionPool > create( const std::uint16_t num_connections ) {
        std::vector< std::unique_ptr< Connection > > connections;
        for ( std::uint16_t k = 0; k < num_connections; ++k ) {
            // cannot use std::make_unique, because constructor is hidden
            connections.emplace_back( std::unique_ptr< TestConnection >( new TestConnection{} ) );
        }
        return std::unique_ptr< ConnectionPool >( new ConnectionPool{std::move( connections )} );
    }
};

}    // namespace cpool


using namespace cpool;


BOOST_AUTO_TEST_SUITE( cpool_pool )

BOOST_AUTO_TEST_CASE( connection_pool ) {
    auto pool = ConnectionPoolFactory< TestConnection >::create( 4 );
    BOOST_CHECK( pool->size_busy() == 0 );
    BOOST_CHECK( pool->size_idle() == 4 );
    BOOST_CHECK( pool->size() == 4 );

    auto connection = pool->get_connection();
    BOOST_CHECK( connection.valid() );
    BOOST_CHECK( pool->size_busy() == 1 );
    BOOST_CHECK( pool->size_idle() == 3 );
    BOOST_CHECK( pool->size() == 4 );

    auto& test_connection = dynamic_cast< TestConnection& >( *connection );
    BOOST_CHECK( test_connection.is_healthy() );

    pool->release_connection( std::move( connection ) );
    BOOST_CHECK( pool->size_busy() == 0 );
    BOOST_CHECK( pool->size_idle() == 4 );
    BOOST_CHECK( pool->size() == 4 );

    pool->heart_beat();
}

BOOST_AUTO_TEST_SUITE_END()