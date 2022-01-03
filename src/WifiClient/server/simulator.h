#pragma once

#include <QObject>
#include <QTimer>
#include <WifiClient/server/networkstatus.h>
#include <chrono>
#include <random>

namespace WifiClient::Server
{

/** @class NetworkSimulator
 *  @brief Changes a network's strength randomly over time
 *  The NetworkSimulator class takes an initial network status, then makes incremental updates
 * to its strength. The frequency and magnitude of updates are random.
 * On each update, it emits a status update.
 */
class NetworkSimulator : public QObject
{
	Q_OBJECT
public:
	/** @brief Sets off the update timer
	 */
	explicit NetworkSimulator(NetworkStatus const& network, QObject* parent);

signals:
	/** @brief Notifies the listeners of the network update
	 *  @param network The new status of the network
	 */
	void update(NetworkStatus network);

private:
	NetworkStatus m_network;
	QTimer& m_updateTimer;
	std::mt19937 m_mt;
	std::uniform_int_distribution<int> m_delayDist;
	std::uniform_real_distribution<float> m_strengthDeltaDist;

private slots:
	/** @brief Sets the update timer to a random interval, then starts it.
	 */
	void restartTimer();
	/** @brief Updates the network and broadcasts it, then restarts the timer.
	 */
	void sendUpdate();
};

} // namespace WifiClient::Server
