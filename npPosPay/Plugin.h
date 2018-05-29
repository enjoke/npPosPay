#pragma once

/* ����࣬dll����Ҫ�࣬���������޸ģ����ܶ���m_pScriptableObject��ʵ�� */
#include "npapi.h"
#include "npruntime.h"
#include "ScriptablePluginObject.h"




/* ����� */
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

	NPBool init(NPWindow* pNPWindow);	//��ʼ�����
	void shut();						//ֹͣ������ͷ��ڴ�
	NPBool isInitialized();				//��ȡ���״̬
	NPObject *GetScriptableObject();	//��ȡ���js����
};

