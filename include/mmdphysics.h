#ifndef MMD_PHYSICS_H
#define MMD_PHYSICS_H

#include "bulletphysics.h"
#include "pmx.h"
#include "vmd.h"
#include "motioncontroller.h"

#include <vector>
#include <string>
#include <fstream>

#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
/*!  \class MMDPhysics
 * \if ENGLISH
 * \brief A class for physics calculations on MMD rigid bodies.
 * 
 * Detailed description starts here(MMDPhysics).
 * \endif
 * 
 * \if JAPANESE
 * \brief MMD剛体に物理演算を行う為のクラス。
 * 
 * 詳細はここに（MMDPhysics）。
 * \endif
 */
class MMDPhysics
{
	public:
	
	MMDPhysics(ClosedMMDFormat::PMXInfo &pmxInfo, VMDMotionController *motionController, BulletPhysics* bulletPhysics);
	
	glm::mat4 createRigidMatrix(glm::vec3 &pos, glm::vec3 &rot, int &i);
	
	void createRigidBody();
	void createJoints();
	void updateBones(bool physicsEnabled);
	
	
	std::vector<int> rigidMeshIndices;
	std::vector<std::vector<glm::vec3> > rigidMeshes;
	
	std::vector<std::vector<glm::vec3> > jointMeshes;
	
	std::vector<btRigidBody*> rigidBodies;
	
	private:
	BulletPhysics* bulletPhysics;
	VMDMotionController *motionController;
	ClosedMMDFormat::PMXInfo &pmxInfo;
};


#endif
