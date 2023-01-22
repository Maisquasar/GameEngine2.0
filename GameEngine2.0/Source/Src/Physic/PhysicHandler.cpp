#include "..\..\Include\Physic\PhysicHandler.h"
#include "Include/Core/Components/Collider.h"
#include "Include/App.h"


Physic::PhysicHandler::PhysicHandler()
{

}

Physic::PhysicHandler::~PhysicHandler()
{
	if (_defaultMaterial)
		_defaultMaterial->release();
	if (_scene)
		_scene->release();
	if (_physics)
		_physics->release();
	if (_dispatcher)
		_dispatcher->release();
	if (_pvd)
		_pvd->release();
	if (_transport)
		_transport->release();
	if (_foundation)
		_foundation->release();
}

void Physic::PhysicHandler::BeginPlay()
{
	if (_defaultMaterial)
		_defaultMaterial->release();
	if (_scene)
		_scene->release();
	if (_physics)
		_physics->release();
	if (_dispatcher)
		_dispatcher->release();
	if (_pvd)
		_pvd->release();
	if (_transport)
		_transport->release();
	if (_foundation)
		_foundation->release();
	Initialize();
	for (auto obj : _objects)
	{
		obj->InitializePhysics();
	}
}

void Physic::PhysicHandler::Initialize()
{
	// init PhysX
	_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _defaultAllocatorCallback, _defaultErrorCallback);
	if (!_foundation) throw("PxCreateFoundation failed!");
	_pvd = PxCreatePvd(*_foundation);
	_transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	_pvd->connect(*_transport, physx::PxPvdInstrumentationFlag::eALL);
	//mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(),true, mPvd);
	_toleranceScale.length = 100;        // typical length of an object
	_toleranceScale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice
	_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, _toleranceScale, false, _pvd);
	//mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale);

	physx::PxSceneDesc sceneDesc(_physics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = _dispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	_scene = _physics->createScene(sceneDesc);

	_sceneClient = _scene->getScenePvdClient();
	if (_sceneClient)
	{
		_sceneClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		_sceneClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		_sceneClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	_defaultMaterial = _physics->createMaterial(0.5f, 0.5f, 0.6f);
}

void Physic::PhysicHandler::Update()
{
	if (Application.GetGameState() == GameState::Play) {
		_scene->simulate(1.f / 60.f);
		_scene->fetchResults(true);
	}
}

void Physic::PhysicHandler::AddCollider(Core::Components::Collider* object)
{
	_objects.push_back(object);
}

void Physic::PhysicHandler::RemoveCollider(Core::Components::Collider* object)
{
	for (int i = 0; i < _objects.size(); i++)
	{
		if (_objects[i] == object)
		{
			_objects.erase(_objects.begin() + i);
			break;
		}
	}
}

physx::PxRigidStatic* Physic::PhysicHandler::CreateStaticCube(const Math::Vec3& extent, physx::PxTransform transform)
{
	physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(extent.x, extent.y, extent.z), *_defaultMaterial);
	physx::PxRigidStatic* body = _physics->createRigidStatic(transform);
	body->attachShape(*shape);
	_scene->addActor(*body);
	shape->release();
	return body;
}

physx::PxRigidDynamic* Physic::PhysicHandler::CreateDynamicCube(const Math::Vec3& extent, physx::PxTransform transform, float mass)
{
	physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(extent.x, extent.y, extent.z), *_defaultMaterial);

	physx::PxRigidDynamic* body = _physics->createRigidDynamic(transform);
	body->attachShape(*shape);
	physx::PxRigidBodyExt::updateMassAndInertia(*body, mass);
	_scene->addActor(*body);
	shape->release();
	return body;
}

physx::PxRigidStatic* Physic::PhysicHandler::CreateStaticSphere(float radius, physx::PxTransform transform)
{
	physx::PxShape* shape = _physics->createShape(physx::PxSphereGeometry(radius), *_defaultMaterial);
	physx::PxRigidStatic* body = _physics->createRigidStatic(transform);
	body->attachShape(*shape);
	_scene->addActor(*body);
	shape->release();
	return body;
}

physx::PxRigidDynamic* Physic::PhysicHandler::CreateDynamicSphere(float radius, physx::PxTransform transform, float mass)
{
	physx::PxShape* shape = _physics->createShape(physx::PxSphereGeometry(radius), *_defaultMaterial);
	physx::PxRigidDynamic* body = _physics->createRigidDynamic(transform);
	body->attachShape(*shape);
	_scene->addActor(*body);
	shape->release();
	return body;
}