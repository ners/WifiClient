#include <QDebug>
#include <QDir>
#include <QFont>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSettings>
#include <QUrl>
#include <WifiClient/client/application.h>
#include <WifiClient/client/connection.h>
#include <WifiClient/client/languagemodel.h>
#include <WifiClient/client/networkmodel.h>
#include <WifiClient/server/server.h>
#include <qcoreapplication.h>

namespace WifiClient::Client
{

static void loadFonts()
{
	QDir const fontDir(":/fonts");
	auto const fileNames = fontDir.entryList({"*.otf"});
	if (fileNames.empty())
	{
		qCritical() << "No font files found!";
	}
	for (auto const& fileName : fileNames)
	{
		qDebug() << "Adding font file:" << fileName;
		QFontDatabase::addApplicationFont(fontDir.filePath(fileName));
	}
}

Application::Application(int& argc, char** argv)
	: QGuiApplication(argc, argv)
	, translator(*new QTranslator(this))
	, server(*new Server::Server("config.json", this))
	, languageModel(*new LanguageModel(this))
	, networkModel(*new NetworkModel(this))
	, view(*new QQmlApplicationEngine(this))
	, connection(*new Connection(server, this))
{
	QCoreApplication::setOrganizationName("ACME");
	QCoreApplication::setApplicationName("WifiClient");
	QGuiApplication::setApplicationDisplayName("WifiClient");

	loadFonts();
	setFont(QFontDatabase().font("Inter", "Regular", 12));

	connect(&server, &Server::Server::foundNetwork, &networkModel, &NetworkModel::addNetwork);
	connect(&languageModel, &LanguageModel::setLanguage, this, &Application::setLanguage);

	languageModel.setLanguage(QSettings().value("Interface/language", "en_GB").toString());

	view.rootContext()->setContextProperty("languageModel", &languageModel);
	view.rootContext()->setContextProperty("networkModel", &networkModel);
	view.rootContext()->setContextProperty("connection", &connection);
	view.load(QUrl("qrc:/qml/client.qml"));
}

void Application::setLanguage(QString const& lang)
{
	QString langPath = QString(":/languages/application_%1").arg(lang);
	if (!translator.isEmpty())
	{
		QCoreApplication::removeTranslator(&translator);
	}

	bool loadLang = translator.load(langPath);
	if (!loadLang)
	{
		qCritical() << "Failed to load language:" << lang;
		return;
	}
	QCoreApplication::installTranslator(&translator);
	qDebug() << "Loaded language:" << lang;
	QSettings().setValue("Interface/language", lang);
	emit languageModel.languageChanged();
	view.retranslate();
}

} // namespace WifiClient::Client
