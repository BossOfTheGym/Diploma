#include "OrbitUtil.h"

namespace space_utils::orbit
{
	Orbit::Orbit(const StateVec& state, Float mu)
	{
		setFromState(state, mu);
	}


	void Orbit::setFromParameters(Float h, Float i, Float ra, Float e, Float ap, Float ta, Float mu)
	{
		m_orbit = OrbitState{h, i, ra, e, ap, ta};
		m_mu    = mu;

		Float cost = cos(ta);
		Float sint = sin(ta);
		
		Float coso = cos(ap);
		Float sino = sin(ap);
		Float cosO = cos(ra);
		Float sinO = sin(ra);
		Float cosi = cos(i);
		Float sini = sin(i);
		
		Vec3 rx = h *  h / mu / (1 + e * cost) * Vec3(cost, sint, 0.0_FL);
		Vec3 vx = mu / h * Vec3(-sint, e + cost, 0.0_FL);
		
		Mat3 Qxx = transpose(
			Mat3(
				  cosO * coso - sinO * sino * cosi, -cosO * sino - sinO * cosi * coso, +sinO * sini
				, sinO * coso + cosO * cosi * sino, -sinO * sino + cosO * cosi * coso, -cosO * sini
				,        sini * sino              ,         sini * coso              ,      cosi
			)
		);
		
		m_state = StateVec{Qxx * rx, Qxx * vx};

		updateSpecificParameters();
	}

	void Orbit::setFromState(const StateVec& state, Float mu)
	{
		m_state = state;
		m_mu = mu;

		//1.
		Vec3 rv = state.r;
		Float r = glm::length(rv);
		
		//2.
		Vec3 vv = state.v;
		Float v = glm::length(vv);
		auto energy = glm::dot(vv, vv) - 2 * mu / r;
		
		//3.
		Float vr = glm::dot(rv / r, vv);
		
		//4, 5
		auto hv = glm::cross(rv, vv);
		m_orbit.h = glm::length(hv);	
		
		//6.
		m_orbit.i = std::acos(hv.z / m_orbit.h);
		
		//7, 8
		auto nv = glm::cross(Vec3(0.0, 0.0, 1.0), hv);
		m_orbit.n = glm::length(nv);
		if (m_orbit.n <= std::numeric_limits<Float>::epsilon())
		{
			// TODO
			//nv = Vec3(1.0, 0.0, 0.0);
		}

		//9.
		m_orbit.ra = std::acos(nv.x / m_orbit.n);
		if(nv.y <= std::numeric_limits<Float>::epsilon())
		{
			m_orbit.ra = math::PI2 - m_orbit.ra;
		}
		
		
		//10, 11
		auto ev = glm::cross(vv, hv) / mu - rv / r;
		m_orbit.e = glm::length(ev);
		
		Vec3 uev = (m_orbit.e > std::numeric_limits<Float>::epsilon() ? ev / m_orbit.e : Vec3(0.0));
		
		//12.
		m_orbit.ap = std::acos(glm::dot((nv / m_orbit.n), uev));
		if (ev.z <= std::numeric_limits<Float>::epsilon())
		{
			m_orbit.ap = math::PI2 - m_orbit.ap;
		}
		
		//13.
		m_orbit.ta = std::acos(glm::dot(rv / r, uev));
		if(vr <= std::numeric_limits<Float>::epsilon())
		{
			m_orbit.ta = math::PI2 - m_orbit.ta;
		}	

		updateSpecificParameters();
	}


	StateVec Orbit::stateFromDeltaAngle(Float deltaAngle) const
	{
		if (std::abs(deltaAngle) <= std::numeric_limits<Float>::epsilon())
		{
			return m_state;
		}

		auto h  = m_orbit.h;
		auto mu = m_orbit.h;

		auto r0 = glm::length(m_state.r);
		auto v0 = glm::length(m_state.v);

		auto vr0 = glm::dot(m_state.v,  m_state.r / r0);

		auto p = h * h / mu;

		auto cosdT = std::cos(deltaAngle);
		auto sindT = std::sin(deltaAngle);

		auto r = p / (1 + (p / r0 - 1) * cosdT - h * vr0 * sindT / mu);

		auto f  = 1 - r / p * (1 - cosdT);
		auto g  = r * r0 / h * sindT;
		auto fd = mu / h * (1 - cosdT) / sindT * ((1 - cosdT) / p - 1 / r0 - 1 / r);
		auto gd = 1 - r0 / p * (1 - cosdT);

		return StateVec{f * m_state.r + g * m_state.v, fd * m_state.r + gd * m_state.v};
	}

