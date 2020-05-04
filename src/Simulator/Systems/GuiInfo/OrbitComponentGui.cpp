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

			sprintf_s(hStr, "%.15lf", h);
			sprintf_s(eStr, "%.15lf", e);

			ImGui::InputText("h", hStr, 64, ImGuiInputTextFlags_CharsDecimal);
			ImGui::InputText("e", eStr, 64, ImGuiInputTextFlags_CharsDecimal);

			h = atof(hStr);
			e = atof(eStr);

			double DEGREE_0 = 0.0, DEGREE_360 = 360.0, DEGREE_180 = 180.0;

			double ra = glm::degrees(orbitState.ra);
			double ap = glm::degrees(orbitState.ap);
			double i  = glm::degrees(orbitState.i);
			double ta = glm::degrees(orbitState.ta);

			ImGui::SliderScalar("ra", ImGuiDataType_Double, &ra, &DEGREE_0, &DEGREE_360, "%.15lf", 1.0f);
			ImGui::SliderScalar("ap", ImGuiDataType_Double, &ap, &DEGREE_0, &DEGREE_360, "%.15lf", 1.0f);
			ImGui::SliderScalar("i" , ImGuiDataType_Double, &i , &DEGREE_0, &DEGREE_180, "%.15lf", 1.0f);
			ImGui::SliderScalar("ta", ImGuiDataType_Double, &ta, &DEGREE_0, &DEGREE_360, "%.15lf", 1.0f);

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
			ImGui::Text("h : %lf", orbitState.h);
			ImGui::Text("ra: %lf", orbitState.ra);
			ImGui::Text("ap: %lf", orbitState.ap);
			ImGui::Text("i : %lf", orbitState.i);
			ImGui::Text("e : %lf", orbitState.e);
			ImGui::Text("ta: %lf", orbitState.ta);
			ImGui::Text("");
			ImGui::Text("ea: %lf", orbitState.ea);
			ImGui::Text("a : %lf", orbitState.a);
			ImGui::Text("T : %lf", orbitState.T);
			ImGui::Text("t : %lf", orbitState.t);
		}
	}

	const char* OrbitComponentGui::name() const
	{
		return "Orbit component";
	}
}