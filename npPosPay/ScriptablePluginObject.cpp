#include "ScriptablePluginObject.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <Windowsx.h>


/* 构造函数主要初始化了js对象：
	NPN_GetStringIdentifier("")声明函数对象以供外部js调用
*/
ScriptablePluginObject::ScriptablePluginObject(NPP npp) : ScriptablePluginObjectBase(npp)
{
	m_posTrade = NPN_GetStringIdentifier("posTrade");
	m_connect = NPN_GetStringIdentifier("connect");
	m_disconnect = NPN_GetStringIdentifier("disconnect");
	m_getStatus = NPN_GetStringIdentifier("getStatus");
	m_hInstance = NULL;
	m_callPos = NULL;
	m_test = NULL;
}

ScriptablePluginObject::~ScriptablePluginObject()
{
}

/* 供浏览器调用，告诉浏览器本插件是否有name方法 */
bool
ScriptablePluginObject::HasMethod(NPIdentifier name) 
{
	return name == m_posTrade || name == m_connect || name == m_disconnect || name == m_getStatus;
}

bool
ScriptablePluginObject::HasProperty(NPIdentifier name)
{
	return false;
}

bool
ScriptablePluginObject::GetProperty(NPIdentifier name, NPVariant *result)
{
	return false;
}

bool ScriptablePluginObject::SetProperty(NPIdentifier name, const NPVariant *value)
{
	return false;
}

/*
	所有供js调用的函数均在本函数实现
 */
bool
ScriptablePluginObject::Invoke(NPIdentifier name, const NPVariant *args,
	uint32_t argCount, NPVariant *result)
{
	if (name == m_posTrade) {
		char msg[4096];
		memset(msg, 0, sizeof(msg));
		PosTrade(NPVARIANT_TO_OBJECT(args[0]), msg);
		multi2WideChar(msg, result);
		return TRUE;
	}
	else if (name == m_connect || name == m_disconnect || name == m_getStatus) {
		char msg[4096];
		memset(msg, 0, sizeof(msg));
		comTest(msg);
		multi2WideChar(msg, result);
		return TRUE;
	}
	return false;
}

/* js对象转内部结构体
 */
int ScriptablePluginObject::NPObj2Struct(NPObject *obj, MIS_REQ *req) {
	NPVariant tmp;
	NPIdentifier requestSerial = NPN_GetStringIdentifier("requestSerial");
	memset(&tmp, 0, sizeof(NPVariant));
	if (obj->_class->getProperty(obj, requestSerial, &tmp)) {
		strcpy(req->requestSerial, NPVARIANT_TO_STRING(tmp).UTF8Characters);
	}

	NPIdentifier operateType = NPN_GetStringIdentifier("operateType");
	memset(&tmp, 0, sizeof(NPVariant));
	if (obj->_class->getProperty(obj, operateType, &tmp)) {
		strcpy(req->operateType, NPVARIANT_TO_STRING(tmp).UTF8Characters);
	}
	NPIdentifier tradeType = NPN_GetStringIdentifier("tradeType");
	memset(&tmp, 0, sizeof(NPVariant));
	if (obj->_class->getProperty(obj, tradeType, &tmp)) {
		strcpy(req->tradeType, NPVARIANT_TO_STRING(tmp).UTF8Characters);
	}

	NPIdentifier payType = NPN_GetStringIdentifier("payType");
	memset(&tmp, 0, sizeof(NPVariant));
	if (obj->_class->getProperty(obj, payType, &tmp)) {
		strcpy(req->payType, NPVARIANT_TO_STRING(tmp).UTF8Characters);
	}
	NPIdentifier operateStaff = NPN_GetStringIdentifier("operateStaff");
	memset(&tmp, 0, sizeof(NPVariant));
	if (obj->_class->getProperty(obj, operateStaff, &tmp)) {
		strcpy(req->operateStaff, NPVARIANT_TO_STRING(tmp).UTF8Characters);
	}
	NPIdentifier channelCode = NPN_GetStringIdentifier("channelCode");
	memset(&tmp, 0, sizeof(NPVariant));
	if (obj->_class->getProperty(obj, channelCode, &tmp)) {
		strcpy(req->channelCode, NPVARIANT_TO_STRING(tmp).UTF8Characters);
	}
	NPIdentifier paySerial = NPN_GetStringIdentifier("paySerial");
	memset(&tmp, 0, sizeof(NPVariant));
	if (obj->_class->getProperty(obj, paySerial, &tmp)) {
		strcpy(req->paySerial, NPVARIANT_TO_STRING(tmp).UTF8Characters);
	}
	NPIdentifier systemSerial = NPN_GetStringIdentifier("systemSerial");
	memset(&tmp, 0, sizeof(NPVariant));
	if (obj->_class->getProperty(obj, systemSerial, &tmp)) {
		strcpy(req->systemSerial, NPVARIANT_TO_STRING(tmp).UTF8Characters);
	}
	NPIdentifier payMoney = NPN_GetStringIdentifier("payMoney");
	memset(&tmp, 0, sizeof(NPVariant));
	if (obj->_class->getProperty(obj, payMoney, &tmp)) {
		strcpy(req->payMoney, NPVARIANT_TO_STRING(tmp).UTF8Characters);
	}

	NPIdentifier extField = NPN_GetStringIdentifier("extField");
	memset(&tmp, 0, sizeof(NPVariant));
	if (obj->_class->getProperty(obj, extField, &tmp)) {
		strcpy(req->extField, NPVARIANT_TO_STRING(tmp).UTF8Characters);
	}

	return 0;
}

