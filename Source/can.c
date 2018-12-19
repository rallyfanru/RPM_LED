#include "can.h"

volatile uint16_t rpm=0;
volatile uint8_t can_alive=0;


const struct can_message canreq_rpm     = {OBD_REQ, {0x02,0x01,ENGINE_RPM,00,00,00,00,00} };

void CEC_CAN_IRQHandler(void){
	struct can_message CAN_MSG;

	CAN_MSG.ID=(uint16_t)((CAN->sFIFOMailBox[0].RIR >> 21) & 0xFFFF);

	CAN_MSG.msg[0]=(uint8_t) (CAN->sFIFOMailBox[0].RDLR & 0xFF);
	CAN_MSG.msg[1]=(uint8_t)((CAN->sFIFOMailBox[0].RDLR >> 8) & 0xFF);
	CAN_MSG.msg[2]=(uint8_t)((CAN->sFIFOMailBox[0].RDLR >> 16) & 0xFF);
	CAN_MSG.msg[3]=(uint8_t)((CAN->sFIFOMailBox[0].RDLR >> 24) & 0xFF);

	CAN_MSG.msg[4]=(uint8_t) (CAN->sFIFOMailBox[0].RDHR & 0xFF);

	CAN_MSG.msg[5]=(uint8_t)((CAN->sFIFOMailBox[0].RDHR >> 8) & 0xFF);
	CAN_MSG.msg[6]=(uint8_t)((CAN->sFIFOMailBox[0].RDHR >> 16) & 0xFF);
	CAN_MSG.msg[7]=(uint8_t)((CAN->sFIFOMailBox[0].RDHR >> 24) & 0xFF);


	CAN->RF0R |= CAN_RF0R_RFOM0;

	switch(CAN_MSG.ID){

	case SET_BRI_ID:
		set_brihtness((uint16_t)( (CAN_MSG.msg[1]<<8) | CAN_MSG.msg[0] ));
		can_send(CAN,(uint16_t)RET_BRI_ID,(uint8_t *)CAN_MSG.msg);
		break;

	case OBD_ANS:
		if(CAN_MSG.msg[2] == ENGINE_RPM) {
			rpm=(((uint16_t)CAN_MSG.msg[3] << 8)+CAN_MSG.msg[4])/4;
			can_alive=1;
			can_send(CAN,(uint16_t)canreq_rpm.ID,(uint8_t *)canreq_rpm.msg);
		}
		break;

	default:
		break;
	};




};



void can_send(CAN_TypeDef* can, uint32_t id, uint8_t *msg){
		uint8_t mailbox=0;

		mailbox = (can->TSR >> 24) & 0x3;

		can->sTxMailBox[mailbox].TDTR &= ~0xFF;
		can->sTxMailBox[mailbox].TDTR |= 8;
		can->sTxMailBox[mailbox].TIR = (id << 21);

		can->sTxMailBox[mailbox].TDLR = (uint32_t)msg[0] | (uint32_t)(msg[1] << 8) | (uint32_t)(msg[2] << 16) | (uint32_t)(msg[3] << 24);
		can->sTxMailBox[mailbox].TDHR = (uint32_t)msg[4] | (uint32_t)(msg[5] << 8) | (uint32_t)(msg[6] << 16) | (uint32_t)(msg[7] << 24);

		can->sTxMailBox[mailbox].TIR |= 1; //Go!
}



