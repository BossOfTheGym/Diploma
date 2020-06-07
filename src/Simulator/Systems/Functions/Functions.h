#pragma once

#include <ECS/ecs_engine.h>
#include <ECS/Entity/Entity.h>

#include <Numerics/Arg/ArgN.h>
#include <Math/MathLib.h>

#include <Function/FunctionHolder.h>

namespace funcs
{
	template<class Scalar, int Dim>
	struct func_traits
	{
		using Type = std::remove_reference_t<std::remove_cv_t<Scalar>>;

		using Vec = Num::Arg::VecN<Type, Dim>;
		using Mat = Num::Arg::MatNxM<Type, Dim, Dim>;

		using Time  = Type;
		using Value = Vec;

		using Function = func::FunctionHolder<Vec(const Time&, const Value&)>;
		using Jacobian = func::FunctionHolder<Mat(const Time&, const Value&)>;
	};

	template<class Scalar, int Dim>
	using func_traits_mat = typename func_traits<Scalar, Dim>::Mat;

	template<class Scalar, int Dim>
	using func_traits_val = typename func_traits<Scalar, Dim>::Value;

	template<class Scalar, int Dim>
	using func_traits_time = typename func_traits<Scalar, Dim>::Time;

	template<class Scalar, int Dim>
	using func_type = func_traits_val<Scalar, Dim>(const func_traits_time<Scalar, Dim>&, const func_traits_val<Scalar, Dim>&);

	template<class Scalar, int Dim>
	using FunctionInterface = func::IFunction<func_type<Scalar, Dim>>;

	template<class Scalar, int Dim>
	using jacob_type = func_traits_mat<Scalar, Dim>(const func_traits_time<Scalar, Dim>&, const func_traits_val<Scalar, Dim>&);

	template<class Scalar, int Dim>
	using JacobianInterface = func::IFunction<jacob_type<Scalar, Dim>>;


	template<class Scalar, int Dim>
	class Gravitation : public FunctionInterface<Scalar, 2 * Dim>
	{
	public:
		using this_t = Gravitation;
		using base_t = FunctionInterface<Scalar, 2 * Dim>;

		using Time  = func_traits_time<Scalar, 2 * Dim>;
		using Value = func_traits_val<Scalar, 2 * Dim>; 

		using Pointer = typename base_t::Pointer;


	public:
		Gravitation(Scalar mu = static_cast<Scalar>(0)) : m_mu(mu)
		{}

		Gravitation(const Gravitation&) = default;
		Gravitation(Gravitation&&)      = default;

		virtual ~Gravitation() = default;

		Gravitation& operator = (const Gravitation&) = default;
		Gravitation& operator = (Gravitation&&)      = default;


	public: // ICopy
		virtual Pointer copy() const override
		{
			return Pointer(new Gravitation(*this));
		}


	public: // IFunction
		virtual Value operator() (const Time& t, const Value& u) override
		{
			Value res{};
			for (int i = 0; i < Dim; i++)
			{
				res[i] = u[Dim + i];
			}

			Scalar sumSquares{};
			for (int i = 0; i < Dim; i++)
			{
				sumSquares += u[i] * u[i];
			}

			Scalar mult = -m_mu / std::pow(sumSquares, Scalar(3.0 / 2.0));
			for (int i = 0; i < Dim; i++)
			{
				res[Dim + i] = mult * u[i];
			}

			return res;
		}

		virtual Value operator() (const Time& t, const Value& value) const override
		{
			return const_cast<Gravitation&>(*this)(t, value);
		}


	public:
		Scalar getMu() const
		{
			return m_mu;
		}

		void setMu(Scalar mu)
		{
			m_mu = mu;
		}


	private:
		Scalar m_mu{};
	};

	template<class Scalar, int Dim>
	class GravitationJacobian : public JacobianInterface<Scalar, 2 * Dim>
	{
	public:
	public:
		using this_t = GravitationJacobian;
		using base_t = JacobianInterface<Scalar, 2 * Dim>;

		using Time  = func_traits_time<Scalar, 2 * Dim>;
		using Value = func_traits_val<Scalar, 2 * Dim>; 
		using Mat   = func_traits_mat<Scalar, 2 * Dim>;

		using Pointer = typename base_t::Pointer;


	public:
		GravitationJacobian(Scalar mu = static_cast<Scalar>(0)) : m_mu(mu)
		{}

		GravitationJacobian(const GravitationJacobian&) = default;
		GravitationJacobian(GravitationJacobian&&)      = default;

		virtual ~GravitationJacobian() = default;

		GravitationJacobian& operator = (const GravitationJacobian&) = default;
		GravitationJacobian& operator = (GravitationJacobian&&)      = default;


	public: // ICopy
		virtual Pointer copy() const override
		{
			return Pointer(new GravitationJacobian(*this));
		}


