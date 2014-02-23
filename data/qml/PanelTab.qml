import QtQuick 2.1

Rectangle
{
	id: panelTab
	
	property alias text: label.text
		
	property color buttonColor: Qt.rgba(0.0, 0.0, 0.0, 0.7)
	property color onHoverColor: Qt.rgba(1, 1, 1, 1)
	property color borderColor: "white"
					
	radius:2
	border.width: 1
	border.color: "white"
					
	anchors.margins: parent.anchors.margins
	anchors.left: parent.left
	width: 20
	y: parent.y
	height: 28
					
	Text
	{
		id: label
		anchors.centerIn: parent
		color: "white"
	}
					
	MouseArea
	{
		id: selectMouse
		anchors.fill: parent
		//onClicked: parent.width += 10
		//onHover: color=onHoverColor
	}
	color: selectMouse.pressed ? Qt.rgba(1, 1, 1, 0.5) : buttonColor
}
