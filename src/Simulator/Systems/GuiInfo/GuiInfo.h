#pragma once

#include <ECS/Entity/Entity.h>
#include <Utility/PureType.h>

#include <utility>
#include <memory>

namespace ecs
{
	class SystemManager;
}

namespace sim
{
	using ecs::entity::Entity;
	using ecs::entity::EntityComponentRegistry;

	template<class T>
	struct TypeTag
	{
		using type = T;
	};



	namespace detail
	{
		class ISystemInfo
		{
		public:
			virtual void render() = 0;

			virtual ISystemInfo* copy() = 0;
		};

		template<class InfoClassT, class SystemT>
		class SystemInfo : public ISystemInfo
		{
		public:
			using this_t = SystemInfo<InfoClassT, SystemT>;
			using base_t = ISystemInfo;

			using SystemInfoPtr = ISystemInfo*;

			using InfoClass = InfoClassT;
			using System    = SystemT;

		public:
			template<class ... Args>
			SystemInfo(Args&& ... args) : m_info(std::forward<Args>(args)...)
			{}

			SystemInfo(const SystemInfo&) = default; // TODO : check
			SystemInfo(SystemInfo&&)      = default; // TODO : check

			virtual ~SystemInfo() = default;

			SystemInfo& operator = (const SystemInfo&) = default; // TODO : check
			SystemInfo& operator = (SystemInfo&&)      = default; // TODO : check


		public:
			virtual void render() override
			{
				m_info.render();
			}

			virtual ISystemInfo* copy()
			{
				return new SystemInfo<InfoClass, System>(*this);
			}


		private:
			InfoClass m_info;
		};
	}
	
	class SystemInfo
	{
	public:
		using SystemInfoPtr = std::unique_ptr<detail::ISystemInfo>;


	public:
		template<class Info, class Tag> // Tag : system type tag
		SystemInfo(Info info, Tag)
			: m_infoPtr(new detail::SystemInfo<util::remove_cv_ref_t<Info>, typename Tag::type>(std::forward<Info>(info)))
		{}

		SystemInfo() = default;
		~SystemInfo() = default;

		SystemInfo(const SystemInfo& another)
			: m_infoPtr(another.m_infoPtr->copy())
		{}

		SystemInfo(SystemInfo&& another)
			: m_infoPtr(std::move(another.m_infoPtr))
		{}


		SystemInfo& operator = (const SystemInfo& another)
		{
			m_infoPtr.reset(another.m_infoPtr->copy());

			return *this;
		}

		SystemInfo& operator = (SystemInfo&& another)
		{
			m_infoPtr = std::move(another.m_infoPtr);

			return *this;
		}


	public:	
		void render()
		{
			m_infoPtr->render();
		}


	private:
		SystemInfoPtr m_infoPtr{nullptr};
	};



	namespace detail
	{
		class IComponentInfo
		{
		public:
			virtual bool hasComponent(Entity e) const = 0;

			virtual void render(Entity e) = 0;

			virtual IComponentInfo* copy() = 0;
		};

		template<class InfoClassT, class ComponentT>
		class ComponentInfo : public IComponentInfo
		{
		public:
			using this_t = ComponentInfo<InfoClassT, ComponentT>;
			using base_t = IComponentInfo;

			using InfoClass = InfoClassT;
			using Component = ComponentT;


		public:
			template<class Info>
			ComponentInfo(Info info)
				: m_info(std::forward<Info>(info))
			{}

			ComponentInfo(const ComponentInfo&) = default; // TODO : check
			ComponentInfo(ComponentInfo&&)      = default; // TODO : check

			virtual ~ComponentInfo() = default;

			ComponentInfo& operator = (const ComponentInfo&) = default; // TODO : check
			ComponentInfo& operator = (ComponentInfo&&)      = default; // TODO : check


		public:
			virtual bool hasComponent(Entity e) const override
			{
				return m_info.hasComponent(e);
			}

			virtual void render(Entity entity) override
			{
				return m_info.render(entity);
			}

			virtual IComponentInfo* copy() override
			{
				return new ComponentInfo(*this);
			}

		private:
			InfoClass m_info;
		};
	}

	class ComponentInfo
	{
	public:
		using ComponentInfoPtr = std::unique_ptr<detail::IComponentInfo>;


	public:
		template<class Info, class Tag> // Tag : component type tag
		ComponentInfo(Info info, Tag)
			: m_infoPtr(
				new detail::ComponentInfo<
					util::remove_cv_ref_t<Info>
					, typename Tag::type
				>(std::forward<Info>(info)))
		{}

		ComponentInfo() = default;
		~ComponentInfo() = default;

		ComponentInfo(const ComponentInfo& another)
			: m_infoPtr(another.m_infoPtr->copy())
		{}

		ComponentInfo(ComponentInfo&&)
			: m_infoPtr(std::move(m_infoPtr))
		{}

		ComponentInfo& operator = (const ComponentInfo& another)
		{
			m_infoPtr.reset(another.m_infoPtr->copy());

			return *this;
		}

		ComponentInfo& operator = (ComponentInfo&& another)
		{
			m_infoPtr = std::move(another.m_infoPtr);

			return *this;
		}


	public:
		bool hasComponent(Entity e) const
		{
			return m_infoPtr->hasComponent(e);
		}

		void render(Entity e)
		{
			m_infoPtr->render(e);
		}


	private:
		ComponentInfoPtr m_infoPtr{};
	};
}