/* 核销函数实现 */
int ScriptablePluginObject::PosTrade(NPObject * obj, char *msg)
{
	MIS_REQ req;
	MIS_RES res;
	char  error[512],path[256];
	memset(error, 0, sizeof(error));
	memset(path, 0, sizeof(path));
	GetCurrentDirectory(sizeof(path), path);
	strcat(path, "\\mispos.dll");

	NPObj2Struct(obj, &req);

	m_hInstance = LoadLibrary(path);
	if (NULL == m_hInstance) {
		ret_err(msg, "", "01", "load dll failed", "0");
		return -1;
	}

	m_callPos = (COMPOS)GetProcAddress(m_hInstance, "Trans");
	if (NULL == m_callPos) {
		FreeLibrary(m_hInstance);
		ret_err(msg, "", "01", "GetProcAddress Trans failed.", "0");
		return -1;
	}

	int rc = m_callPos(&res, &req, error);
	if (rc) {
		ret_err(msg, "", "01", error, "1");
	}
	else {
		pkg_jso(msg, &res);
	}

	if (m_hInstance)
		FreeLibrary(m_hInstance);
	return rc;
}

int ScriptablePluginObject::comTest(char * msg)
{
	char  error[512],path[256];
	memset(error, 0, sizeof(error));
	memset(path, 0, sizeof(path));
	GetCurrentDirectory(sizeof(path), path);
	strcat(path, "mispos.dll");

	m_hInstance = LoadLibrary(path);
	if (NULL == m_hInstance) {
		ret_err(msg, "", "-1", "load dll failed", "0");
		return FALSE;
	}

	m_test = (COMTEST)GetProcAddress(m_hInstance, "COM_test");
	if (NULL == m_test) {
		FreeLibrary(m_hInstance);
		ret_err(msg, "", "-1", "GetProcAddress Trans failed.", "0");
		return FALSE;
	}

	if (m_test(error)) {
		ret_err(msg, "", "0", "", "1");
	}
	else {
		ret_err(msg, "", "-1", error, "0");
	}
	FreeLibrary(m_hInstance);
	return 0;
}

/* 一下均为返回json报文打包 */
void ScriptablePluginObject::pkg_jso_add(char *pJso, char *k, char *v, int isEnd)
{
	char	temp[256];
	if (!strlen(v)) return;
	sprintf(temp, "%s:\'%s\'%c", k, v, isEnd ? ' ' : ',');
	strcat(pJso, temp);
}

void ScriptablePluginObject::pkg_jso(char *strRes, MIS_RES *pRes)
{
	char	sRes[4096], temp[2048];
	memset(sRes, 0, sizeof(sRes));
	memset(temp, 0, sizeof(temp));

	strcpy(strRes, "{");

	pkg_jso_add(strRes, "resultFlag", pRes->resultFlag, 0);
	pkg_jso_add(strRes, "requestSerial", pRes->requrestSerial, 0);
	pkg_jso_add(strRes, "errorMsg", pRes->errorMsg, 0);
	pkg_jso_add(strRes, "status", pRes->status, 0);

	pkg_resCov(sRes, &pRes->resultContent);
	sprintf(temp, "resultContent:%s", sRes);
	strcat(strRes, temp);
	
	strcat(strRes, "}");

}

