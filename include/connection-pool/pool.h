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

#pragma once

#include <connection-pool/connection.h>

#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>


namespace cpool {


class ConnectionPool {
public:
    class ConnectionProxy final {
        friend ConnectionPool;

    public:
        ConnectionProxy( ConnectionProxy&& other ) noexcept;
        ConnectionProxy& operator=( ConnectionProxy&& other ) noexcept;

        ~ConnectionProxy();

        Connection* operator->();
        Connection& operator*();

        bool valid() const;

    private:
        ConnectionProxy( ConnectionPool* pool, Connection* connection ) noexcept;

        ConnectionPool* m_pool;
        Connection*     m_connection;
    };

protected:
    explicit ConnectionPool( std::vector< std::unique_ptr< Connection > >&& connections );

    template < class T >
    friend class ConnectionPoolFactory;

public:
    virtual ~ConnectionPool();

    ConnectionProxy get_connection();
    void            release_connection( ConnectionProxy&& proxy );

    std::size_t size() const;
    std::size_t size_idle() const;
    std::size_t size_busy() const;

    void heart_beat();

private:
    void release_connection( Connection* connection );

    mutable std::mutex                                               m_connections_mtx;
    std::unordered_map< Connection*, std::unique_ptr< Connection > > m_connections_idle;
    std::unordered_map< Connection*, std::unique_ptr< Connection > > m_connections_busy;
};


template < class T >
class ConnectionPoolFactory final {
private:
    ConnectionPoolFactory();
};


}    // namespace cpool