import QtQuick 2.1

Item
{
	//anchors.fill: parent
	anchors.bottom: parent.bottom
	anchors.top: parent.top
	anchors.margins: 5
	x: 30
	width:parent.width-30
	
	Canvas
	{
		id:canvas
		antialiasing: true
						
		anchors.fill: parent					
						
		property color strokeStyle: Qt.rgba(0,0.65,1,1)
		property string fillStyle:"red"
		property int lineWidth: 5
		property real alpha: 1
		
		renderStrategy: Canvas.Threaded
		tileSize: "16x16"
						
		onPaint:
		{
			var ctx = canvas.getContext('2d');
			ctx.save();
			ctx.clearRect(0, 0, canvas.width, canvas.height);
			
			ctx.beginPath();
			ctx.moveTo(0,height); //start point
			ctx.lineTo(p1.x+p1.width/2,p1.y+p1.height/2);
			ctx.strokeStyle = Qt.rgba(0.3,0.3,0.3,1);
			ctx.stroke().closePath;
			
			ctx.beginPath();
			ctx.moveTo(width,0); //start point
			ctx.lineTo(p2.x+p2.width/2,p2.y+p2.height/2);
			ctx.strokeStyle = Qt.rgba(0.3,0.3,0.3,1);
			ctx.stroke().closePath;
							
			ctx.strokeStyle = canvas.strokeStyle;
			ctx.lineWidth = canvas.lineWidth;
			ctx.beginPath();
			ctx.moveTo(0, height);//start point
			ctx.bezierCurveTo(p1.x+p1.width/2, p1.y+p1.height/2, p2.x+p2.width/2, p2.y+p2.height/2, width, 0);
			ctx.stroke().closePath();
		}
							
	}
	
	/*Rectangle
	{
		id: p0
		x: 0-width/2
		y: canvas.height-height/2
		width: 20
		height: 20
		
		radius:30
		antialiasing: true
		
		color: Qt.rgba(0, 0, 0, 0.0)
		border.width: 1
		border.color: "white"
	}
	
	Rectangle
	{
		id: p3
		x: canvas.width-width/2
		y: 0-height/2
		width: 20
		height: 20
		
		radius:30
		antialiasing: true
		
		color: Qt.rgba(0, 0, 0, 0.0)
		border.width: 1
		border.color: "white"
	}*/
	
	Rectangle
	{
		id: p1
		x: (canvas.width/2)-(width/2)
		y: (canvas.height/2)-(height/2)
		width: 20
		height: 20
		
		radius:30
		//antialiasing: true
		
		color: Qt.rgba(1, 0, 1, 1)
		border.width: 1
		border.color: Qt.darker(color,1.5);
		
		onXChanged: canvas.requestPaint();
		onYChanged: canvas.requestPaint();
		
		MouseArea
		{
            anchors.fill: parent
            drag.target: p1
            drag.minimumX: 0
            drag.maximumX: canvas.width-p2.width
            drag.minimumY: 0
            drag.maximumY: canvas.height-p2.height
        }
	}
	
	Rectangle
	{
		property real xVal: (canvas.width/2)-(width/2)
		property real yVal: (canvas.height/2)-(height/2)
		
		
		id: p2
		x: (canvas.width/2)-(width/2)
		y: (canvas.height/2)-(height/2)
		width: 20
		height: 20
		
		radius:30
		//antialiasing: true
		
		color: Qt.rgba(0, 1, 0.5, 1)
		border.width: 1
		border.color: Qt.darker(color,1.5);
		
		onXChanged: canvas.requestPaint();
		onYChanged: canvas.requestPaint();
		
		MouseArea
		{
            anchors.fill: parent
            drag.target: p2
            drag.minimumX: 0
            drag.maximumX: canvas.width-p2.width
            drag.minimumY: 0
            drag.maximumY: canvas.height-p2.height
        }
	}
}
