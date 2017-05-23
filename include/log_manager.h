#ifndef _LOG_MANAGER_H_
#define _LOG_MANAGER_H_

#include <stdafx.h>
#include <singleton.h>

#define LL_DEBUG 1			/// This Log-level indicates a diagnostic log message
#define LL_NORMAL 2			/// This Log-level indicates a normal-run log message
#define LL_WARNING 4		/// This Log-level indicates a potential problem log message
#define LL_ERROR 8			/// This Log-level inidcates a serious problem log message
#define LL_SCRIPT 16		/// This Log-level indicates something happend in the script-world

// "Convenience macro" for accessing the singleton
#define LOG_MANAGER ssuge::LogManager::getSingletonPtr()

namespace ssuge
{
	/// The goal of the LogManager is to write data to a log-file (identified when we 
	/// first create the LogManager).
	class LogManager : public Singleton<LogManager>
	{
	/***** ATTRIBUTES *****/
	protected:
		/// The file reference object we'll use for the lifetime of the LogManager
		std::ofstream mFile;

		/// A bit-wise OR'ing of all log levels to allow to the log file (see setLogMask())
		unsigned int mLogMask;

	/***** CONSTRUCTORS / DESTRUCTORS *****/
	public:
		/// Opens the file (in write-create mode) and initializes the log mask.
		LogManager(std::string fname);

		/// Shuts down the file connection.
		~LogManager();

	/***** METHODS *****/
	public:
		/// Writes the given message to the log file *IF* the logMask & lvl is non-zero (true)
		void log(std::string message, unsigned int lvl = LL_NORMAL);

		/// Replaces the existing log-mask.  This has no effect on log messages that have already been written to the log.
		void setLogMask(unsigned int lvl_mask = LL_DEBUG | LL_NORMAL | LL_WARNING | LL_ERROR);
	};
}

#endif