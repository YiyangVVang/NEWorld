/*
* NEWorld: A free game with similar rules to Minecraft.
* Copyright (C) 2016 NEWorld Team
*
* This file is part of NEWorld.
* NEWorld is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* NEWorld is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with NEWorld.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SERVER_H_
#define SERVER_H_

#include <memory>
#include <vector>
#include <networkshared.h>
#include <logger.h>
#include <session.h>
#include <worldmanager.h>
#include <blockmanager.h>
#include <pluginmanager.h>
#include <pluginapi.h>

constexpr int updateInterval = 10, globalUpdateInterval = 10;

extern unsigned short globalPort;

class Server
{
public:
    Server(boost::asio::io_service& ioservice, unsigned short port, const std::string& base)
        : m_acceptor(ioservice, boost::asio::ip::tcp::endpoint(tcp::v4(), port)), m_socket(ioservice),
          m_worlds(m_plugins), m_world(m_worlds.addWorld("TestWorld"))
    {
        // Initialization
        PluginAPI::Blocks = &m_blocks;
        infostream << "Initializing plugins...";
        m_plugins.loadPlugins(base);
        // Start server
        infostream << "Server started!";
        doGlobalUpdate();
        doAccept();
    }

    ~Server()
    {
        // TODO: Terminate here
    }

    //void sendToAllSessions(Packet packet);

private:
    void doAccept();
    void doGlobalUpdate();

    tcp::acceptor m_acceptor;
    tcp::socket m_socket;
    std::vector<std::weak_ptr<Session>> m_sessions;

    WorldManager m_worlds;
    BlockManager m_blocks;
    PluginManager m_plugins; // Loaded plugins
    World& m_world; // Single world, only for debugging
};

#endif // SERVER_H__
