#ifndef _POS_STRU_H_
#define _POS_STRU_H_

/* ��������������� */
typedef struct {
	char	requestSerial[23];		/* ������ˮ�� */
	char	operateType[3];			/* ����ҵ������ */
	char	tradeType[3];			/* �������� */
	char	payType[3];				/* �������� */
	char	operateStaff[13];		/* �տ�Ա�� */
	char	channelCode[13];		/* Ӫҵ������ */
	char	paySerial[7];			/* ������ˮ�� */
	char	systemSerial[7];		/* ϵͳ��ˮ�� */
	char	payMoney[19];			/* �����12.2 */
	char	extField[65];			/* ��չ�ֶ� */
} MIS_REQ;

/* ���׽������ */
typedef struct _result RESULT_STRU;
struct _result {
	char	operateType[3];			/* ����ҵ������ */
	char	tradeType[3];			/* �������� */
	char	payType[3];				/* �������� */
	char	payMoney[19];				/* �����12.2 */
	char	operateStaff[13];		/* �տ�Ա�� */
	char	channelCode[13];		/* Ӫҵ������ */
	char	settleSerial[7];		/* �������� */
	char	partnerNbr[16];			/* �̻��� */
	char	partnerName[41];		/* �̻����� */
	char	terminalNbr[9];			/* �ն˺� */
	char	paymentNbr[20];			/* ֧���˺� */
	char	cardEffDate[5];			/* ����Ч�� */
	char	cardBankCode[9];		/* �����б��� */
	char	cardBankName[21];		/* ���������� */
	char	tradeDate[9];			/* �������� */
	char	tradeTime[7];			/* ����ʱ�� */
	char	authorizeNbr[13];		/* ��Ȩ�� */
	char	sysReferNbr[13];		/* ϵͳ�ο��� */
	char	paySerial[7];			/* ������ˮ�� */
	char	srcPaySerial[7];		/* ԭ������ˮ�� */
	char	systemSerial[7];		/* ϵͳ��ˮ�� */
	char	srcSystemSerial[7];		/* ԭϵͳ��ˮ�� */
	char	tradeMedia[2];			/* ���׽��� */
	char	extField[65];			/* Ԥ���ֶ� */
};
 
/* ���׷��� */
typedef struct {
	char		resultFlag[3];		/* ������ */
	char		requrestSerial[21];	/* ����������ˮ�� */
	char		errorMsg[101];		/* �������� */
	RESULT_STRU	resultContent;		/* ���׽��ʵ����� */
	char        status[2];
} MIS_RES;

#define		MAX_REQ_NUM	10

#if 0
#ifndef DECLARE_REQ_DATA
#define DECLARE_REQ_DATA
	LPOLESTR reqName[MAX_REQ_NUM] = {
		L"requestSerial",
		L"operateType",
		L"tradeType",
		L"payType",
		L"operateStaff",
		L"channelCode",
		L"paySerial",
		L"systemSerial",
		L"payMoney",
		L"extField"
	};
#endif
#endif

#endif
