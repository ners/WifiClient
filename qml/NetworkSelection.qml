import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.4

ComboBox {
	id: combobox
	Layout.fillWidth: true

	property string ssid
	property double strength
	property bool locked

	contentItem: Rectangle {
		anchors.left: parent.left
		anchors.right: parent.right
		color: "#DDD"

		Image {
			id: currentIcon
			height: 15
			fillMode: Image.PreserveAspectFit
			anchors.verticalCenter: parent.verticalCenter
			anchors.left: parent.left
			anchors.leftMargin: 10
			verticalAlignment: Image.AlignVCenter
		}
		Text {
			id: currentSsid
			anchors.verticalCenter: parent.verticalCenter
			anchors.left: currentIcon.right
			anchors.leftMargin: 10
			text: combobox.ssid
		}
	}

	delegate: ItemDelegate {
		id: delegateModel
		anchors.left: parent ? parent.left : undefined
		anchors.right: parent ? parent.right : undefined
		contentItem: Rectangle {
			anchors.left: parent.left
			anchors.right: parent.right
			color: "transparent"
			Image {
				id: icon
				source: combobox.icon(strength, locked)
				height: 15
				fillMode: Image.PreserveAspectFit
				anchors.left: parent.left
				anchors.leftMargin: 10
			}
			Text {
				text: ssid
				anchors.left: icon.right
				anchors.leftMargin: 10
				anchors.right: parent.right
			}
		}
		highlighted: combobox.highlightedIndex === index
	}

	onActivated: {
		var model = delegateModel.items.get(currentIndex).model
		combobox.ssid = model.ssid
		combobox.strength = model.strength
		combobox.locked = model.locked
		currentIcon.source = icon(model.strength, model.locked)
	}
	function iconOff() {
		return "../images/wifi-strength-off-outline.svg"
	}
	function icon(strength, locked) {
		if (strength >= -40)
			strength = 4
		else if (strength >= -50)
			strength = 3
		else if (strength >= -70)
			strength = 2
		else
			strength = 1
		return "../images/wifi-strength-%1%2.svg".arg(strength).arg(locked ? "-lock" : "")
	}
}
