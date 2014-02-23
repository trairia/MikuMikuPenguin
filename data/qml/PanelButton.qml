import QtQuick 2.1

Rectangle
{
	id: panelButton
	
	property alias text: label.text
	property alias selectMouse: selectMouse
		
	property color buttonColor: Qt.rgba(0.0, 0.0, 0.0, 0.5)
	property color onHoverColor: Qt.rgba(1, 1, 1, 1)
	property color borderColor: "white"
					
	radius: 10
					
	anchors.margins: parent.anchors.margins
	anchors.left: parent.left
	width: (parent.width/2)-parent.anchors.margins*2
	y: parent.y+15
	height: parent.height/4
					
	Text
	{
		id: label
		anchors.centerIn: parent
		//text: "Select"
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
