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
	if (_foundation)
		_foundation->release();
}

void Physic::PhysicHandler::BeginPlay()
{
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
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
	//mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(),true, mPvd);
	_toleranceScale.length = 100;        // typical length of an object
	_toleranceScale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice
	_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, _toleranceScale, true, _pvd);
	//mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale);

	physx::PxSceneDesc sceneDesc(_physics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = _dispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	_scene = _physics->createScene(sceneDesc);

	physx::PxPvdSceneClient* pvdClient = _scene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	_defaultMaterial = _physics->createMaterial(0.5f, 0.5f, 0.6f);

	physx::PxRigidStatic* groundPlane = PxCreatePlane(*_physics, physx::PxPlane(0, 1, 0, 50), *_defaultMaterial);
	_scene->addActor(*groundPlane);
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

physx::PxRigidDynamic* Physic::PhysicHandler::CreateCube(const Math::Vec3& extent, physx::PxTransform transform, float mass)
{
	auto size = extent;
	physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(size.x, size.y, size.z), *_defaultMaterial);
	physx::PxRigidDynamic* body = _physics->createRigidDynamic(transform);
	body->attachShape(*shape);
	physx::PxRigidBodyExt::updateMassAndInertia(*body, mass);
	_scene->addActor(*body);
	shape->release();
	return body;
}

