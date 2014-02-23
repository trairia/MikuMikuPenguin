import QtQuick 2.1

Rectangle
{
	id: flickablePanel
 
	//width: 800; height: 480
	anchors.left: parent.left
	anchors.right: parent.right
	//anchors.bottom: parent.bottom
	
	y: parent.height-120
	height: 120
	
	clip: true
	
	Behavior on color { ColorAnimation { duration: 200 } }
	color: Qt.rgba(0.1, 0.1, 0.1, 0.5)
 
	Flickable
	{
		id: flickable
 
		anchors.fill: parent
		contentWidth: (200+5)*5; contentHeight: flickable.height
 
		//onMovementStarted: flickablePanel.color = Qt.rgba(0,0,0, 0.2)
		//onMovementEnded: flickablePanel.color = Qt.rgba(0, 0, 0, 0.2)
 
		Row
		{
			x: 0
			spacing: 5
			
			Panel
			{
				//color: Qt.rgba(0.1, 0.1, 0.1, 1)
				border.color: Qt.rgba(0.5, 0.5, 0.5, 1)
				text: "Interpolation";
				
				Column
				{
					anchors.margins: parent.anchors.margins
					
					x: parent.x
					y: parent.y+3
					
					spacing: 0
					PanelTab
					{
						text: "X"
					}
					PanelTab
					{
						text: "Y"
					}
					PanelTab
					{
						text: "Z"
					}
					PanelTab
					{
						text: "R"
					}
				}
				
				BezierCanvas
				{
					
				}
			}
			
			FlipablePanel
			{
				id: bonePanel
				
				front:
				Panel
				{
					border.color: Qt.rgba(0, 0.6, 1, 1)
					text: "Bone Manipulation";
					
					PanelButton
					{
						id: selectButton
						text: "Select"
						
						selectMouse.visible: bonePanel.flipped==false? true: false
					}
					PanelButton
					{
						id: rotateButton
						text: "Rotate"
						
						anchors.top: selectButton.bottom
						
						selectMouse.visible: bonePanel.flipped==false? true: false
					}
					PanelButton
					{
						id: translateButton
						text: "Translate"
						
						anchors.top: rotateButton.bottom
						
						selectMouse.visible: bonePanel.flipped==false? true: false
					}
					
					PanelButton
					{
						id: setButton
						text: "Register"
						
						anchors.left: translateButton.right
						
						selectMouse.visible: bonePanel.flipped==false? true: false
					}
					
					PanelButton
					{
						id: resetButton
						text: "Reset"
						
						anchors.left: translateButton.right
						anchors.top: setButton.bottom
						
						selectMouse.visible: bonePanel.flipped==false? true: false
					}
				}
				back:
				Panel
				{
					border.color: Qt.rgba(0, 0.6, 1, 1)
					text: "Bone Settings";
					
					anchors.centerIn: parent
				}
			}
			
			Panel
			{				
				border.color: Qt.rgba(0, 1, 0.4, 1)
				text: "Face Manipulation";
			}
			
			Panel
			{				
				border.color: Qt.rgba(1, 0, 1, 1)
				text: "Camera Manipulation";
			}
			
			Panel
			{				
				border.color: Qt.rgba(1, 0.5, 0.0, 1)
				text: "Model Settings";
			}
				
				
			/*Repeater
			{
				id: repeater
				model: 20
				Rectangle
				{
					width: 2; height: flickable.height; color: "white"
 
					Text
					{
						anchors.bottom: parent.bottom
						x: -15; text: index + 1; color: "white"
					}
				}
			}*/
		}
	}
}
