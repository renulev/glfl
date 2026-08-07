#if defined(_WIN32)
static HMODULE hGLModule = NULL;
#else
static void* hGLModule = NULL;
#endif
void glfl_init(void);
void* glfl_get_func(const char* strFunctionName);
void glfl_finalize(void);
void glfl_set_function_pointers(void);
#ifdef GLFL_IMPLEMENTATION
void glfl_init(void) {
#if defined(_WIN32)
	hGLModule = LoadLibrary(TEXT("OpenGL32.dll"));
#else
	hGLModule = dlopen("libGL.so.1", RTLD_LAZY | RTLD_GLOBAL);
#endif
}
void* glfl_get_func(const char* strFunctionName) {
	void* pFunction = NULL;
#if defined(_WIN32)
	pFunction = (void*)GetProcAddress(hGLModule, strFunctionName);
	if (pFunction == NULL) {
		pFunction = (void*)wglGetProcAddress(strFunctionName);
		if (pFunction == (void*)0x1 || pFunction == (void*)0x2 ||
			pFunction == (void*)0x3 || pFunction == (void*)-1) {
			pFunction = NULL;
		}
	}
#else
	if (hGLModule != NULL) {
		pFunction = dlsym(hGLModule, strFunctionName);
	}
	if (pFunction == NULL) {
		pFunction =
			(void*)glXGetProcAddressARB((const GLubyte*)strFunctionName);
	}
#endif
	return pFunction;
}
void glfl_finalize(void) {
	if (hGLModule != NULL) {
#if defined(_WIN32)
		FreeLibrary(hGLModule);
#else
		dlclose(hGLModule);
#endif
		hGLModule = NULL;
	}
}
void glfl_set_function_pointers(void) {
