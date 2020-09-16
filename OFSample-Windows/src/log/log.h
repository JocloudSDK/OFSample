#pragma once

#include <string>
#include <utility>
#include <vector>

using std::string;
using std::vector;
using std::pair;

/**
 * Log format：[TAG] [logid] method_name(duration_ms) | message { info_key:info_value, ... }
 */
/**
 * 日志规范：[TAG] [logid] method_name(duration_ms) | message { info_key:info_value, ... }
 */


namespace  base {

	class Log {
	public:
		Log();
		Log(const string& methodName);

		Log& setMethod(const string& methodName);
		Log& setDuration(int duration);
		Log& setLogid(const string& logid);
		Log& setMessage(const string& message);
		Log& setMessage(const char *fmt, ...);
		Log& addDetail(const string& key, const string& value);

		operator string() const;

	private:
		string logid;
		string method;
		int    duration;
		string message;
		vector<pair<string, string> > details;
	};
}


