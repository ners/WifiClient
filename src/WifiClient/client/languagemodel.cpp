#include <QDebug>
#include <QDir>
#include <QSettings>
#include <WifiClient/client/languagemodel.h>

namespace WifiClient::Client
{

LanguageModel::LanguageModel(QObject* parent)
	: QAbstractListModel(parent)
{
	auto const fileNames = QDir(":/languages").entryList({"*.qm"});

	if (fileNames.empty())
	{
		qCritical() << "No translation files found!";
	}

	for (auto const& fileName : fileNames)
	{
		qDebug() << "Found language file:" << fileName;
		auto const code = QString(fileName).replace(QRegExp("application_(.*).qm"), "\\1");
		m_languages.push_back(code);
	}

	connect(
		this,
		&LanguageModel::setLanguage,
		[this](auto const& lang) { m_currentLanguage = lang; });
}

auto LanguageModel::rowCount([[maybe_unused]] QModelIndex const& parent) const -> int
{
	// qDebug() << __PRETTY_FUNCTION__ << m_languages.count();
	return m_languages.count();
}

auto LanguageModel::data(QModelIndex const& index, int role) const -> QVariant
{
	// qDebug() << __PRETTY_FUNCTION__ << index << role;
	if (!index.isValid() || index.row() < 0 || index.row() >= rowCount())
	{
		return QVariant();
	}
	switch (role)
	{
	case Flag:
		return m_languages[index.row()];
	}
	return QVariant();
}

auto LanguageModel::roleNames() const -> QHash<int, QByteArray>
{
	return {{Flag, "flag"}};
}

} // namespace WifiClient::Client
