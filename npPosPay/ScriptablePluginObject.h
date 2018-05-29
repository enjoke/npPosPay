#include <stdio.h>
#include "npfunctions.h"
#include "npapi.h"
#include "npruntime.h"

#include "PosStru.h"

/* JS对象类，
	ScriptablePluginObjectBase为基类，无需修改，
	所有跟JS交互的功能都在ScriptablePluginObject类中实现
*/


typedef int (FAR WINAPI *COMPOS)(MIS_RES *, MIS_REQ *, char *);
typedef int (FAR WINAPI *COMTEST)(char *);


// Helper class that can be used to map calls to the NPObject hooks
// into virtual methods on instances of classes that derive from this
// class.
class ScriptablePluginObjectBase : public NPObject
{
public:
	ScriptablePluginObjectBase(NPP npp)
		: mNpp(npp)
	{
	}

	virtual ~ScriptablePluginObjectBase()
	{
	}

	// Virtual NPObject hooks called through this base class. Override
	// as you see fit.
	virtual void Invalidate();
	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
		uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
		NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
	virtual bool RemoveProperty(NPIdentifier name);
	virtual bool Enumerate(NPIdentifier **identifier, uint32_t *count);
	virtual bool Construct(const NPVariant *args, uint32_t argCount,
		NPVariant *result);

public:
	static void _Deallocate(NPObject *npobj);
	static void _Invalidate(NPObject *npobj);
	static bool _HasMethod(NPObject *npobj, NPIdentifier name);
	static bool _Invoke(NPObject *npobj, NPIdentifier name,
		const NPVariant *args, uint32_t argCount,
		NPVariant *result);
	static bool _InvokeDefault(NPObject *npobj, const NPVariant *args,
		uint32_t argCount, NPVariant *result);
	static bool _HasProperty(NPObject * npobj, NPIdentifier name);
	static bool _GetProperty(NPObject *npobj, NPIdentifier name,
		NPVariant *result);
	static bool _SetProperty(NPObject *npobj, NPIdentifier name,
		const NPVariant *value);
	static bool _RemoveProperty(NPObject *npobj, NPIdentifier name);
	static bool _Enumerate(NPObject *npobj, NPIdentifier **identifier,
		uint32_t *count);
	static bool _Construct(NPObject *npobj, const NPVariant *args,
		uint32_t argCount, NPVariant *result);

protected:
	NPP mNpp;
};


/* 以下是js对象类基类和子类的声明 */
#define DECLARE_NPOBJECT_CLASS_WITH_BASE(_class, ctor)                        \
    static NPClass s##_class##_NPClass = {                                        \
    NP_CLASS_STRUCT_VERSION_CTOR,                                               \
    ctor,                                                                       \
    ScriptablePluginObjectBase::_Deallocate,                                    \
    ScriptablePluginObjectBase::_Invalidate,                                    \
    ScriptablePluginObjectBase::_HasMethod,                                     \
    ScriptablePluginObjectBase::_Invoke,                                        \
    ScriptablePluginObjectBase::_InvokeDefault,                                 \
    ScriptablePluginObjectBase::_HasProperty,                                   \
    ScriptablePluginObjectBase::_GetProperty,                                   \
    ScriptablePluginObjectBase::_SetProperty,                                   \
    ScriptablePluginObjectBase::_RemoveProperty,                                \
    ScriptablePluginObjectBase::_Enumerate,                                     \
    ScriptablePluginObjectBase::_Construct                                      \
}

#define GET_NPOBJECT_CLASS(_class) &s##_class##_NPClass

class ScriptablePluginObject : public ScriptablePluginObjectBase
{
private:
	NPIdentifier m_posTrade;				//函数指针，供外部js调用
	NPIdentifier m_connect;
	NPIdentifier m_disconnect;
	NPIdentifier m_getStatus;
	HINSTANCE	m_hInstance;
	COMPOS		m_callPos;
	COMTEST     m_test;

	int NPObj2Struct(NPObject *obj, MIS_REQ *req);
	int PosTrade(NPObject *obj, char *error);
	int comTest(char *msg);
	void pkg_jso_add(char *pJso, char *k, char *v, int isEnd);
	void pkg_jso(char *strRes, MIS_RES *pRes);
	void pkg_resCov(char *strCov, RESULT_STRU *pResult);
	void ret_err(char *pJso, char *reqNo, char *szRc, char *szRcDetail, char *status);
	void multi2WideChar(char *str, NPVariant *obj);
public:
	ScriptablePluginObject(NPP npp);
	virtual ~ScriptablePluginObject();

	virtual bool HasMethod(NPIdentifier name);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
		uint32_t argCount, NPVariant *result);

	
};
static NPObject *
AllocateScriptablePluginObject(NPP npp, NPClass *aClass)
{
	return new ScriptablePluginObject(npp);
}

DECLARE_NPOBJECT_CLASS_WITH_BASE(ScriptablePluginObject,AllocateScriptablePluginObject);
