#ifndef JSM_JSCURL
#define JSM_JSCURL

#include "ClassLoader.h"

class JSCURL : public JSClass {
		static void constructor(KoalaJS* js, BCVar *c, void *);
		static void init(KoalaJS* js, BCVar *c, void *);
		static void quit(KoalaJS* js, BCVar *c, void *);
		static void setURL(KoalaJS* js, BCVar *c, void *);
		static void perform(KoalaJS* js, BCVar *c, void *);
	protected:
	inline void registerFunctions(KoalaJS* js, const std::string& className) {
		addFunction(js, className, "constructor()", constructor, NULL);
		addFunction(js, className, "init()", init, NULL);
		addFunction(js, className, "quit()", quit, NULL);
		addFunction(js, className, "setURL(url)", setURL, NULL);
		addFunction(js, className, "perform()", perform, NULL);
	}

	public:
	DECL_INSTANCE(JSCURL)
};


#endif
