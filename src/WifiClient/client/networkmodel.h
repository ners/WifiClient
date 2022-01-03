#pragma once

#include <QAbstractListModel>
#include <QHash>
#include <QVariant>
#include <QVector>
#include <WifiClient/server/networkstatus.h>

namespace WifiClient::Client
{

/** @class NetworkModel
 *  @brief Interface to reading available networks
 *  This class establishes a list of all the networks that have been published by the server
 * and offers it to the application. It makes fresh copies of NetworkStatus with each server
 * update, but only refreshes application data every few seconds. When doing so, the network
 * statuses are sorted in descending order of signal strength.
 */
class NetworkModel : public QAbstractListModel
{
public:
	explicit NetworkModel(QObject* parent);
	/** @brief Proxy for size of networks list. This number changes every few seconds.
	 *  @return The number of available networks
	 */
	Q_INVOKABLE [[nodiscard]] auto rowCount(const QModelIndex& parent = QModelIndex()) const
		-> int override;
	/** @brief Proxy for elements of network list.
	 *  @return The network field pointed to by index, if role is in Roles, empty QVariant
	 * otherwise
	 */
	Q_INVOKABLE [[nodiscard]] auto data(
		const QModelIndex& index, int role = Qt::DisplayRole) const -> QVariant override;
	Q_INVOKABLE [[nodiscard]] auto roleNames() const -> QHash<int, QByteArray> override;

	/** @brief Add a new network to the list of networks. Gets added to the map immediately,
	 * but only updates the application every few seconds.
	 */
	void addNetwork(Server::NetworkStatus const& network);

	enum Roles : int
	{
		SsidRole,
		LockedRole,
		StrengthRole,
	};

private:
	/** @brief Replaces the list of networks with a fresh one from the map of active networks.
	 * Sorts the list by descending signal strength. Notifies the application to redraw.
	 */
	void updateAll();
	QVector<Server::NetworkStatus> networks;
	QHash<QString, Server::NetworkStatus> networksBySsid;
};

} // namespace WifiClient::Client
