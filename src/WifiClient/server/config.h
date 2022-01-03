#pragma once

#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <QVector>
#include <optional>

namespace WifiClient::Server
{

/** @class Config
 *  @brief Holds the network configurations that the server knows about.
 */
struct Config
{
	struct WifiConfig
	{
		struct WifiNetwork
		{
			QString ssid;
			std::optional<QString> password;
			void read(QJsonObject const& json);
			void write(QJsonObject& json) const;
		};
		QVector<WifiNetwork> networks;
		void read(QJsonArray const& json);
		void write(QJsonArray& json) const;
	};
	WifiConfig wifiConfig;
	void read(QJsonObject const& json);
	void write(QJsonObject& json) const;
};

} // namespace WifiClient::Server
