#pragma once

#include "StateVec.h"

namespace space_utils::orbit
{
	using math::operator "" _FL;
	using math::Float;
	using math::Vec3;

	// TODO : orbit utility
	struct OrbitState
	{
		Float h{};  // specific angular momentum
		Float i{};  // inclination
		Float ra{}; // right ascension of the ascending node
		Float e{};  // eccentricity
		Float ap{}; // argument of periapsis
		Float ta{}; // true anomaly

		Float a{};  // apoapsis
		Float p{};  // periapsis
		Float ea{}; // eccentric anomaly
		Float n{};  // mean motion
		Float t{};  // time since periapsis
		Float T{};  // orbit period
	};

	class Orbit
	{
	public:
		Orbit(const StateVec& state, Float mu);

		Orbit() = default;

		Orbit(const Orbit&) = default;
		Orbit(Orbit&&)      = default;

		~Orbit() = default;

		Orbit& operator = (const Orbit&) = default;
		Orbit& operator = (Orbit&&)      = default;


	public:
		void setFromParameters(Float c, Float i, Float ra, Float e, Float ap, Float ta, Float mu);

		void setFromState(const StateVec& state, Float mu);


		StateVec stateFromDeltaAngle(Float delta) const;

		StateVec stateFromAngle(Float angle) const;

		// TODO
		//StateVec stateFromTime(Float time) const;


	public:
		const OrbitState& getOrbit() const;

		const StateVec& getState() const;

		Float getMu() const;


	private:
		void updateSpecificParameters();


	private:
		OrbitState m_orbit{}; // orbital parameters
		StateVec   m_state{}; // state parameters

		Float m_mu{}; // extern(planet mu)
	};
}

//#pragma once
//
//namespace components
//{
//	struct OrbitComponent : public IComponent
//	{
//	public:
//		struct R_V
//		{
//			Vec3 radius;
//			Vec3 velocity;
//		};
//
//
//	public:
//		OrbitComponent(
//			IComponent* parent
//			, const PhysicsComponentWeak& planet
//			, const PhysicsComponentWeak& satellite
//			);
//
//		OrbitComponent(const OrbitComponent& comp) = default;
//		OrbitComponent(OrbitComponent&& comp)      = default;
//
//		virtual ~OrbitComponent() = default;
//
//		OrbitComponent& operator = (const OrbitComponent& comp) = default;
//		OrbitComponent& operator = (OrbitComponent&& comp)      = default;
//
//
//	public:
//		virtual const Type& componentType() const override;
//
//
//	public:
//		R_V orbitStateAngle(double theta) const;
//
//		R_V orbitStateTime(const Time& dt) const;
//
//
//		void updateOrbit();
//
//		void setOrbit(double h, double e, double i, double Omega, double omega, double thetha);
//
//
//		//get
//		double angularMomentum() const
//		{
//			return mC;
//		}
//
//		double inclination() const
//		{
//			return mI;
//		}
//
//		double rightAscension() const
//		{
//			return mRA;
//		}
//
//		double eccentricity() const
//		{
//			return mE;
//		}
//
//		double argumentOfPeriapsis() const
//		{
//			return mAP;
//		}
//
//		double trueAnomaly() const
//		{
//			return mTA;
//		}
//
//
//		Vec3 angularMomentumV() const
//		{
//			return mCv;
//		}
//
//		Vec3 apsisLineV() const
//		{
//			return mEv;
//		}
//
//		Vec3 nodeLineV() const
//		{
//			return mNv;
//		}
//
//
//		double energy() const
//		{
//			return mH;
//		}
//
//		double semi_latusRectum() const
//		{
//			return mP;
//		}
//
//		double mu() const
//		{
//			return mMu;
//		}
//
//		double meanMotion() const
//		{
//			return mN;
//		}
//
//
//
//		double period() const
//		{
//			return mOP;
//		}
//
//		double apoapsis() const
//		{
//			return mA;
//		}
//
//		double eccentricAnomaly() const
//		{
//			return mEA;
//		}
//
//		double time() const
//		{
//			return mT;
//		}
//
//
//	private:
//		void updateSpecificParams();
//
//
//	private:
//		PhysicsComponentWeak mSatellitePhys;
//		PhysicsComponentWeak mPlanetPhys;
//
//		//main
//		double mC;   // specific Angular Momentum(length)
//		double mI;   // Inclination
//		double mRA;  // right ascension of the Ascending Node
//		double mE;   // Eccentricity
//		double mAP;  // Argument of Periapsis
//		double mTA;  // True Anomaly
//
//					 //misc
//		Vec3 mCv;   // specific Angular Momentum
//		Vec3 mEv;   // apsis line
//		Vec3 mNv;   // node line
//
//		double mH;   // energy integral
//		double mP;   // semi-latus rectum
//		double mMu;  // mu param
//
//					 //specific
//		double mN;   // mean motion
//		double mOP;  // Orbit Period
//		double mA;   // apoapsis
//		double mEA;  // Eccentric Anomaly
//		double mT;   // time since periapsis	
//	};
//}