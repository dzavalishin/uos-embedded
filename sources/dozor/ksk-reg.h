/*
 * Описание регистров контроллера КСК.
 *
 * Автор: Дмитрий Подхватилин, НПП "Дозор" 2011
 */
 
#ifndef _KSK_REG_H_
#define _KSK_REG_H_

/* !!! ВНИМАНИЕ !!!
 *
 * Для использования данного заголовочного файла необходимо задать
 * базовый адрес КСК в адресном пространстве процессора - константу 
 * KSK_MEM_BASE перед включением данного файла в текст программы. 
 * Например,
 * #define KSK_MEM_BASE	0x10000000
 * #include <dozor/ksk-reg.h>
 */

/* Структура области передатчика */
struct ksk_tx_area {
	uint32_t	gcr;			/* Регистр управления */
	uint32_t	grup1;			/* Параметры первой группы */
	uint32_t	grup2;			/* Параметры второй группы */
	uint32_t	word_num_overall;	/* Общая длина первой и второй групп */
	uint32_t	reserve0;		/* Резерв */
	uint32_t	tssr_ch1;		/* Статус передатчика первого канала */
	uint32_t	tssr_ch2;		/* Статус передатчика второго канала */
	uint32_t	tssr_ch3;		/* Статус передатчика третьего канала */
	uint32_t	reserve1;		/* Резерв */
	uint32_t	tpc_ch1;		/* Счетчик переданных слов первого канала */
	uint32_t	tpc_ch2;		/* Счетчик переданных слов второго канала */
	uint32_t	tpc_ch3;		/* Счетчик переданных слов третьего канала */
	uint32_t	reserve2;		/* Резерв */
	uint32_t	rvec_ch1;		/* Шкала активных устройств в первом канале */
	uint32_t	rvec_ch2;		/* Шкала активных устройств во втором канале */
	uint32_t	rvec_ch3;		/* Шкала активных устройств в третьем канале */
	unsigned	data [512 - 16];	/* Данные для передачи */
} __attribute__((packed));

#define KSK_GRUP_WORDS_PER_ROUND(n)	((n) & 0x3FF)		/* Количество слов, выдаваемых в одном раунде TDMA */
#define KSK_GRUP_WORDS_PER_CYCLE(n)	(((n) & 0x3FF) << 10)	/* Количество слов, выдаваемых в одном цикле системы */
#define KSK_GRUP_ADDRESS(n)		(((n) & 0x3FF) << 20)	/* Стартовый адрес данных, от начала области передатчика */

/* Регистр управления GCR */
#define KSK_GCR_RESET		(1 << 0)		/* Программный сброс */
#define KSK_GCR_BLOCK_TX	(1 << 1)		/* Блокировка выдачи в каналы в тестовом режиме */
/* Типы прерываний */
#define KSK_INTR_WINDOW_ERR	(0 << 2)	/* Расхождение сигналов разрешения выдачи в канал ПЛИС1 и ПЛИС2 более 3 тактов */
#define KSK_INTR_TX_DONE	(1 << 2)	/* Прерывание по концу передачи */
#define KSK_INTR_RX_DONE	(2 << 2)	/* Прерывание по концу приема */
#define KSK_INTR_END_CYCLE	(3 << 2)	/* Прерывание по концу цикла системы */
/* Источники DEBUG */
#define KSK_DBG_CTRL		(0 << 4)	/* Выдача DEBUG из модуля управления */
#define KSK_DBG_TX1		(1 << 4)	/* Выдача DEBUG из передатчика первого канала */
#define KSK_DBG_TX2		(2 << 4)	/* Выдача DEBUG из передатчика второго канала */
#define KSK_DBG_TX3		(3 << 4)	/* Выдача DEBUG из передатчика третьего канала */
#define KSK_DBG_ARB		(4 << 4)	/* Выдача DEBUG из модуля арбитра */
#define KSK_DBG_RX1		(5 << 4)	/* Выдача DEBUG из приемника первого канала */
#define KSK_DBG_RX2		(6 << 4)	/* Выдача DEBUG из приемника второго канала */
#define KSK_DBG_RX3		(7 << 4)	/* Выдача DEBUG из приемника третьего канала */

/* Статусы передатчиков */
#define KSK_TX_FIN		(1 << 0)			/* Передача окончена */
#define KSK_TX_T2_FIN		(1 << 1)			/* Передача слова типа 2 окончена */
#define KSK_ECHO_PLACE_OK	(1 << 2)			/* Эхоконтроль номера места прошел */
#define KSK_ECHO_DATA_OK	(1 << 3)			/* Эхоконтроль данных прошел */
#define KSK_ECHO_PARITY_ERR	(1 << 4)			/* Ошибка эхоконтроля чётности */
#define KSK_WINDOW_ERR		(1 << 5)			/* Ошибка - несовпадение окон ПЛИС1 и ПЛИС2 */
#define KSK_TX_SYNC_MAIN	(1 << 6)			/* Завершена выдача основного синхрослова */
#define KSK_TX_SYNC_RSRV	(1 << 7)			/* Завершена выдача резервного синхрослова */
#define KSK_TEST_MODE		(1 << 10)			/* Признак тестового режима */
#define KSK_PLACE_PARITY	(1 << 11)			/* Контрольный разряд номера места */
#define KSK_GET_PLACE(tssr)	(((tssr) >> 12) & 0xF)		/* Номер места */
#define KSK_GET_CTR_T2_TX(tssr)	(((tssr) >> 16) & 0xFFFF)	/* Значение таймера (CTR) в момент окончания выдачи слота типа 2 */

