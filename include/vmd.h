#ifndef VMD_H
#define VMD_H

#include <vector>
#include <string>
#include <fstream>

#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

#include "interpolation.h"
//VMD related structs

namespace ClosedMMDFormat
{
	struct VMDInfo;
	VMDInfo &readVMD(std::string filename);

	struct VMDBoneFrame
	{
		std::string name; //char[15] before UTF8 conversion
		
		unsigned int frame;
		
		glm::vec3 translation; //values will be 0 when there's no position
		glm::quat rotation; //values will be 0 when there's no position (w will be 1.0)
		
		BezierParameters bezier;
		
		bool operator < (const VMDBoneFrame &k) const
		{
			//Comparison by frame number
			return frame < k.frame;
		}
	};

	struct VMDMorphFrame
	{
		std::string name; //char[15] before UTF8 conversion
		unsigned int frame;
		float value;
		
		bool operator < (const VMDMorphFrame &k) const
		{
			//Comparison by frame number
			return frame < k.frame;
		}
	};

	struct VMDCameraFrame
	{
		unsigned int frame;
		
		float distance; //Distance between the camera and pt of interest (the camera is negative when in front of the object)
		glm::vec3 position;
		glm::vec3 rotation;
		
		std::string interpolationParameters; //char[24] before UTF8 conversion
		
		unsigned int viewAngle; //viewing angle in degrees
		bool perspective; //0:ON 1:OFF
	};

	struct VMDLightFrame
	{
	};

	struct VMDSelfShadowFrame
	{
	};
	
	
	/*!  \class VMDInfo
	 * \if ENGLISH
	 * \brief Class for loading/storing info about a VMD motion file.
	 * 
	 * \endif
	 * 
	 * \if JAPANESE
	 * \brief VMDモーションを読み込む／格納する為のクラス。
	 * 
	* \endif
	*/
	struct VMDInfo
	{
		char headerStr[30];
		std::string modelName;
		
		//Bone Frames
		int boneCount; //number of continuing bone datasets
		VMDBoneFrame *boneFrames;
		
		//Morph Frames (Emotion data)
		int morphCount;
		VMDMorphFrame *morphFrames;
		
		int cameraCount;
		VMDCameraFrame *cameraFrames;
		
		int lightCount;
		VMDLightFrame *lightFrames;
		
		int selfShadowCount;
		VMDSelfShadowFrame *selfShadowFrames;
	};
}

#endif
