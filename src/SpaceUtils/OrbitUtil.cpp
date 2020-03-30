//#include "Orbit.h"
//
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