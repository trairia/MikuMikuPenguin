import QtQuick 2.1
import "."

Flipable
{
	id: flipable
	width: 200; height: flickable.height;
    //width: 240
	//height: 240

	property bool flipped: false

	//front: Panel { anchors.centerIn: parent }
	//back: Panel { anchors.centerIn: parent }

	transform: Rotation
	{
		id: rotation
		origin.x: flipable.width/2
		origin.y: flipable.height/2
		axis.x: 0; axis.y: 1; axis.z: 0     // set axis.y to 1 to rotate around y-axis
		angle: 0    // the default angle
	}

	states: State
	{
		name: "back"
		PropertyChanges { target: rotation; angle: 180 }
		PropertyChanges { target: front; }
		when: flipable.flipped
	}

	transitions: Transition
    {
		NumberAnimation { target: rotation; property: "angle"; duration: 300 }
	}

	MouseArea
	{
		anchors.fill: parent		
		onDoubleClicked: flipable.flipped = !flipable.flipped
		//acceptedButtons: Qt.RightButton
		//acceptedButtons: Qt.LeftButton | Qt.RightButton
	}
}
