#pragma once

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>

namespace WifiClient::Server
{
class Server;
}

namespace WifiClient::Client
{

class LanguageModel;
class NetworkModel;
class Connection;

/** @class Application
 *  @brief Root of the QObject tree
 *  The Application class owns the top-level utility and display containers, and
 * orchestrates their interactions. It is also the only class that interacts with QSettings.
 */
class Application : public QGuiApplication
{
public:
	Application(int& argc, char** argv);
	using QGuiApplication::exec;

public slots:
	/** @brief Retranslates the application and stores the language in the settings.
	 *  @param lang The ISO/IEC 15897 locale identifier
	 */
	void setLanguage(QString const& lang);

private:
	QTranslator& translator;
	Server::Server& server;
	LanguageModel& languageModel;
	NetworkModel& networkModel;
	QQmlApplicationEngine& view;
	Connection& connection;
};

} // namespace WifiClient::Client
