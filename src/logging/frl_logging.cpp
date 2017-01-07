#include "logging/frl_logging.h"

namespace frl{ namespace logging{

namespace private_
{
	struct LogWrite
	{
		LogElementList formaters;
		LogParameter param;

		LogWrite( const LogElementList& formaters_, const LogParameter& param_ )
			:	formaters( formaters_ ), param( param_ )
		{
		}

		void operator()( boost::shared_ptr< frl::logging::ILogWriter > &el )
		{
			el->write( formaters, param );
		}
	};
}

Logger::Logger()
	:	level( frl::logging::LEVEL_TRACE )
{
}

Logger::Logger( const String& name_ )
	:	name( name_ ), level( frl::logging::LEVEL_TRACE )
{
}

Logger::~Logger()
{
	clearOutFormat();
	clearWriters();
}

void Logger::clearOutFormat()
{
	formaters.clear();
}

void Logger::clearWriters()
{
	destionations.clear();
}

void Logger::setFormat( const LogElementList &list_ )
{
	clearOutFormat();
	formaters = list_;
}

void Logger::log( const LogParameter &param )
{
	if( level <= param.level )
	{
		private_::LogWrite writer( formaters, param );
		std::for_each( destionations.begin(), destionations.end(), writer );
	}
}

void Logger::setName( const String& newName )
{
	name = newName;
}

void Logger::setLevel( frl::logging::Level newLevel )
{
	level = newLevel;
}

frl::logging::Level Logger::getLevel()
{
	return level;
}

void Logger::addDestination( const FileWriter& )
{
	if( name.empty() )
		return;
	destionations.push_back( boost::make_shared< FileWriter >( name + FRL_STR(".log") ) );
}

} // namespace logging

namespace private_ {

LogProxy::LogProxy( frl::logging::Logger &log, frl::logging::LogParameter &param_ )
	:	dest( log ), param( param_ )
{
}

LogProxy::LogProxy( const LogProxy &other ) : dest( other.dest ), param( other.param )
{
	ss << other.ss.str();
}

LogProxy::~LogProxy()
{
	param.msg = ss.str();
	dest.log( param );
}


LogProxy getLogProxy(	frl::logging::Logger &log,
									frl::logging::Level level,
									frl::ULong thread_id,
									const frl::String &file,
									const frl::String &function,
									frl::ULong line )
{
	frl::logging::LogParameter tmp;
	tmp.level = level;
	tmp.thread_id = thread_id;
	tmp.function = function;
	tmp.file = file;
	tmp.line = line;
	return LogProxy( log, tmp );
}

} // namespace private_
} // FatRat Library
