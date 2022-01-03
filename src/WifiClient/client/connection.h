#pragma once

#include <QObject>
#include <QString>
#include <WifiClient/server/networkstatus.h>
#include <WifiClient/server/server.h>
#include <memory>

namespace WifiClient::Client
{

/** @class Connection
 *  @brief Encapsulates an established connection
 *  The Connection class describes the status of the current connection, if one is established;
 * or the lack thereof. To avoid memory problems, this class owns its copy of the network
 * status, which it updates when the server broadcasts updates. If the server removes its copy
 * of the network, the Connection's copy will persist until it is severed otherwise.
 */
class Connection : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QVariantMap network READ network NOTIFY networkUpdate)

public:
	Connection(Server::Server& server, QObject* parent);

	/** @brief Cast the optional NetworkStatus to a VariantMap.
	 *  This method is necessary because the QML engine does not know how to work with QVariant
	 * or NetworkStatus*. Each of the NetworkStatus fields has a key with the same name.
	 *  @return A mapping from keys to QVariant values, e.g. "ssid" => QVariant("Network")
	 */
	[[nodiscard]] QVariantMap network() const;

	/** @brief Attempt to establish a connection against the server.
	 *  If the connection succeeds, this class begins to emit network updates for this network.
	 *  @return Was the connection successful
	 */
	Q_INVOKABLE bool tryConnect(QString const& ssid, QString const& password);

signals:
	void networkUpdate();

private:
	Server::Server& m_server;
	std::optional<Server::NetworkStatus> m_network;
	void maybeUpdateStatus(Server::NetworkStatus const&);
};

} // namespace WifiClient::Client
