#include "SimulatorStateGui.h"

#include <Math/MathLib.h>

#include "../TimeSystem.h"

#include "../../Components/SimData.h"
#include "../../Components/Rendezvous.h"

namespace sim
{
	SimulatorStateGui::SimulatorStateGui(ecs::sys::SystemManager* manager)
		: base_t(manager)
	{
		m_transferTime = 3600.0;
		m_destX = 0.0;
		m_destY = 0.0;
		m_destZ = 0.0;

		auto* sysManager = getSystemManager();
		auto* engine     = sysManager->getECSEngine();
		auto& registry   = engine->getRegistry();

		auto* simulatorState = sysManager->get<SimulatorState>();

		m_axesBound = simulatorState->cameraBoundToNaturalAxes();
	}

	void SimulatorStateGui::render()
	{
		auto* sysManager = getSystemManager();
		auto* engine     = sysManager->getECSEngine();
		auto& registry   = engine->getRegistry();

		auto* timeSystem     = sysManager->get<TimeSystem>();
		auto* simulatorState = sysManager->get<SimulatorState>();


		if (simulatorState->paused())
		{
			if (ImGui::Button("Resume"))
			{
				simulatorState->resume();
			}
		}
		else
		{
			if (ImGui::Button("Pause"))
			{
				simulatorState->pause();
			}
		}

		if (simulatorState->paused())
		{
			if (!simulatorState->rendezvousStarted())
			{
				char transferStr[64];
				sprintf_s(transferStr, "%.15Lf", m_transferTime);

				ImGui::InputText("time", transferStr, 64, ImGuiInputTextFlags_CharsDecimal);

				m_transferTime = atof(transferStr);
				if (ImGui::Button("Start"))
				{
					auto t  = timeSystem->getTime();
					auto dt = ecs::toTime<ecs::Tick>(ecs::Seconds<double>(m_transferTime));

					simulatorState->startRendezvous(t, dt);
				}

				if (ImGui::Button("Reset state"))
				{
					simulatorState->resetTestState();
				}
				// TEST
				//char destStr[64];
				//
				//sprintf_s(destStr, "%.15Lf", m_destX);
				//ImGui::InputText("x", destStr, 64, ImGuiInputTextFlags_CharsDecimal);
				//m_destX = atof(destStr);
				//
				//sprintf_s(destStr, "%.15Lf", m_destY);
				//ImGui::InputText("y", destStr, 64, ImGuiInputTextFlags_CharsDecimal);
				//m_destY = atof(destStr);
				//
				//sprintf_s(destStr, "%.15Lf", m_destZ);
				//ImGui::InputText("z", destStr, 64, ImGuiInputTextFlags_CharsDecimal);
				//m_destZ = atof(destStr);
				//
				//if (ImGui::Button("Test Lambert"))
				//{
				//	auto t = timeSystem->getTime();
				//	auto dt = ecs::toTime<ecs::Tick>(ecs::Seconds<double>(m_transferTime));
				//
				//	simulatorState->startLambertTransfer({m_destX, m_destY, m_destZ}, dt);
				//}
				// END TEST
			}
		}

		if (simulatorState->rendezvousStarted())
		{
			auto target = simulatorState->getTarget();
			auto chaser = simulatorState->getChaser();

			if (registry.valid(target) && registry.has<comp::SimData>(target) 
				&& registry.valid(chaser) && registry.has<comp::SimData, comp::Rendezvous>(chaser))
			{
				auto& targetSim = registry.get<comp::SimData>(target);
				auto& chaserSim = registry.get<comp::SimData>(chaser);

				auto& rendComp = registry.get<comp::Rendezvous>(chaser);

				auto dr = targetSim.getRadius() - chaserSim.getRadius();
				auto dv = targetSim.getVelocity() - chaserSim.getVelocity();

				ImGui::Text("dr:%f  x:%f y:%f z:%f", glm::length(dr), dr.x, dr.y, dr.z);
				ImGui::Text("dv:%f  x:%f y:%f z:%f", glm::length(dv), dv.x, dv.y, dv.z);
				ImGui::Text("Time remaining: %f", ecs::toSeconds<double>(rendComp.duration).count());
			}
			if (ImGui::Button("Abort"))
			{
				simulatorState->abortRendezvous();
			}
		}
		if (ImGui::Checkbox("Bind camera to natural axes", &m_axesBound))
		{
			simulatorState->setCameraBindValue(m_axesBound);
		}
	}

	const char* SimulatorStateGui::name() const
	{
		return "Simulator state";
	}
}