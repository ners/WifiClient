#include <WifiClient/server/config.h>

#include <QDebug>

namespace WifiClient::Server
{

void Config::WifiConfig::WifiNetwork::read(QJsonObject const& json)
{
	ssid = json["ssid"].toString();
	if (json["password"].isString())
	{
		password = json["password"].toString();
	}
	else
	{
		password = std::nullopt;
	}
}

void Config::WifiConfig::WifiNetwork::write(QJsonObject& json) const
{
	json["ssid"] = ssid;
	if (password.has_value())
	{
		json["password"] = password.value();
	}
	else
	{
		json.remove("password");
	}
}

void Config::WifiConfig::read(QJsonArray const& json)
{
	for (auto const& value : json)
	{
		if (!value.isObject())
		{
			qCritical() << "WiFi config is not an object!";
			continue;
		}
		WifiNetwork network;
		network.read(value.toObject());
		networks.push_back(network);
	}
}

void Config::WifiConfig::write(QJsonArray& json) const
{
	for (auto const& network : networks)
	{
		QJsonObject networkJson;
		network.write(networkJson);
		json.push_back(networkJson);
	}
}

void Config::read(QJsonObject const& json)
{
	auto const wifiConfigJson = json["wifiConfig"];
	if (!wifiConfigJson.isArray())
	{
		qCritical() << "Config contains no wifiConfig";
		return;
	}
	wifiConfig.read(wifiConfigJson.toArray());
}

void Config::write(QJsonObject& json) const
{
	QJsonArray wifiConfigJson;
	wifiConfig.write(wifiConfigJson);
	json["wifiConfig"] = wifiConfigJson;
}

} // namespace WifiClient::Server
