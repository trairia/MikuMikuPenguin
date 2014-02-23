import QtQuick 2.1

Rectangle
{
	id: flickableGrid
	
	radius: 2
	border.width: 1
	border.color: "white"
	
	x:0
	width: 480-anchors.margins-25
	y:0
	height: bottomDrawer.button.y
	
	anchors.margins:5 
	
	Behavior on color { ColorAnimation { duration: 200 } }
	color: Qt.rgba(0.1, 0.1, 0.1, 0.5)
	//color: Qt.rgba(0.1,0.1,0.1,1);
	
	clip: true
	
	Flickable
	{
		id: flickable
		
		//anchors.fill: parent
		//anchors.margins:1
		
		clip: true
		
		x: parent.x+30
		width: parent.width-25
		anchors.bottom:parent.bottom
		anchors.top:parent.top
		anchors.margins:1
		
		//contentWidth: sidebarGrid.x+sidebarGrid.width
		//contentHeight: sidebarGrid.y+sidebarGrid.height
		
		property real columnCount: 61
		property real rowCount: 50
		property real columnSize: 35
		property real rowSize: 25
		
		contentWidth: columnCount*columnSize
		contentHeight: rowCount*rowSize
 
		//onMovementStarted: flickablePanel.color = Qt.rgba(0,0,0, 0.2)
		//onMovementEnded: flickablePanel.color = Qt.rgba(0, 0, 0, 0.2)
		
		Repeater
		{
			model: flickable.columnCount
			
			Rectangle
			{
				x: flickable.columnSize*index+15
				width: index%5==0? 2 : 1;
				height: flickable.contentHeight
				color: index%5==0? "white" : "grey"
			}
		}
		
		Repeater
		{
			model: flickable.rowCount
			
			Rectangle
			{
				width: flickable.contentWidth-flickable.columnSize; height: 1
				x: 15
				y: flickable.rowSize*(index+1)-2
				color: Qt.rgba(0.5,0.2,0.0,1);
			}
		}
	}
	
	Rectangle
	{
		//anchors.fill: parent
		anchors.margins:1
		
		x:14+flickable.x-flickable.contentX;
		y:1;
		width: flickable.contentWidth-20;
		height: flickable.rowSize-10;
		color: Qt.rgba(0.5,0.3,0.2,1);
	}
	Repeater
	{
		model: flickable.columnCount
			
		Rectangle
		{
			x: flickable.columnSize*index+15+flickable.x-flickable.contentX
			width: 0; height: flickable.rowSize
			//color: index%5==0? "white" : "grey"
			
			Text
			{
				visible: index%5==0? true:false
				//anchors.top: parent.top
				x: index<10? -2.7 : -5;
				text: index;
				color: "white"
			}
		}
	}
}
