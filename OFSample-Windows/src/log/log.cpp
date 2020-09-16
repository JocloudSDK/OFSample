#include "Log.h"
#include <stdio.h>
#include <stdarg.h>
#include <sstream>

using namespace std;

namespace base {
	Log::Log() {
		this->duration = -1;
	}

	Log::Log(const string& methodName) {
		this->duration = -1;
		this->method   = methodName;
	}

	Log& Log::setMethod(const string& methodName) {
		this->method = methodName;
		return *this;
	}

	Log& Log::setDuration(int duration) {
		this->duration = duration;
		return *this;
	}

	Log& Log::setLogid(const string& logid) {
		this->logid = logid;
		return *this;
	}

	Log& Log::setMessage(const string& message) {
		this->message = message;
		return *this;
	}

	Log& Log::setMessage(const char *fmt, ...) {
		char buffer[1024];

		va_list args;
		va_start(args, fmt);
		vsnprintf(buffer, 1024, fmt, args);
		va_end(args);

		this->message = string(buffer);
		return *this;
	}

	Log& Log::addDetail(const string& key, const string& value) {
		this->details.push_back(make_pair(key, value));
		return *this;
	}

	Log::operator string() const {
		stringstream ss;

		if (!logid.empty()) {
			ss << '[' << logid << ']';
		}

		if (!method.empty()) {
			if (ss.tellp() != streampos(0)) {
				ss << ' ';
			}
			ss << method;
		}

		if (!message.empty() || !details.empty() && ss.tellp() != streampos(0)) {
			ss << " |";
		}

		if (!message.empty()) {
			if (ss.tellp() != streampos(0)) {
				ss << ' ';
			}
			ss << message;
		}

		if (!details.empty()) {
			if (ss.tellp() != streampos(0)) {
				ss << ' ';
			}

			ss << '{';
			for (size_t i = 0; i < details.size(); ++i) {
				auto& pr = details[i];

				if (i > 0) {
					ss << ", ";
				}
				ss << pr.first << ":" << pr.second;
			}
			ss << '}';
		}

		return ss.str();
	}
}
