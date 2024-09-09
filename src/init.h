#pragma once

namespace ed
{
	class Init
	{
	private:
		bool init{ true };
	public:
		void Negate() { init = !init; }
		bool InitOk() const { return init; }
	};
}