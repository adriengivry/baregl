/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#include <cstdio>

#include <baregl/debug/Log.h>

namespace baregl::debug
{
	template<>
	void Log<ELogLevel::INFO>(const std::string_view p_message)
	{
		fprintf(
			stdout,
			"[baregl] <info> %s\n",
			p_message.data()
		);
	}

	template<>
	void Log<ELogLevel::WARNING>(const std::string_view p_message)
	{
		fprintf(
			stdout,
			"[baregl] <warning> %s\n",
			p_message.data()
		);
	}

	template<>
	void Log<ELogLevel::ERROR>(const std::string_view p_message)
	{
		fprintf(
			stderr,
			"[baregl] <error> %s\n",
			p_message.data()
		);
	}
}
