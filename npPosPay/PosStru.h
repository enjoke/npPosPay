#ifndef _POS_STRU_H_
#define _POS_STRU_H_

/* 交易请求接收容器 */
typedef struct {
	char	requestSerial[23];		/* 请求流水号 */
	char	operateType[3];			/* 操作业务类型 */
	char	tradeType[3];			/* 交易类型 */
	char	payType[3];				/* 付款类型 */
	char	operateStaff[13];		/* 收款员号 */
	char	channelCode[13];		/* 营业厅编码 */
	char	paySerial[7];			/* 收银流水号 */
	char	systemSerial[7];		/* 系统流水号 */
	char	payMoney[19];			/* 付款金额，12.2 */
	char	extField[65];			/* 扩展字段 */
} MIS_REQ;

/* 交易结果对象 */
typedef struct _result RESULT_STRU;
struct _result {
	char	operateType[3];			/* 操作业务类型 */
	char	tradeType[3];			/* 交易类型 */
	char	payType[3];				/* 付款类型 */
	char	payMoney[19];				/* 付款金额，12.2 */
	char	operateStaff[13];		/* 收款员号 */
	char	channelCode[13];		/* 营业厅编码 */
	char	settleSerial[7];		/* 结算批次 */
	char	partnerNbr[16];			/* 商户号 */
	char	partnerName[41];		/* 商户名称 */
	char	terminalNbr[9];			/* 终端号 */
	char	paymentNbr[20];			/* 支付账号 */
	char	cardEffDate[5];			/* 卡有效期 */
	char	cardBankCode[9];		/* 发卡行编码 */
	char	cardBankName[21];		/* 发卡行名称 */
	char	tradeDate[9];			/* 交易日期 */
	char	tradeTime[7];			/* 交易时间 */
	char	authorizeNbr[13];		/* 授权号 */
	char	sysReferNbr[13];		/* 系统参考号 */
	char	paySerial[7];			/* 收银流水号 */
	char	srcPaySerial[7];		/* 原收银流水号 */
	char	systemSerial[7];		/* 系统流水号 */
	char	srcSystemSerial[7];		/* 原系统流水号 */
	char	tradeMedia[2];			/* 交易介质 */
	char	extField[65];			/* 预留字段 */
};
 
/* 交易返回 */
typedef struct {
	char		resultFlag[3];		/* 返回码 */
	char		requrestSerial[21];	/* 交易请求流水号 */
	char		errorMsg[101];		/* 错误描述 */
	RESULT_STRU	resultContent;		/* 交易结果实体对象 */
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
