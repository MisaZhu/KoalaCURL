#include "JSCURL.h"
#include <stdlib.h>
#include <curl/curl.h>


static CURL* _getCurl(BCVar* var) {
	BCVar* thisV = var->getParameter(THIS);
	if(thisV == NULL)
		return NULL;
	BCNode* n = thisV->getChild("curl");
	if(n == NULL)
		return NULL;
	return (CURL*)n->var->getPoint();
}

#define GET_CURL CURL* curl = _getCurl(c); \
		if(curl == NULL) \
			return;

static void _destroyCurl(void* p) {
	if(p != NULL) {
		CURL* curl = (CURL*)p;
		curl_easy_cleanup(curl);
	}
}

struct PerformData {
	char *ptr;
	size_t len;
};

bool initPerformData(struct PerformData *s) {
	s->len = 0;
	s->ptr = (char*)malloc(s->len+1);
	if(s->ptr == NULL)
		return false;

	s->ptr[0] = '\0';
	return true;
}

size_t _perfromWrite(void *ptr, size_t size, size_t nmemb, struct PerformData *s) {
	size_t new_len = s->len + size*nmemb;
	s->ptr = (char*)realloc(s->ptr, new_len+1);
	if (s->ptr == NULL) {
		return 0;
	}
	memcpy(s->ptr+s->len, ptr, size*nmemb);
	s->ptr[new_len] = '\0';
	s->len = new_len;

	return size*nmemb;
}

void JSCURL::constructor(KoalaJS* js, BCVar *c, void *) {
	BCVar* thisV = c->getParameter(THIS);
	if(thisV == NULL)
		return;
	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

	BCVar* v = new BCVar();
	v->setPoint(curl, NO_BYTES, _destroyCurl, true);
	thisV->addChild("curl", v);
	c->setReturnVar(thisV);
}

void JSCURL::init(KoalaJS* js, BCVar *c, void *) {
  curl_global_init(CURL_GLOBAL_DEFAULT);
}

void JSCURL::quit(KoalaJS* js, BCVar *c, void *) {
  curl_global_cleanup();
}

void JSCURL::setURL(KoalaJS* js, BCVar *c, void *) {
	GET_CURL
	string url = c->getParameter("url")->getString();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
}

void JSCURL::perform(KoalaJS* js, BCVar *c, void *) {
	GET_CURL

	struct PerformData s;
	initPerformData(&s);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _perfromWrite);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
				        
  CURLcode res = curl_easy_perform(curl);
	if(res == CURLE_OK) {
		BCVar* v = js->newObject("Bytes");
		v->setPoint(s.ptr, s.len, free, true);
		c->setReturnVar(v);
	}
}

