#include <QDebug>
#include <WifiClient/server/simulator.h>
#include <chrono>
#include <random>

namespace WifiClient::Server
{

NetworkSimulator::NetworkSimulator(NetworkStatus const& network, QObject* parent)
	: QObject(parent)
	, m_network(network)
	, m_updateTimer(*new QTimer(this))
	, m_mt(std::random_device()())
	, m_delayDist(100, 500)
	, m_strengthDeltaDist(-1, 1)
{
	std::uniform_real_distribution<float> strengthDist(-90, -30);
	m_network.m_strength = strengthDist(m_mt);
	connect(&m_updateTimer, &QTimer::timeout, this, &NetworkSimulator::sendUpdate);
	m_updateTimer.setSingleShot(true);
	restartTimer();
}

void NetworkSimulator::restartTimer()
{
	m_updateTimer.stop();
	std::chrono::milliseconds const delay(m_delayDist(m_mt));
	m_updateTimer.setInterval(delay);
	m_updateTimer.start();
}

void NetworkSimulator::sendUpdate()
{
	m_network.m_strength += m_strengthDeltaDist(m_mt);
	update(m_network);
	restartTimer();
}

} // namespace WifiClient::Server
