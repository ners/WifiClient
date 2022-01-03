import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.4

ApplicationWindow {
	visible: true
	minimumWidth: 480
	minimumHeight: 300

	ColumnLayout {
		id: layout
		anchors.fill: parent
		anchors.margins: 20

		RowLayout {
			LanguageSelection {
				id: language
				model: languageModel
				lang: languageModel ? languageModel.currentLanguage : ""
				onActivated: {
					languageModel.currentLanguage = language.lang
				}
			}

			Text {
				id: helloText
				Layout.fillWidth: true
				horizontalAlignment: Text.AlignHCenter
				height: 30
				text: qsTr("WiFi Setup")
				font.pointSize: 24
			}
		}

		InputField {
			label: qsTr("WiFi Network:")
			NetworkSelection {
				id: wifiNetwork
				model: networkModel
				onActivated: {
					wifiPassword.password = ""
				}
			}
		}

		InputField {
			label: qsTr("Password:")
			PasswordField {
				id: wifiPassword
				Layout.fillWidth: true
			}
		}

		RowLayout {
			Button {
				id: connectButton
				contentItem: Text {
					text: qsTr("Connect")
					font.pixelSize: 15
					horizontalAlignment: Text.AlignHCenter
					verticalAlignment: Text.AlignVCenter
					elide: Text.ElideRight
				}
				onClicked: {
					var ssid = wifiNetwork.ssid
					if (!ssid)
					{
						error(qsTr("No network selected"))
						return
					}
					var password = wifiPassword.password
					if (wifiNetwork.locked)
					{
						if (password == "")
						{
							error(qsTr("Password required"))
							return
						}
						// Must be a minimum of 16 characters
						if (password.length < 16)
						{
							error(qsTr("Password must be at least %L1 characters long").arg(16))
							return
						}
						// Cannot start with a numeric character.
						if (password[0] >= '0' && password[0] <= '9')
						{
							error(qsTr("Password cannot start with a number"))
							return
						}
					}
					tryConnect(ssid, password)
				}
			}
			Text {
				id: statusText
				font.pixelSize: 15
				Layout.leftMargin: 10
			}
		}
		RowLayout {
			Image {
				id: statusIcon
				source: connection.network["ssid"]
					? wifiNetwork.icon(connection.network["strength"], connection.network["locked"])
					: wifiNetwork.iconOff()
			}
			Text {
				id: statusSsid
				text: connection.network["ssid"] || qsTr("No connection")
			}
		}
	}
	function error(message) {
		statusText.text = message
		statusText.color = "red"
	}
	function success(message) {
		statusText.text = message
		statusText.color = "darkGreen"
	}
	function tryConnect(ssid, password) {
		var connected = connection.tryConnect(ssid, password)
		if (connected)
		{
			success(qsTr("Connection successful"))
			wifiPassword.password = ""
		}
		else
		{
			error(qsTr("Connection failed: wrong SSID or password"))
		}
	}
}
