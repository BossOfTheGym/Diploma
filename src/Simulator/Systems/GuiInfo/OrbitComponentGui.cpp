#include "OrbitComponentGui.h"

#include "../SimulatorState.h"
#include "../PlanetSystem.h"

#include "../../Components/Planet.h"
#include "../../Components/SimData.h"

namespace sim
{
	OrbitComponentGui::OrbitComponentGui(ecs::sys::SystemManager* manager)
		: base_t(manager)
	{}

	void OrbitComponentGui::render(Entity entity)
	{
		auto* sysManager = getSystemManager();
		auto* engine     = sysManager->getECSEngine();
		auto& registry   = engine->getRegistry();

		auto* simulatorState = sysManager->get<SimulatorState>();
		auto* planetSystem   = sysManager->get<PlanetSystem>();

		auto& planetComp = registry.get<comp::Planet>(simulatorState->getPlanet());

		auto& orbit      = registry.get<comp::Orbit>(entity);
		auto& orbitState = orbit.getOrbit();
		if (simulatorState->paused())
		{
			char hStr[64];
			char eStr[64];

			double h = orbitState.h;
			double e = orbitState.e;

			sprintf_s(hStr, "%.15f", h);
			sprintf_s(eStr, "%.15f", e);

			ImGui::InputText("angular momentum(h)", hStr, 64, ImGuiInputTextFlags_CharsDecimal);
			ImGui::InputText("eccentricity(e)" ,    eStr, 64, ImGuiInputTextFlags_CharsDecimal);

			h = atof(hStr);
			e = atof(eStr);


			double DEGREE_0 = 0.0, DEGREE_360 = 360.0, DEGREE_180 = 180.0;

			double ra = glm::degrees(orbitState.ra);
			double ap = glm::degrees(orbitState.ap);
			double i  = glm::degrees(orbitState.i);
			double ta = glm::degrees(orbitState.ta);

			ImGui::SliderScalar("right ascension"    , ImGuiDataType_Double, &ra, &DEGREE_0, &DEGREE_360, "%.15f", 0.2f);
			ImGui::SliderScalar("argument of perigee", ImGuiDataType_Double, &ap, &DEGREE_0, &DEGREE_360, "%.15f", 0.2f);
			ImGui::SliderScalar("inclination"        , ImGuiDataType_Double, &i , &DEGREE_0, &DEGREE_180, "%.15f", 0.2f);
			ImGui::SliderScalar("true anomaly"       , ImGuiDataType_Double, &ta, &DEGREE_0, &DEGREE_360, "%.15f", 0.2f);

			orbit.setFromParameters(
				  h
				, glm::radians(i)
				, glm::radians(ra)
				, e
				, glm::radians(ap)
				, glm::radians(ta)
				, planetComp.mu);

			if (registry.has<comp::SimData>(entity))
			{
				auto& simData = registry.get<comp::SimData>(entity);
				simData.setRadius(orbit.getState().r);
				simData.setVelocity(orbit.getState().v);
				planetSystem->updateEntity(entity);
			}
		}
		else
		{
			ImGui::Text("Specific ang. momentum : %f", orbitState.h);
			ImGui::Text("Right ascension        : %f", orbitState.ra);
			ImGui::Text("Argument of perigee    : %f", orbitState.ap);
			ImGui::Text("Inclination            : %f", orbitState.i);
			ImGui::Text("Eccentricity           : %f", orbitState.e);
			ImGui::Text("True anomaly           : %f", orbitState.ta);
			ImGui::Text("");
			ImGui::Text("Eccentric anomaly : %f", orbitState.ea);
			ImGui::Text("Apoapsis          : %f", orbitState.a);
			ImGui::Text("Orbit period      : %f", orbitState.T);
			ImGui::Text("Time              : %f", orbitState.t);
		}
	}

	const char* OrbitComponentGui::name() const
	{
		return "Orbit component";
	}
}