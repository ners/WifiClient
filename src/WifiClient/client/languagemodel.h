#pragma once

#include <QAbstractListModel>
#include <QString>
#include <QVector>

namespace WifiClient::Client
{

/** @class LanguageModel
 *  @brief Interface to reading available languages and setting the current one
 *  This class establishes a list of all the languages that the application is translated to,
 * and offers it to the application. The list never changes after construction.
 * The class also allows the application to set a new language, although it does not
 * retranslate the application.
 */
class LanguageModel : public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY(QString currentLanguage MEMBER m_currentLanguage WRITE setLanguage NOTIFY
				   languageChanged)
public:
	/** @brief Builds the list of the available languages by iterating through the application
	 * resources
	 */
	explicit LanguageModel(QObject* parent);
	/** @brief Proxy for size of languages list
	 *  @return The number of available languages
	 */
	Q_INVOKABLE [[nodiscard]] auto rowCount(QModelIndex const& parent = QModelIndex()) const
		-> int override;
	/** @brief Proxy for elements of languages list. Only the Flag role returns a value.
	 *  @return The language pointed to by index, if role is in Roles, empty QVariant otherwise
	 */
	Q_INVOKABLE [[nodiscard]] auto data(
		QModelIndex const& index, int role = Qt::DisplayRole) const -> QVariant override;
	Q_INVOKABLE [[nodiscard]] auto roleNames() const -> QHash<int, QByteArray> override;

	enum Roles : int
	{
		Flag
	};

signals:
	/** @brief Request the retranslation of the application to a new language. Also stores this
	 * language into the current language variable.
	 */
	void setLanguage(QString const& lang);
	void languageChanged();

private:
	QVector<QString> m_languages;
	QString m_currentLanguage;
};

} // namespace WifiClient::Client
