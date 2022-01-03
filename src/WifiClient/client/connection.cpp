#include <WifiClient/client/connection.h>
#include <WifiClient/server/networkstatus.h>
#include <WifiClient/server/server.h>
#include <memory>

namespace WifiClient::Client
{

Connection::Connection(Server::Server& server, QObject* parent)
	: QObject(parent)
	, m_server(server)
{
	connect(&server, &Server::Server::foundNetwork, this, &Connection::maybeUpdateStatus);
}

auto Connection::network() const -> QVariantMap
{
	if (!m_network)
	{
		return QVariantMap();
	}
	return QVariantMap(
		{{"ssid", m_network->m_ssid},
		 {"locked", m_network->m_locked},
		 {"strength", m_network->m_strength}});
}

auto Connection::tryConnect(QString const& ssid, QString const& password) -> bool
{
	m_network = m_server.tryAuth(ssid, password);
	emit networkUpdate();
	return m_network.has_value();
}

void Connection::maybeUpdateStatus(Server::NetworkStatus const& network)
{
	if (!m_network || m_network->m_ssid != network.m_ssid)
	{
		return;
	}
	m_network = network;
	emit networkUpdate();
}

} // namespace WifiClient::Client
