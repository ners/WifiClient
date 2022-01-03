#include <QDebug>
#include <QTimer>
#include <WifiClient/client/networkmodel.h>
#include <WifiClient/server/networkstatus.h>
#include <algorithm>
#include <chrono>

namespace WifiClient::Client
{

NetworkModel::NetworkModel(QObject* parent)
	: QAbstractListModel(parent)
{
	auto& updateTimer = *new QTimer(this);
	updateTimer.setSingleShot(false);
	updateTimer.setInterval(std::chrono::seconds(3));
	connect(&updateTimer, &QTimer::timeout, this, &NetworkModel::updateAll);
	updateTimer.start();
}

auto NetworkModel::rowCount(QModelIndex const& parent) const -> int
{
	// qDebug() << __PRETTY_FUNCTION__ << ssids.size();
	return networks.size();
}

auto NetworkModel::data(QModelIndex const& index, int role) const -> QVariant
{
	// qDebug() << __PRETTY_FUNCTION__ << index << role;
	if (!index.isValid() || index.row() < 0 || index.row() >= networks.size())
	{
		return QVariant();
	}
	auto const& network = networks[index.row()];
	switch (role)
	{
	case SsidRole:
		return network.m_ssid;
	case StrengthRole:
		return network.m_strength;
	case LockedRole:
		return network.m_locked;
	}
	return QVariant();
}

void NetworkModel::addNetwork(Server::NetworkStatus const& network)
{
	if (network.m_strength >= -85)
	{
		networksBySsid[network.m_ssid] = network;
	}
	else
	{
		networksBySsid.remove(network.m_ssid);
	}
}

auto NetworkModel::roleNames() const -> QHash<int, QByteArray>
{
	return {
		{SsidRole, "ssid"},
		{LockedRole, "locked"},
		{StrengthRole, "strength"},
	};
}

void NetworkModel::updateAll()
{
	beginResetModel();
	networks.clear();
	for (auto const& network : networksBySsid.values())
	{
		networks.push_back(network);
	}
	std::sort(
		networks.begin(),
		networks.end(),
		[](auto const& n1, auto const& n2) { return n1.m_strength > n2.m_strength; });
	endResetModel();
}

} // namespace WifiClient::Client