/* Указатель на управляющую область */
#define KSK_TX_AREA		((struct ksk_tx_area *) KSK_MEM_BASE)

/* Структура области приема от n-го узла */
struct ksk_rx_area {
	uint32_t	reserve0 [5];		/* Резерв */
	uint32_t	rssr_ch1;		/* Статус приемника первого канала */
	uint32_t	rssr_ch2;		/* Статус приемника второго канала */
	uint32_t	rssr_ch3;		/* Статус приемника третьего канала */
	uint32_t	reserve1;		/* Резерв */
	uint32_t	rpc_ch1;		/* Счетчик успешно принятых слов первого канала */
	uint32_t	rpc_ch2;		/* Счетчик успешно принятых слов второго канала */
	uint32_t	rpc_ch3;		/* Счетчик успешно принятых слов третьего канала */
	uint32_t	reserve2;		/* Резерв */
	uint32_t	evc_ch1;		/* Счетчик слов, принятых в первом канале с ошибками */
	uint32_t	evc_ch2;		/* Счетчик слов, принятых во втором канале с ошибками */
	uint32_t	evc_ch3;		/* Счетчик слов, принятых в третьем канале с ошибками */	
	unsigned	data [512 - 16];	/* Принятые данные */
} __attribute__((packed));

/* Область приема от n-го узла */
#define KSK_RX_AREA(n)	((struct ksk_rx_area *) (KSK_MEM_BASE + (n) * 512 * 4))

#define KSK_MAGIC		0x20111027	/* Магическое число, идентифицирующее мезонин КСК */

/* Структура области циклограммы */
struct ksk_cycl_area {
	uint32_t	mtr;			/* Длина раунда TDMA */
	uint32_t	sync;			/* Номера основных и резервных устройств */
	uint32_t	sync_times;		/* Окна выдачи синхрослов - слов типа 1 */
	uint32_t	magic;			/* Сюда КСК запишет магическое число при включении питания */
	uint32_t	reserve0 [12];		/* Резерв */
	uint32_t	windows [16];		/* Окна выдачи, индекс - номер узла, windows[0] - не используется */
} __attribute__((packed));

/* Макросы для задания номеров устройств, выдающих синхрослова */
#define KSK_SYNC_MAIN_CH1(n)	((n) & 0xF)		/* Номер узла, выдающего основное синхрослово в первом канале */
#define KSK_SYNC_RSRV_CH1(n)	(((n) & 0xF) << 4)	/* Номер узла, выдающего резерное синхрослово в первом канале */
#define KSK_SYNC_MAIN_CH2(n)	(((n) & 0xF) << 8)	/* Номер узла, выдающего основное синхрослово во втором канале */
#define KSK_SYNC_RSRV_CH2(n)	(((n) & 0xF) << 12)	/* Номер узла, выдающего резерное синхрослово во втором канале */
#define KSK_SYNC_MAIN_CH3(n)	(((n) & 0xF) << 16)	/* Номер узла, выдающего основное синхрослово в третьем канале */
#define KSK_SYNC_RSRV_CH3(n)	(((n) & 0xF) << 20)	/* Номер узла, выдающего резерное синхрослово в третьем канале */

/* Макросы для задания окон выдачи синхрослов */
#define KSK_SYNC_MAIN_BEGIN(t)	((t) & 0xFF)		/* Начало выдачи основного синхрослова */
#define KSK_SYNC_RSRV_BEGIN(t)	(((t) & 0xFF) << 8)	/* Начало выдачи резервного синхрослова */
#define KSK_SYNC_MAIN_END(t)	(((t) & 0xFF) << 16)	/* Конец выдачи основного синхрослова */
#define KSK_SYNC_RSRV_END(t)	(((t) & 0xFF) << 24)	/* Конец выдачи резервного синхрослова */

/* Макросы для задания окон выдачи данных */
#define KSK_DATA_WIND_BEGIN(t)	((t) & 0xFFFF)		/* Начало окна выдачи данных */
#define KSK_DATA_WIND_END(t)	(((t) & 0xFFFF) << 16)	/* Конец окна выдачи данных */

/* Область циклограммы, используется только в тестовом режиме 
 * n - собственный номер узла
 */
#define KSK_CYCL_AREA(n)	((struct ksk_cycl_area *) (KSK_MEM_BASE + (n) * 512 * 4))

#endif /*_KSK_REG_H_*/

