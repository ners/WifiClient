import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.4

Rectangle {
	Layout.fillWidth: true

	property alias password: password.text

	height: 40
	color: "#DDD"

	TextField {
		id: password
		font.pixelSize: 20
		anchors.left: parent.left
		anchors.right: visibility.left
		anchors.rightMargin: 5
		anchors.verticalCenter: parent.verticalCenter
		echoMode: TextInput.Password
		background: Rectangle {
			color: "transparent"
			border.color: "transparent"
		}
	}
	Image {
		id: visibility
		source: "../images/visibility.svg"
		width: 25
		height: 25
		anchors.right: parent.right
		anchors.rightMargin: 5
		anchors.verticalCenter: parent.verticalCenter
	}
	MouseArea {
		anchors.fill: visibility
		onPressed: password.echoMode = TextInput.Normal
		onReleased: password.echoMode = TextInput.Password
	}
}
