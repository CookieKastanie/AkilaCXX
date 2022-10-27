#pragma once

namespace akila {
	class Core {
	public:
		static int run(void (*init)(void));
		static void restart();

	private:
		static bool restartFlag;
	};
}
