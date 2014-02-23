import QtQuick 2.1
import "."

Item
{
	id: drawer
	
	property alias button: button
	property variant statesNames:["DRAWER_CLOSED","DRAWER_OPEN"]
	
	state: "DRAWER_CLOSED"
	
	DrawerButton
	{
		id: button
		
		y: parent.height-146
	
		height:25
		width: 60
	}
}
