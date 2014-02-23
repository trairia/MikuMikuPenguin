import QtQuick 2.1
import OpenGLUnderQML 1.0
import SceneGraphRendering 1.0
import "."

Item
{
	id:screen
	width: 480
	height: 320
	
	/*OpenGLScene
	{
		SequentialAnimation on t
		{
			NumberAnimation { to: 1; duration: 2500; easing.type: Easing.InQuad }
			NumberAnimation { to: 0; duration: 2500; easing.type: Easing.OutQuad }
			loops: Animation.Infinite
			running: true
		}
	}*/
	
	Renderer
	{
        id: renderer
        anchors.fill: parent
        //anchors.margins: 10

        // The transform is just to show something interesting..
        /*transform: [
            Rotation { id: rotation; axis.x: 0; axis.z: 0; axis.y: 1; angle: 0; origin.x: renderer.width / 2; origin.y: renderer.height / 2; },
            Translate { id: txOut; x: -renderer.width / 2; y: -renderer.height / 2 },
            Scale { id: scale; },
            Translate { id: txIn; x: renderer.width / 2; y: renderer.height / 2 }
		]*/
	}
	
	Drawer
	{
		id: bottomDrawer
		
		states:
		[
			State
			{
				name: "DRAWER_OPEN"
				PropertyChanges { target: bottomDrawer.button; height: 25; y: screen.height-146}
			},
			State
			{
				name: "DRAWER_CLOSED"
				PropertyChanges { target: flickableRow; y: screen.height}
				PropertyChanges { target: bottomDrawer.button; height: 50; y: screen.height-bottomDrawer.button.height}
				PropertyChanges { target: bottomDrawer.button.arrowIcon; rotation: 180}
			}
		]
		
		transitions:
		[
			Transition
			{
				to: "*"
				NumberAnimation { target: bottomDrawer.button; properties: "y"; duration: 100; easing.type: Easing.OutExpo }
				NumberAnimation { target: flickableRow; properties: "y"; duration: 100; easing.type: Easing.OutExpo }
			}
		]
	}
	
	Drawer
	{
		id: leftDrawer
		
		button.arrowIcon.source: "img/arrowLeftRight.png"
		
		states:
		[
			State
			{
				name: "DRAWER_OPEN"
				PropertyChanges { target: leftDrawer.button; height: 50; x: flickableGrid.x+flickableGrid.width; y:0; width: 25}
				PropertyChanges { target: leftDrawer.button.arrowIcon; rotation: 0}
			},
			State
			{
				name: "DRAWER_CLOSED"
				PropertyChanges { target: flickableGrid; y: screen.height}
				PropertyChanges { target: leftDrawer.button; x: 0; y:0; height: 50}
				PropertyChanges { target: leftDrawer.button.arrowIcon; rotation: 180}
			}
		]
		
		transitions:
		[
			Transition
			{
				to: "*"
				NumberAnimation { target: leftDrawer.button; properties: "x"; duration: 100; easing.type: Easing.OutExpo }
				NumberAnimation { target: flickableGrid; properties: "x"; duration: 100; easing.type: Easing.OutExpo }
			}
		]
	}
	
	FlickableGrid
	{
		id:flickableGrid
	}
	
	FlickableRow
	{
		id:flickableRow
	}
	
	
}