void ScriptablePluginObject::pkg_resCov(char *strCov, RESULT_STRU *pResult)
{
	strcpy(strCov, "{");

	pkg_jso_add(strCov, "authorizeNbr", pResult->authorizeNbr, 0);
	pkg_jso_add(strCov, "cardBankCode", pResult->cardBankCode, 0);
	pkg_jso_add(strCov, "cardBankName", pResult->cardBankName, 0);
	pkg_jso_add(strCov, "channelCode", pResult->channelCode, 0);
	pkg_jso_add(strCov, "cardEffDate", pResult->cardEffDate, 0);
	pkg_jso_add(strCov, "channelCode", pResult->channelCode, 0);
	pkg_jso_add(strCov, "extField", pResult->extField, 0);
	pkg_jso_add(strCov, "operateStaff", pResult->operateStaff, 0);
	pkg_jso_add(strCov, "operateType", pResult->operateType, 0);
	pkg_jso_add(strCov, "partnerName", pResult->partnerName, 0);
	pkg_jso_add(strCov, "partnerNbr", pResult->partnerNbr, 0);
	pkg_jso_add(strCov, "paymentNbr", pResult->paymentNbr, 0);
	pkg_jso_add(strCov, "payMoney", pResult->payMoney, 0);
	pkg_jso_add(strCov, "paySerial", pResult->paySerial, 0);
	pkg_jso_add(strCov, "payType", pResult->payType, 0);
	pkg_jso_add(strCov, "settleSerial", pResult->settleSerial, 0);
	pkg_jso_add(strCov, "srcPaySerial", pResult->srcPaySerial, 0);
	pkg_jso_add(strCov, "srcSystemSerial", pResult->srcSystemSerial, 0);
	pkg_jso_add(strCov, "systemSerial", pResult->systemSerial, 0);
	pkg_jso_add(strCov, "sysReferNbr", pResult->sysReferNbr, 0);
	pkg_jso_add(strCov, "terminalNbr", pResult->terminalNbr, 0);
	pkg_jso_add(strCov, "tradeDate", pResult->tradeDate, 0);
	pkg_jso_add(strCov, "tradeMedia", pResult->tradeMedia, 0);
	pkg_jso_add(strCov, "tradeTime", pResult->tradeTime, 0);
	pkg_jso_add(strCov, "tradeType", pResult->tradeType, 1);

	strcat(strCov, "}");
}

void ScriptablePluginObject::ret_err(char *pJso, char *reqNo, char *szRc, char *szRcDetail, char *status)
{
	MIS_RES		res;

	memset(&res, 0, sizeof(MIS_RES));
	strcpy(res.errorMsg, szRcDetail);
	strcpy(res.resultFlag, szRc);
	strcpy(res.requrestSerial, reqNo);
	strcpy(res.status, status);

	pkg_jso(pJso, &res);
}
/* 以上均为返回报文打包 */
/* utf8转unicode */
void ScriptablePluginObject::multi2WideChar(char * str, NPVariant * obj)
{
	WCHAR *pwszBuffer = NULL;
	char *pszBuffer = NULL;
	int nLen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	if (nLen == 0) goto __END__;

	pwszBuffer = new WCHAR[nLen + 1];
	nLen = MultiByteToWideChar(CP_ACP, 0, str, -1, pwszBuffer, nLen);
	if (nLen == 0) goto __END__;

	nLen = WideCharToMultiByte(CP_UTF8, 0, pwszBuffer, -1, NULL, 0, NULL, NULL);
	if (nLen == 0) goto __END__;

	pszBuffer = new char[nLen + 1];
	nLen = WideCharToMultiByte(CP_UTF8, 0, pwszBuffer, -1, pszBuffer, nLen, NULL, NULL);
	if (nLen == 0) goto __END__;

	nLen = strlen(pszBuffer) + 1;
	NPUTF8  *utf8_chars = (NPUTF8*)NPN_MemAlloc(nLen); //NPN_MemAlloc申请的内存会被浏览器接管，这里不需要关心释放问题
	memset(utf8_chars, 0, nLen);
	strncpy(utf8_chars, pszBuffer, nLen);
	STRINGZ_TO_NPVARIANT(utf8_chars, *obj);

__END__:
	if (pszBuffer)
		delete[]pszBuffer;
	if (pwszBuffer)
		delete[]pwszBuffer;
}