void can_init(void){
/*
CAN1:
PA11 - RX
PA12 - TX
*/
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	//MODE - 10 AF
	GPIOA->MODER |= GPIO_MODER_MODER11_1;
	GPIOA->MODER |= GPIO_MODER_MODER12_1;
	//Push-pull mode 0
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_11|GPIO_OTYPER_OT_12);
	//Very High speed 11
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR11_0|GPIO_OSPEEDER_OSPEEDR11_1;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12_0|GPIO_OSPEEDER_OSPEEDR12_1;
	//AF4
	GPIOA->AFR[1] |= (4 << GPIO_AFRH_AFSEL11_Pos);
	GPIOA->AFR[1] |= (4 << GPIO_AFRH_AFSEL12_Pos);

	//CAN
	RCC->APB1ENR |= RCC_APB1ENR_CANEN;

	CAN->MCR &= ~CAN_MCR_SLEEP;
	CAN->MCR |= CAN_MCR_INRQ; //Init mode
	while (!(CAN->MSR & CAN_MSR_INAK)) {};
	CAN->MCR &= ~CAN_MCR_TTCM;
	CAN->MCR |= CAN_MCR_ABOM;
	CAN->MCR |= CAN_MCR_AWUM;
	CAN->MCR &= ~CAN_MCR_NART;
	CAN->MCR &= ~CAN_MCR_RFLM;
	CAN->MCR |= CAN_MCR_TXFP;

	CAN->BTR = 0;
	//Test mode
	//CAN1->BTR |= CAN_BTR_SILM| CAN_BTR_LBKM;
	CAN->BTR |= CAN_BTR_48MHz_500kb;

	//Filters

#if defined CAN_ENABLE_FILTERS
		CAN->FMR |= CAN_FMR_FINIT;
		CAN->FM1R |= CAN_FM1R_FBM0;		 //Identifier List Mode для фильтра 0
		//CAN->FM1R |= CAN_FM1R_FBM1;	 	 //Identifier List Mode для фильтра 1

		//16bit
		//CAN->FS1R &= ~CAN_FS1R_FSC0; 	 //1-32bit 0-16bit ID mode для фильтра 0
		//CAN->sFilterRegister[0].FR1=((uint16_t)SET_BRI_ID << 21)|((uint16_t)OBD_ANS << 5);		//  2*16-bit ID
		//CAN->sFilterRegister[0].FR2=0;

		//32bit
		CAN->FS1R |= CAN_FS1R_FSC0; 	 //1-32bit 0-16bit ID mode для фильтра 0
		CAN->sFilterRegister[0].FR1=(uint32_t)SET_BRI_ID << 21;
		CAN->sFilterRegister[0].FR2=(uint32_t)OBD_ANS << 21;

		/*
		 * Выравнивание ID по левому краю, поэтому если в одном фильтре 4 16-bit ID, то смещения 5 и 21 влево,
		 * а если в фильтре два 32-bit ID то смещение на 21 у обоих.
		 * Если фильтр пустой (с ID 0), но включен, сообщение с ID=0 будет проходить.
		 */

//		CAN->FS1R &= ~CAN_FS1R_FSC1; 	 //1-32bit 0-16bit ID mode для фильтра 0
//		CAN->sFilterRegister[1].FR1=0;
//		CAN->sFilterRegister[1].FR2=0;

		CAN->FFA1R = 0;															//использовать FIFO0
		CAN->FA1R |= CAN_FA1R_FACT0; 											//Enable filter 0
		//CAN->FA1R |= CAN_FA1R_FACT1; 											//Enable filter 1
		CAN->FMR &= ~CAN_FMR_FINIT;												//закончить настройку фильтров
#else
		CAN->FMR |= CAN_FMR_FINIT;
		CAN->FM1R &= ~CAN_FM1R_FBM0; //Mask Mode
		CAN->FS1R |= CAN_FS1R_FSC0; //32bit mode
		CAN->sFilterRegister[0].FR1=0x0;
		CAN->sFilterRegister[0].FR2=0x0;
		CAN->FA1R |= CAN_FA1R_FACT0; //Enable filter
		CAN->FMR &= ~CAN_FMR_FINIT;
#endif

	//Interrupts
	//CAN->IER |= CAN_IER_TMEIE;
	//CAN->IER |= CAN_IER_FFIE0 | CAN_IER_FMPIE0|  CAN_IER_FOVIE0;
	CAN->IER |= CAN_IER_FMPIE0;

	CAN->MCR &= ~CAN_MCR_INRQ; //Init mode off
	//while (CAN1->MSR & CAN_MSR_INAK) {};

	NVIC_EnableIRQ(CEC_CAN_IRQn);


}
