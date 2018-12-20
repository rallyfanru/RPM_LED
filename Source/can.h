#ifndef CAN_INIT_H
#define CAN_INIT_H
#include "stm32f0xx.h"
#include "timer.h"

#define DATA_CAN			(CAN_TypeDef*)CAN

//Включена фильтрация CAN пакетов
#define CAN_ENABLE_FILTERS  1
/*
http://www.bittiming.can-wiki.info/
*/
#define CAN_BTR_48MHz_500kb					(uint32_t)0x001c0005
#define CAN_BTR_48MHz_1000kb				(uint32_t)0x001c0002


/*
 * PIDS
 */
#define OBD_REQ   (uint16_t)0x7DF
#define OBD_ANS   (uint16_t)0x7E8
#define SET_BRI_ID (uint16_t)0x101
#define RET_BRI_ID (uint16_t)0x102

/*
 * OBDII
 */
#define ENGINE_RPM          0x0C


#define MAX_RPM	 (uint32_t)4000


struct can_message
{
	 uint16_t ID;
	 uint8_t  msg[8];
};


extern volatile uint16_t rpm;
extern volatile uint8_t can_alive;
extern const struct can_message canreq_rpm;
extern volatile uint8_t can_send_ok;

void can_init(void);
void can_send(CAN_TypeDef* can, uint32_t id, uint8_t *msg);


#endif
