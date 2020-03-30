#pragma once

namespace space_utils::orbit
{
	// TODO : orbit utility
	struct OrbitalParameters
	{

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