	StateVec Orbit::stateFromAngle(Float angle) const
	{		
		Float deltaAngle = std::fmod(angle, math::PI2) - m_orbit.ta;

		return stateFromDeltaAngle(deltaAngle);
	}

	// TODO  
	//StateVec Orbit::stateFromTime(Float time) const{}

	const OrbitState& Orbit::getOrbit() const
	{
		return m_orbit;
	}

	const StateVec& Orbit::getState() const
	{
		return m_state;
	}

	Float Orbit::getMu() const
	{
		return m_mu;
	}


	// private
	void Orbit::updateSpecificParameters()
	{
		auto& h  = m_orbit.h;
		auto& e  = m_orbit.e;
		auto& ta = m_orbit.ta;

		auto& a  = m_orbit.a;
		auto& p  = m_orbit.p;
		auto& ea = m_orbit.ea;
		auto& n  = m_orbit.n;
		auto& t  = m_orbit.t;
		auto& T  = m_orbit.T;

		if (e < 1.0_FL) //elliptic
		{	
			a = (h * h / m_mu) / (1 - e * e);
			p = (h * h / m_mu) / (1 + e);

			ea = 2 * std::atan(std::sqrt((1 - e) / (1 + e)) * std::tan(ta / 2));
			if (ta > math::PI)
			{
				ea += math::PI2;
			}
		
			n = std::sqrt(m_mu) / std::pow(a, 1.5_FL);
		
			t = 1.0_FL / n * (ea - e * std::sin(ea));
			T = 1.0_FL / n * math::PI2;
		}
		else if (e > 1.0_FL) // hyperbolic
		{
			a = (h * h / m_mu) / (e * e - 1);
			p = 0.0;

			ea = 2 * std::atanh(std::sqrt((e - 1) / (e + 1)) * std::tanh(ta / 2));
		
			n = std::sqrt(m_mu) / std::pow(a, 1.5_FL);
		
			t = 1.0_FL / n * (e * std::sinh(ea) - ea);
			T = std::numeric_limits<Float>::infinity();
		}
		else // parabolic
		{
			// still TODO
			/*a = std::numeric_limits<Float>::infinity();
			p = (h * h / m_mu) / 2;

			n = 0.0_FL;

			T = std::numeric_limits<Float>::infinity();*/
			auto inf = std::numeric_limits<Float>::infinity();
			m_orbit = OrbitState{
				  inf, inf, inf, inf, inf, inf
				, inf, inf, inf, inf, inf, inf
			};
		}
	}
}

