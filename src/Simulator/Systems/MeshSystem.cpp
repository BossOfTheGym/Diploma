#include "MeshSystem.h"

#include "Meshes/BoxMesh.h"
#include "Meshes/BoxMeshBuilder.h"
#include "Meshes/QuadMesh.h"
#include "Meshes/QuadMeshBuilder.h"
#include "Meshes/SphereMesh.h"
#include "Meshes/SphereMeshBuilder.h"

#include <ECS/System/SystemManager.h>

#include "GraphicsSystem.h"

namespace sim
{
	using meshes::BoxMeshBuilder;
	using meshes::QuadMeshBuilder;
	using meshes::SphereMeshBuilder;

	using Name = MeshSystem::Name;
	using Id   = MeshSystem::Id;

	using ResourcePtr = MeshSystem::ResourcePtr;


	const Name MeshSystem::CUBE   = "cube";
	const Name MeshSystem::SPHERE = "sphere";
	const Name MeshSystem::QUAD   = "quad";

	const Name MeshSystem::BAD_NAME = "";

	const Id MeshSystem::BAD_ID = ecs::BAD_ID;


	MeshSystem::MeshSystem(ecs::sys::SystemManager* manager)
		: ecs::sys::System<MeshSystem>(manager)
	{
		// TODO : load basic meshes
		auto  graphicsSystem = manager->get<GraphicsSystem>();
		auto& graphicsEngine = graphicsSystem->getGraphicsEngine();

		auto pos = *graphicsEngine.getCommonAttribLocation(POSITION);

		BoxMeshBuilder    boxBuilder({pos});
		QuadMeshBuilder   quadBuilder({pos});
		SphereMeshBuilder sphereBuilder(4, {pos});

		loadMesh(boxBuilder.buildMesh(), MeshSystem::CUBE);
		loadMesh(quadBuilder.buildMesh(), MeshSystem::QUAD);
		loadMesh(sphereBuilder.buildMesh(), MeshSystem::SPHERE);
	}

	void MeshSystem::update(ecs::Float t, ecs::Float dt)
	{}


	bool MeshSystem::loadMesh(RawMesh* meshToAdd, const Name& name)
	{
		if (name != MeshSystem::BAD_NAME)
		{			
			auto id = m_counter++;

			m_meshes[id].reset(meshToAdd);
			m_nameId[name] = id;
			m_idName[id] = name;

			return true;
		}
		return false;
	}


	Id MeshSystem::getIdFromName(const Name& name) const
	{
		if (auto it = m_nameId.find(name); it != m_nameId.end())
		{
			return it->second;
		}
		return MeshSystem::BAD_ID;
	}

	const Name& MeshSystem::getNameFromId(Id id) const
	{
		if (auto it = m_idName.find(id); it != m_idName.end())
		{
			return it->second;
		}
		return MeshSystem::BAD_NAME;
	}

	ResourcePtr MeshSystem::acquireResource(const Name& name) const
	{
		auto id = getIdFromName(name);
		if (id != BAD_ID)
		{
			// if id != BAD_ID then mesh always exists
			if (auto it = m_meshes.find(id); it != m_meshes.end())
			{
				return it->second;
			}
		}
		return {};
	}

	ResourcePtr MeshSystem::acquireResource(Id id) const
	{
		if (auto it = m_meshes.find(id); it != m_meshes.end())
		{
			return it->second;
		}
		return {};
	}
}