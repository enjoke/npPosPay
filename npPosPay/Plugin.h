#pragma once

/* 插件类，dll的主要类，基本无需修改，功能都在m_pScriptableObject内实现 */
#include "npapi.h"
#include "npruntime.h"
#include "ScriptablePluginObject.h"




/* 插件类 */
class CPlugin		
{
private:
	NPP m_pNPInstance;
	NPWindow	* m_Window;
	NPStream	* m_pNPStream;
	NPBool		m_bInitialized;
	NPObject	*m_pScriptableObject;

public:
	CPlugin(NPP pNPInstance);
	~CPlugin();

	NPBool init(NPWindow* pNPWindow);	//初始化插件
	void shut();						//停止插件，释放内存
	NPBool isInitialized();				//获取插件状态
	NPObject *GetScriptableObject();	//获取插件js对象
};

