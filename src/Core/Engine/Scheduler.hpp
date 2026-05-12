// Deterministic generic phase scheduler.

#pragma once

#include <array>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

namespace battle_sim::core::engine
{
	enum class Phase
	{
		Startup,
		BeforeTick,
		Tick,
		AfterTick,
		Cleanup,
		Shutdown,
		Count
	};

	class IScheduledSystem
	{
	public:
		virtual ~IScheduledSystem() = default;
		virtual void run() = 0;
	};

	class Scheduler
	{
	public:
		void add(Phase phase, std::unique_ptr<IScheduledSystem> system)
		{
			if (!system)
			{
				throw std::invalid_argument("Scheduled system must not be null");
			}
			_systems[indexOf(phase)].push_back(std::move(system));
		}

		void runPhase(Phase phase)
		{
			for (const auto& system : _systems[indexOf(phase)])
			{
				system->run();
			}
		}

		void runAllPhases()
		{
			for (std::size_t phase = 0; phase < indexOf(Phase::Count); ++phase)
			{
				for (const auto& system : _systems[phase])
				{
					system->run();
				}
			}
		}

	private:
		static constexpr std::size_t indexOf(Phase phase)
		{
			return static_cast<std::size_t>(phase);
		}

		std::array<std::vector<std::unique_ptr<IScheduledSystem>>, static_cast<std::size_t>(Phase::Count)> _systems;
	};
}
