#include <QCryptographicHash>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <WifiClient/server/config.h>
#include <WifiClient/server/networkstatus.h>
#include <WifiClient/server/server.h>
#include <WifiClient/server/simulator.h>
#include <optional>
#include <random>

namespace WifiClient::Server
{

static void readConfig(QString const& configFileName, Config& configuration)
{
	if (configFileName.isNull())
	{
		qCritical() << "Not loading configuration: no configuration file was selected.";
		return;
	}
	if (!configFileName.endsWith(".json"))
	{
		qCritical() << "Not loading configuration: invalid file type was selected.";
		return;
	}

	QFile configFile(configFileName);
	if (!configFile.exists())
	{
		qCritical() << "Cannot find config file!";
		return;
	}
	configFile.open(QIODevice::ReadOnly);
	if (!configFile.isOpen())
	{
		qCritical() << "Cannot open config file:" << configFile.errorString();
		return;
	}
	auto const json = configFile.readAll();
	configFile.close();

	auto const document = QJsonDocument::fromJson(json);
	configuration.read(document.object());
}

Server::Server(QString const& configFileName, QObject* parent)
	: QObject(parent)
{
	qCritical() << "Current directory:" << QDir::current().absolutePath();
	readConfig(configFileName, configuration);
	for (auto const& network : configuration.wifiConfig.networks)
	{
		networkConfig.insert(network.ssid, &network);
		auto* simulator = new NetworkSimulator(
			NetworkStatus{network.ssid, network.password.has_value()}, this);
		connect(simulator, &NetworkSimulator::update, this, &Server::updateNetwork);
	}
}

auto Server::tryAuth(QString const& ssid, QString const& password) const
	-> std::optional<NetworkStatus>
{
	if (!networkConfig.contains(ssid) || !networkStatus.contains(ssid))
	{
		return std::nullopt;
	}
	auto const& config = networkConfig.value(ssid);
	auto const& status = networkStatus.value(ssid);
	auto const hashedPassword =
		QCryptographicHash::hash(password.toLocal8Bit(), QCryptographicHash::Sha256).toHex();
	if (config->password.has_value() && config->password.value() != hashedPassword)
	{
		qDebug() << "Passwords do not match:\n"
				 << hashedPassword << "\n"
				 << config->password.value();
		return std::nullopt;
	}
	return status;
}

void Server::updateNetwork(NetworkStatus const& network)
{
	networkStatus[network.m_ssid] = network;
	emit foundNetwork(network);
}

} // namespace WifiClient::Server