	public: // IFunction
		virtual Mat operator() (const Time& t, const Value& u) override
		{
			auto d = [] (auto i, auto j)
			{
				return (i == j ? 1 : 0);
			};

			Mat res{};
			for (int i = 0; i < Dim; i++)
			{
				res[i][Dim + i] = Scalar(1.0);
			}

			Scalar sumSquares{};
			for (int i = 0; i < Dim; i++)
			{
				sumSquares += u[i] * u[i];
			}

			Scalar mult = -m_mu / std::pow(sumSquares, Scalar(3.0 / 2.0));
			for (int i = 0; i < Dim; i++)
			{
				for (int j = 0; j < Dim; j++)
				{
					res[Dim + i][j] = mult * (d(i, j) - 3 * u[i] * u[j] / sumSquares);
				}
			}

			return res;
		}

		virtual Mat operator() (const Time& t, const Value& u) const override
		{
			return const_cast<GravitationJacobian&>(*this)(t, u);
		}


	public:
		Scalar getMu() const
		{
			return m_mu;
		}

		void setMu(Scalar mu)
		{
			m_mu = mu;
		}


	private:
		Scalar m_mu{};
	};


	using math::Vec3;
	using math::Float;

	using ecs::entity::Entity;
	using ecs::entity::null;

	template<class Scalar, int Dim>
	class GravitationPerturbed : public FunctionInterface<Scalar, 2 * Dim>
	{
		static_assert(Dim == 3, "Dim != 3, dim should be equal to 3");

	public:
		using this_t = Gravitation;
		using base_t = FunctionInterface<Scalar, 2 * Dim>;

		using Time  = func_traits_time<Scalar, 2 * Dim>;
		using Value = func_traits_val<Scalar, 2 * Dim>; 

		using Pointer = typename base_t::Pointer;


	public:
		GravitationPerturbed()
		{}

		GravitationPerturbed(const GravitationPerturbed&) = default;
		GravitationPerturbed(GravitationPerturbed&&)      = default;

		virtual ~GravitationPerturbed() = default;

		GravitationPerturbed& operator = (const GravitationPerturbed&) = default;
		GravitationPerturbed& operator = (GravitationPerturbed&&)      = default;


	public: // ICopy
		virtual Pointer copy() const override
		{
			return Pointer(new GravitationPerturbed(*this));
		}


	public: // IFunction
		virtual Value operator() (const Time& t, const Value& u) override
		{
			if (m_needUpdate)
			{
				updatePerturbationForce();
			}

			Vec3 r{u[0], u[1], u[2]};
			Vec3 v{u[3], u[4], u[5]};
			Vec3 h{};

			r = glm::normalize(r);
			v = glm::normalize(v);
			h = glm::normalize(glm::cross(r, v));
			v = glm::normalize(glm::cross(h, r));

			Vec3 force = r * m_pr + v * m_pt + h * m_ph;

			Value res{};
			for (int i = 0; i < Dim; i++)
			{
				res[i] = u[Dim + i];
			}

			Scalar sumSquares{};
			for (int i = 0; i < Dim; i++)
			{
				sumSquares += u[i] * u[i];
			}

			Scalar mult = -m_mu / std::pow(sumSquares, Scalar(3.0 / 2.0));
			for (int i = 0; i < Dim; i++)
			{
				res[Dim + i] = mult * u[i] + force[i];
			}

			return res;
		}

		virtual Value operator() (const Time& t, const Value& value) const override
		{
			return const_cast<GravitationPerturbed&>(*this)(t, value);
		}


	public:
		void setMu(Float mu)
		{
			m_mu = mu;
			m_needUpdate = true;
		}

		void setJ2(Float J2)
		{
			m_J2 = J2;
			m_needUpdate = true;
		}

		void setI(Float i)
		{
			m_i = i;
			m_needUpdate = true;
		}

		void setTA(Float ta)
		{
			m_ta = ta;
			m_needUpdate = true;
		}

		void setAP(Float ap)
		{
			m_ap = ap;
			m_needUpdate = true;
		}

		void setR(Float R)
		{
			m_R = R;
			m_needUpdate = true;
		}

		void setr(Float r)
		{
			m_r = r;
			m_needUpdate = true;
		}


	private:
		void updatePerturbationForce() const 
		{
			auto coef = -m_mu / (m_r * m_r) * m_J2 * 1.5 * (m_R / m_r) * (m_R / m_r);

			auto sini  = std::sin(m_i);
			auto sin2i = std::sin(2 * m_i);
			auto sinTaAp  = std::sin(m_ta + m_ap);
			auto sin2TaAp = std::sin(2 * (m_ta + m_ap));

			m_pr = coef * (static_cast<Float>(1.0) -3 * sini * sini * sinTaAp * sinTaAp);
			m_pt = coef * sini * sini * sin2TaAp;
			m_ph = coef * sin2i * sinTaAp;

			m_needUpdate = false;
		}

	private:
		mutable bool m_needUpdate{false};

		Float m_mu{};
		Float m_R{};
		Float m_J2{};

		Float m_r{};

		Float m_i{};  // inclination
		Float m_ta{}; // true anomaly
		Float m_ap{}; // argument of perigee

		mutable Float m_pr{}; // radial
		mutable Float m_pt{}; // transverse
		mutable Float m_ph{}; // normal
	};
}