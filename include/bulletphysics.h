#ifndef BULLET_PHYSICS_H
#define BULLET_PHYSICS_H

#include "btBulletDynamicsCommon.h"

#include <vector>
#include <string>
#include <fstream>

#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "bulletdebugdrawer.h"

/*!  \class BulletPhysics
 * \if ENGLISH
 * \brief A class for handling a Rigid Body physics environment using Bullet.
 * 
 * Detailed description starts here(BulletPhysics).
 * \endif
 * 
 * \if JAPANESE
 * \brief 剛体環境をBulletで処理する為のクラス。
 * 
 * 詳細はここに（BulletPhysics）。
 * \endif
*/
class BulletPhysics
{
	public:
	BulletPhysics(GLuint shaderProgram=0);
	~BulletPhysics();
	
	
	/*! \if ENGLISH \brief Creates a Rigid Body Box.
	 * 
	 * When mass is 0 and kinematic is false, the box created becomes a static rigid body. \n
	 * When mass is 0 and kinematic is true, the box can be moved manually, but it becomes a kinematic rigid body that isn't effected by physics calculation.
	 * \param width The box's width (x-length).
	 * \param height The box's height (y-length).
	 * \param depth The box's depth (z-length)
	 * \param world A pointer to the 
	 * \endif
	 * \if JAPANESE \brief 剛体箱を作る関数。
	 * massが0、kinematicがfalseだった場合、箱がstatic剛体になる。 \n
	 * massが0、kinematicがfalseだった場合、箱が手動で動かせるのだが、物理演算の影響を受けないKinematic剛体になる。
	 * \param width 箱の幅さ（X軸の長さ）。
	 * \param height 箱の高さ（Y軸の長さ）。
	 * \param depth 箱の深さ（Z軸の長さ）。
	*/
	btRigidBody* CreateBox(float width, float height, float depth, const glm::mat4* world, 
		float mass = 0, float restitution = 0, float friction = 0.5f, float linear_damp = 0, float angular_damp = 0, bool kinematic = 0, unsigned short group = 1, unsigned short mask = 0xFFFF);
	btRigidBody* CreateSphere(float radius, const glm::mat4* world, 
		float mass = 0, float restitution = 0, float friction = 0.5f, float linear_damp = 0, float angular_damp = 0, bool kinematic = 0, unsigned short group = 1, unsigned short mask = 0xFFFF);
	btRigidBody* CreateCylinder(float radius, float length, const glm::mat4* world,	//The center axis is the Z-axis
		float mass = 0, float restitution = 0, float friction = 0.5f, float linear_damp = 0, float angular_damp = 0, bool kinematic = 0, unsigned short group = 1, unsigned short mask = 0xFFFF);
	btRigidBody* CreateCapsule(float radius, float height, const glm::mat4* world,	//The center axis is the Z-axis. The height is the distance to the sphere's center.
		float mass = 0, float restitution = 0, float friction = 0.5f, float linear_damp = 0, float angular_damp = 0, bool kinematic = 0, unsigned short group = 1, unsigned short mask = 0xFFFF);
		
	void MoveRigidBody(btRigidBody* body, const glm::mat4* world);
	
	// 拘束条件追加
	void AddPointToPointConstraint(btRigidBody* body, const glm::vec3& pivot);
	void AddPointToPointConstraint(btRigidBody* bodyA, btRigidBody* bodyB, const glm::vec3& pivotInA, const glm::vec3& pivotInB);
	
	/*! \if ENGLISH \brief Add a 6-axis joint.
	 * 
	 * @param bodyA Rigid Body A
	 * @param bodyB Rigid Body B
	 * @param frameInA The joint's World Transformation Matrix (In Rigid Body A Local Coordinate Form)
	 * @param frameInB The joint's World Transformation Matrix (In Rigid Body B Local Coordinate form)
	 * @param c_p1 Movement limiter 1
	 * @param c_p2 Movement limiter 2
	 * @param c_r1 Rotation limiter 1
	 * @param c_r2 Rotation limiter 2
	 * @param stiffness Spring stiffness (Translation x, y, z, Rotation x, y, z, a total of 6 elements) \endif
	*/
	/// \if JAPANESE \brief 6軸ジョイントを追加
	/// @param bodyA 剛体A
	/// @param bodyB 剛体B
	/// @param frameInA ジョイントのワールド変換行列(剛体Aローカル座標系)
	/// @param frameInB ジョイントのワールド変換行列(剛体Bローカル座標系)
	/// @param c_p1 移動制限1
	/// @param c_p2 移動制限2
	/// @param c_r1 回転制限1
	/// @param c_r2 回転制限2
	/// @param stiffness バネ剛性(平行移動x, y, z, 回転移動x, y, zの順の6要素) \endif
	void Add6DofSpringConstraint(btRigidBody* bodyA, btRigidBody* bodyB, const glm::mat4& frameInA, const glm::mat4& frameInB,
								const glm::vec3& c_p1, const glm::vec3& c_p2, const glm::vec3& c_r1, const glm::vec3& c_r2, std::vector<float> stiffness);
	void Add6DofSpringConstraint(btRigidBody* bodyB, const glm::mat4& frameInBDX,
								const glm::vec3& c_p1, const glm::vec3& c_p2, const glm::vec3& c_r1, const glm::vec3& c_r2, std::vector<float> stiffness);
	
	//Step simulation 1/60s forward
	void StepSimulation();
	
	void setDebugMode(int mode);
	void DebugDrawWorld();

	glm::mat4 GetWorld(btRigidBody* body); //Get the resulting World Matrix
	
	
	DebugDrawer *debugDrawer;
	private:
	btDefaultCollisionConfiguration *collisionConfiguration;
	btCollisionDispatcher *dispatcher;
	btBroadphaseInterface *overlappingPairCache;
	btSequentialImpulseConstraintSolver *solver;
	btDiscreteDynamicsWorld *dynamicsWorld;
	
	//List of rigid body shapes
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	
	
	btRigidBody* CreateShape(btCollisionShape* shape, const glm::mat4* world,
	float mass, float restitution, float friction, float linear_damp, float angular_damp, bool kinematic, unsigned short group, unsigned short mask);
	
	btVector3 ConvertVectorGLToBT(const glm::vec3&);
	glm::vec3 ConvertVectorBTToGL(const btVector3&);
	btTransform ConvertMatrixGLToBT(const glm::mat4&);
	glm::mat4 ConvertMatrixBTToGL(const btTransform&);
};


#endif