//namespace components
//{
//	OrbitComponent::OrbitComponent(
//		IComponent* parent
//		, const PhysicsComponentWeak& planet
//		, const PhysicsComponentWeak& satellite
//		)
//		: IComponent(parent)
//
//		, mPlanetPhys(planet)
//		, mSatellitePhys(satellite)
//
//		, mC(0.0f)
//		, mI(0.0f)
//		, mRA(0.0f)
//		, mE(0.0f)
//		, mAP(0.0f)
//		, mTA(0.0f)
//
//		, mP(0.0f)
//		, mMu(0.0f)
//
//		, mOP(0.0f)
//		, mEA(0.0f)
//		, mT(0.0f)
//	{
//		updateOrbit();
//	}
//
//	const IComponent::Type& OrbitComponent::componentType() const
//	{
//		static const Type type = "Orbit";
//
//		return type;
//	}
//
//
//	void OrbitComponent::setOrbit(double h, double e, double i, double Omega, double omega, double theta)
//	{
//		using glm::transpose;
//		using glm::cross;
//		using glm::length;
//
//
//		double cost = cos(theta);
//		double sint = sin(theta);
//
//		double coso = cos(omega);
//		double sino = sin(omega);
//		double cosO = cos(Omega);
//		double sinO = sin(Omega);
//		double cosi = cos(i);
//		double sini = sin(i);
//
//		auto planet = mPlanetPhys.lock();
//		auto sat = mSatellitePhys.lock();
//		if (!sat || !planet)
//		{
//			return;
//		}
//
//		double mu = G * planet->mMass;
//
//		Vec3 rx = h * h / mu / (1.0 + e * cost) * Vec3(cost, sint, 0.0);
//		Vec3 vx = mu / h * Vec3(-sint, e + cost, 0.0);
//
//		Mat3 Qxx = transpose(
//			Mat3(
//				cosO * coso - sinO * sino * cosi, -cosO * sino - sinO * cosi * coso, sinO * sini
//				, sinO * coso + cosO * cosi * sino, -sinO * sino + cosO * cosi * coso, -cosO * sini
//				,        sini * sino              ,         sini * coso              ,      cosi
//				)
//			);
//
//		//setting parameters
//		sat->mPosition = Qxx * rx;
//		sat->mMat[3] = Vec4(sat->mPosition, 1.0);
//		sat->mVelocity = Qxx * vx;
//
//		mC = h;
//		mH = h;
//		mE = e;
//		mI = i;
//		mP = h * h / mu;
//		mAP = omega;
//		mRA = Omega;
//		mTA = theta;
//		mMu = mu;
//
//		mCv = cross(sat->mPosition, sat->mVelocity);
//		mNv = cross(Vec3(0.0, 0.0, 1.0), mCv);
//		mEv = cross(sat->mVelocity, mCv) / mMu - sat->mPosition / length(sat->mPosition);
//
//		updateSpecificParams();
//	}
//
//	//TODO
//	OrbitComponent::R_V OrbitComponent::orbitStateAngle(double theta) const
//	{
//		//TODO
//
//		return {};
//	}
//
//	OrbitComponent::R_V OrbitComponent::orbitStateTime(const Time& dt) const
//	{
//		//TODO
//
//		return {};
//	}
//
//
//	void OrbitComponent::updateOrbit()
//	{
//		using glm::cross;
//		using glm::dot;
//		using glm::length;
//
//
//		auto planet    = mPlanetPhys.lock();
//		auto satellite = mSatellitePhys.lock();
//		if (!planet || !satellite)
//		{
//			return;
//		}
//
//		//algorithm
//		mMu = G * planet->mMass;
//
//		//1.
//		Vec3& r0 = satellite->mPosition;
//		Vec3& v0 = satellite->mVelocity;
//		Vec3& center = planet->mPosition;
//
//
//		Vec3  rv = r0 - center;
//		double r = length(rv);
//
//		//2.
//		Vec3  vv = v0;
//		double v = length(v0);
//		mH = dot(vv, vv) - 2.0 * mMu / r;
//
//		//3.
//		double vr = dot(rv / r, vv);
//
//		//4, 5
//		mCv = cross(rv, vv);//
//		mC  = length(mCv);	
//		mP  = mC * mC / mMu;
//
//		//6.
//		mI = acos(mCv.z / mC);
//
//		//7, 8
//		mNv = cross(Vec3(0.0, 0.0, 1.0), mCv);
//		double N = length(mNv);
//
//		//9.
//		mRA = acos(mNv.x / N);
//		if(mNv.y <= std::numeric_limits<double>::epsilon())
//		{
//			mRA = PI_2 - mRA;
//		}	
//
//		//10, 11
//		mEv = cross(vv, mCv) / mMu - rv / r;
//		mE  = length(mEv);
//
//		Vec3 uev = (mE > std::numeric_limits<double>::epsilon() ? mEv / mE : Vec3(0.0));
//
//		//12.
//		mAP = acos(dot((mNv / N), uev));
//		if (mEv.z <= std::numeric_limits<double>::epsilon())
//		{
//			mAP = PI_2 - mAP;
//		}
//
//		//13.
//		mTA = acos(dot(rv / r, uev));
//		if(vr <= std::numeric_limits<double>::epsilon())
//		{
//			mTA = PI_2 - mTA;
//		}	
//
//		updateSpecificParams();
//	}
//
//	void OrbitComponent::updateSpecificParams()
//	{
//		if (mE < 1.0) //elliptic
//		{
//			mA  = mP / (1.0 - mE * mE);
//			mEA = 2.0 * atan(sqrt((1.0 - mE) / (1.0 + mE)) * tan(mTA / 2.0));
//			if (mTA > PI)
//			{
//				mEA = PI_2 + mEA;
//			}
//
//			mN = sqrt(mMu) / pow(mA, 1.5);
//
//			mT  = 1.0 / mN * (mEA - mE * sin(mEA));
//			mOP = 1.0 / mN * PI_2;
//		}
//		else if (mE > 1.0) // hyperbolic
//		{
//			mA  = mP / (mE * mE - 1.0);
//			mEA = 2 * atanh(sqrt((mE - 1.0) / (mE + 1.0)) * tanh(mTA / 2));
//
//			mN = sqrt(mMu) / pow(mA, 1.5);
//
//			mT  = 1.0 / mN * (mE * sinh(mEA) - mEA); //something strange happens here
//			mOP = std::numeric_limits<double>::infinity();
//		}
//		else
//		{
//			mA = 0.0;
//			mT = 0.0;
//			mOP = 0.0;
//			mEA = 0.0;
//		}
//	}
//}