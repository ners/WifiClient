import QtQuick 2.0
import QtQuick.Layouts 1.4

ColumnLayout {
	Layout.fillWidth: true

	height: 50
	property string label

	Text {
		text: label
	}
}
