#include <QGuiApplication>
#include <QtQuick/QQuickView>
#include "openglscene.h"
#include "fboinsgrenderer.h"

#include <iostream>

using namespace std;

void printHelpInfo()
{
	cout<<"This is MMPEditor, a VMD motion editor using the MikuMikuPenguin library."<<endl
	<<"Usage: MMPEditor [options] model_file.pmx motion_file.vmd"<<endl
	<<"Options: "<<endl
	<<"  --help\tdisplay this help and exit"<<endl
	<<"  --version\toutput version information and exit"<<endl<<endl
	<<"FOR JAPANESE DOCUMENTATION, SEE:"<<endl
	<<"<file://"<<DATA_PATH<<"/doc/README_JP.html>"<<endl<<endl
	<<"Report bugs to <ibenrunnin@gmail.com> (English or Japanese is ok)."<<endl;
}

void printVersionInfo()
{
	cout<<"MMPEditor v1.0"<<endl
	<<"Copyright (C) 2014 Ben Clapp (sn0w75)"<<endl
     <<"This is free software; see the source for copying conditions.  There is NO"<<endl
     <<"warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE."<<endl
     <<"Written by Ben Clapp (sn0w75), <ibenrunnin@gmail.com>."<<endl<<endl;
}

int main(int argc, char **argv)
{
    string model_file="";
	string motion_file="";
	
	int count=0;
	if(argc > 1)
	{
	  for (count = 1; count < argc; count++)
		{
			if(string(argv[count]) == "--help")
			{
				printHelpInfo();
				return 0;
			}
			else if(string(argv[count]) == "--version")
			{
				printVersionInfo();
				return 0;
			}
			else
			{
				string str=string(argv[count]);
				if(str.substr(str.size()-4)==".pmx")
				{
					model_file=str;
				}
				else if(str.substr(str.size()-4)==".vmd")
				{
					motion_file=str;
				}
			}
		}
	}
	else
	{
		printHelpInfo();
		return 0;
	}
	
	if(model_file=="")
	{
		cout<<"Did not specify a model file!"<<endl<<endl;
		printHelpInfo();
		return 0;
	}
	else if(motion_file=="")
	{
		cout<<"Did not specify a motion file!"<<endl<<endl;
		printHelpInfo();
		return 0;
	}
    
    
    
    
    
    QGuiApplication app(argc, argv);

    qmlRegisterType<OpenGLScene>("OpenGLUnderQML", 1, 0, "OpenGLScene");
	qmlRegisterType<FboInSGRenderer>("SceneGraphRendering", 1, 0, "Renderer");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl::fromLocalFile(DATA_PATH"/qml/main.qml"));
    view.show();

    return app.exec();
}
