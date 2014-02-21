import QtQuick 2.1

Rectangle
{
	property alias arrowIcon: arrowIcon
	property alias mouseArea: mouseArea
	
	/*y: parent.height-146
	
	height:25
	width: 60*/
	
	radius: 5
	
	border.width: 1
	border.color: Qt.rgba(1, 1, 1, 1)
	
	Image
	{
		id: arrowIcon
		antialiasing: false
		source: "img/arrowUpDown.png"
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.verticalCenter: parent.verticalCenter
	}
	
	MouseArea
	{
		id: mouseArea
		anchors.fill:parent
		onClicked:
		{
			if(drawer.state == "DRAWER_CLOSED")
			{
				drawer.state = "DRAWER_OPEN"
			}
			else if (drawer.state == "DRAWER_OPEN")
			{
				drawer.state = "DRAWER_CLOSED"
			}
		}
	}
	color: mouseArea.pressed ? Qt.rgba(0.5, 0.5, 0.5, 1.0) : Qt.rgba(0.1,0.1,0.1, 0.0)
}
