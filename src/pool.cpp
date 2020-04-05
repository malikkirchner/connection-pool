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

#include <connection-pool/pool.h>

#include <algorithm>


namespace cpool {


/*====================================================================================================================*/
/* ConnectionPool::ConnectionProxy                                                                                    */
/*====================================================================================================================*/

ConnectionPool::ConnectionProxy::ConnectionProxy( ConnectionPool* pool, Connection* connection ) noexcept
    : m_pool{pool}, m_connection{connection} {}

ConnectionPool::ConnectionProxy::ConnectionProxy( ConnectionPool::ConnectionProxy&& other ) noexcept
    : m_pool{other.m_pool}, m_connection{other.m_connection} {
    other.m_pool       = nullptr;
    other.m_connection = nullptr;
}

ConnectionPool::ConnectionProxy&
ConnectionPool::ConnectionProxy::operator=( ConnectionPool::ConnectionProxy&& other ) noexcept {
    m_pool             = other.m_pool;
    m_connection       = other.m_connection;
    other.m_pool       = nullptr;
    other.m_connection = nullptr;
    return *this;
}

ConnectionPool::ConnectionProxy::~ConnectionProxy() {
    if ( m_pool != nullptr ) {
        m_pool->release_connection( m_connection );
    }
}

Connection* ConnectionPool::ConnectionProxy::operator->() { return m_connection; }

Connection& ConnectionPool::ConnectionProxy::operator*() { return *m_connection; }

bool ConnectionPool::ConnectionProxy::valid() const {
    if ( ( m_pool != nullptr ) && ( m_connection != nullptr ) ) {
        std::unique_lock lock{m_pool->m_connections_mtx};
        return m_pool->m_connections_busy.count( m_connection ) > 0;
    }

    return false;
}


/*====================================================================================================================*/
/* ConnectionPool                                                                                                     */
/*====================================================================================================================*/

namespace {

bool check_connect( Connection& connection ) {
    if ( !connection.is_healthy() ) {
        return connection.connect();
    }

    return true;
}

}    // namespace


ConnectionPool::ConnectionPool( std::vector< std::unique_ptr< Connection > >&& connections ) {
    for ( auto& connection : connections ) {
        Connection* key = connection.get();
        m_connections_idle.emplace( key, std::move( connection ) );
    }
}

ConnectionPool::~ConnectionPool() = default;

std::optional< ConnectionPool::ConnectionProxy > ConnectionPool::get_connection() {
    std::unique_lock lock{m_connections_mtx};

    if ( m_connections_idle.empty() ) {
        return std::nullopt;
    }

    for ( auto& item : m_connections_idle ) {
        if ( !check_connect( *item.second ) ) {
            continue;
        }

        ConnectionProxy proxy{this, item.first};
        m_connections_busy.emplace( item.first, std::move( item.second ) );
        m_connections_idle.erase( m_connections_idle.begin() );

        return proxy;
    }

    return std::nullopt;
}

void ConnectionPool::release_connection( ConnectionPool::ConnectionProxy&& proxy ) {
    release_connection( proxy.m_connection );
}

void ConnectionPool::release_connection( Connection* connection ) {
    if ( connection == nullptr ) {
        return;
    }

    std::unique_lock lock{m_connections_mtx};
    if ( auto it = m_connections_busy.find( connection ); it != m_connections_busy.end() ) {
        check_connect( *it->second );
        m_connections_idle.emplace( it->first, std::move( it->second ) );
        m_connections_busy.erase( it );
    }
}

std::size_t ConnectionPool::size() const {
    std::unique_lock lock{m_connections_mtx};
    return m_connections_busy.size() + m_connections_idle.size();
}

std::size_t ConnectionPool::size_idle() const {
    std::unique_lock lock{m_connections_mtx};
    return m_connections_idle.size();
}

std::size_t ConnectionPool::size_busy() const {
    std::unique_lock lock{m_connections_mtx};
    return m_connections_busy.size();
}

void ConnectionPool::heart_beat() {
    std::unique_lock lock{m_connections_mtx};

    // Only send heart beat on idle connections,
    // busy connections should be busy for a reason.
    for ( auto& connection : m_connections_idle ) {
        connection.second->heart_beat();
    }
}


}    // namespace cpool