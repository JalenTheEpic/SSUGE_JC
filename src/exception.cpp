#include <stdafx.h>
#include <exception.h>
#include <log_manager.h>


ssuge::Exception::Exception(std::string file, unsigned int line, std::string desc) : 
						mDescription(desc), mFile(file), mLine(line)
{
	if (LOG_MANAGER)
	{
		std::stringstream ss;
		ss << file << "@" << line << ": " << desc;
		LOG_MANAGER->log(ss.str(), LL_ERROR);
	}
}