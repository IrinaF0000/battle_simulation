// Neutral engine loop over scheduled phases.

#include "Core/Engine/EngineRunner.hpp"

#include <algorithm>

namespace battle_sim::core::engine
{
	EngineRunner::EngineRunner(GameContext& context)
		: _context(context)
	{
	}

	EngineRunResult EngineRunner::run()
	{
		int ticksExecuted = 0;
		const int maxTicks = std::max(0, _context.settings.maxTicks);

		_context.scheduler.runPhase(Phase::Startup);

		while (!_stopRequested && ticksExecuted < maxTicks)
		{
			_context.scheduler.runPhase(Phase::BeforeTick);
			if (_stopRequested)
			{
				break;
			}

			_context.scheduler.runPhase(Phase::Tick);
			if (_stopRequested)
			{
				break;
			}

			_context.scheduler.runPhase(Phase::AfterTick);
			if (_stopRequested)
			{
				break;
			}

			_context.scheduler.runPhase(Phase::Cleanup);
			++ticksExecuted;
			_context.world.advanceTick();
		}

		_context.scheduler.runPhase(Phase::Shutdown);

		return EngineRunResult{
			.ticksExecuted = ticksExecuted,
			.stopRequested = _stopRequested,
			.reachedMaxTicks = !_stopRequested && ticksExecuted >= maxTicks};
	}

	void EngineRunner::requestStop()
	{
		_stopRequested = true;
	}

	void EngineRunner::clearStopRequest()
	{
		_stopRequested = false;
	}

	bool EngineRunner::stopRequested() const
	{
		return _stopRequested;
	}
}
