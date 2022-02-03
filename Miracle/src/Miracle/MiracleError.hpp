#pragma once

namespace Miracle {
	enum class MiracleError : int {
		// No error
		NoError                   = 0x00000000,

		// Window errors
		WindowInitializationError = 0x00000001,
		WindowCreationError
	};
}
