#pragma once

#include <ECS/ecs_common.h>
#include <ECS/System/System.h>

#include <Mesh/IMesh.h>
#include <Mesh/IMeshBuilder.h>

#include <map>
#include <memory>
#include <string>

#include "../Components/MeshComponent.h"

namespace sim
{
	class MeshSystem final : public ecs::sys::System<MeshSystem>
	{
	public:
		using Name = std::string;
		using Id   = comp::Id;

		using RawMesh      = mesh::IMesh; 
		using IMeshBuilder = mesh::IMeshBuilder;
		using MeshPtr      = std::shared_ptr<RawMesh>;
		using ResourcePtr  = std::weak_ptr<RawMesh>; 

		using Meshes = std::map<Id, MeshPtr>;
		using NameId = std::map<Name, Id>;
		using IdName = std::map<Id, Name>;


		static const Name CUBE;
		static const Name SPHERE;
		static const Name QUAD;

		static const Name BAD_NAME;

		static const Id BAD_ID;

	public:
		MeshSystem(ecs::sys::SystemManager* manager);

		virtual ~MeshSystem() = default;


	public:
		bool loadMesh(RawMesh* meshToAdd, const Name& name);


		Id getIdFromName(const Name& name) const;

		const Name& getNameFromId(Id id) const;


		ResourcePtr acquireResource(const Name& name) const;

		ResourcePtr acquireResource(Id id) const;


	private:
		Meshes m_meshes{};
		NameId m_nameId{};
		IdName m_idName{};

		Id m_counter{};
	};
}