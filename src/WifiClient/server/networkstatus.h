#pragma once

#include <QDebug>
#include <QString>

namespace WifiClient::Server
{

/** @class NetworkStatus
 *  @brief A logic-less container that holds the description of a network in a given moment.
 * Does not include sensitive data like password.
 */
struct NetworkStatus
{
	QString m_ssid;
	bool m_locked = false;
	float m_strength = 0;
};

inline auto operator<<(QDebug& out, NetworkStatus const& network) -> QDebug&
{
	out.nospace() << QString("NetworkStatus{ssid=%1,strength=%2,locked=%3}")
						 .arg(network.m_ssid)
						 .arg(network.m_strength)
						 .arg(network.m_locked ? 1 : 0);
	return out;
}

} // namespace WifiClient::Server

Q_DECLARE_METATYPE(WifiClient::Server::NetworkStatus);
