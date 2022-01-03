import QtQuick 2.0
import QtQuick.Controls 2.0

ComboBox {
	id: combobox
	// editable: true
	width: 40
	height: 20
	background: Rectangle {
		color: "transparent"
	}

	property string lang

	contentItem: Rectangle {
		width: 40
		Image {
			id: currentIcon
			source: icon(lang)
			sourceSize.width: 40
			sourceSize.height: 40
			anchors.verticalCenter: parent.verticalCenter
			verticalAlignment: Image.AlignVCenter
		}
	}

	delegate: ItemDelegate {
		id: delegateModel
		anchors.left: parent.left
		anchors.right: parent.right
		contentItem: Rectangle {
			width: 20
			height: 20
			color: "transparent"
			anchors.margins: 0
			Image {
				id: icon
				source: combobox.icon(flag)
				sourceSize.width: 20
				sourceSize.height: 20
			}
		}
		highlighted: combobox.highlightedIndex === index
	}
	indicator: Rectangle { }
	onActivated: {
		var model = delegateModel.items.get(currentIndex).model
		combobox.lang = model.flag
		currentIcon.source = icon(model.flag)
	}
	function icon(lang) {
		return "../images/languages/%1.svg".arg(lang)
	}
}
