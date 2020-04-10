#pragma once

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


	// returns tuple of gravitation function and its jacobian
	template<class Scalar, int Order>
	auto gravitationForce(Scalar&& planetMu)
	{
		// equation of motiion is the second order equation
		using traits = func_traits<Scalar, 2 * Order>;

		using Type = typename traits::Type;

		using Time  = typename traits::Time;
		using Value = typename traits::Value;

		using Mat = typename traits::Mat;
		using Vec = typename traits::Vec;

		using Function = typename traits::Function;
		using Jacobian = typename traits::Jacobian;

		// u: packed radius and velocity vectors, (r, v)
		auto equ = [mu = planetMu] (const Time& t, const Value& u)
		{
			
		};

		auto jac = [mu = planetMu] (const Time& t, const Value& u)
		{
			auto d = [] (auto i, auto j)
			{
				return (i == j ? 1 : 0);
			};

			Mat res{};
			for (int i = 0; i < Order; i++)
			{
				res[i][Order + i] = Type(1.0);
			}

			Type sumSquares{};
			for (int i = 0; i < Order; i++)
			{
				sumSquares += u[i] * u[i];
			}

			Type mult = -mu / std::pow(sumSquares, Type(3.0 / 2.0));
			for (int i = 0; i < Order; i++)
			{
				for (int j = 0; j < Order; j++)
				{
					res[Order + i][j] = mult * (d(i, j) - 3 * u[i] * u[j] / sumSquares);
				}
			}

			return res;
		};

		return std::make_tuple(Function(equ), Jacobian(jac));
	}


	//	auto jacob = [&] (const Time& t, const Value& u)
	//	{
	//		
	//	};
	//
	//	return 0;
	//}
	//
	//// returns tuple of perturbed gravitation function and its jacobian 
	//template<
	//	  class Scalar
	//	, int Order
	//	, class Perturbation
	//	, class PerturbationJacobian
	//>
	//auto gravitatationForcePerturbed(
	//	  Scalar&& planetMu
	//	, Perturbation&& perturbation
	//	, PerturbationJacobian&& perturbationJacobian
	//)
	//{
	//	return 0;
	//}
}