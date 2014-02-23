import QtQuick 2.1

Rectangle
{
	property alias text: label.text

	width: 200; height: 120;
	color: Qt.rgba(0.1, 0.1, 0.1, 0.1)
				
	radius: 10
	border.width: 1
	border.color: "white"
	//border.color: Qt.rgba(0, 0.6, 1, 1)
					
	anchors.margins: 5

	Text
	{
		id: label
		
		anchors.top: parent.top
		anchors.horizontalCenter: parent.horizontalCenter
					
		//text: "Bone Manipulation";
		color: "white";
	}
}
