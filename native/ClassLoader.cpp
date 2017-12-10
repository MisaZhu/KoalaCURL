#include "native/CURL/JSCURL.h"

extern "C" void _moduleLoader(KoalaJS* js) {
	JSCURL::instance().load(js, "CURL");
}
