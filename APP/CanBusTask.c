#include "main.h"
#include "stdio.h"

static uint32_t can_count = 0;

volatile Encoder CM1Encoder = {0,0,0,0,0,0,0,0,0};
volatile Encoder CM2Encoder = {0,0,0,0,0,0,0,0,0};
volatile Encoder CM3Encoder = {0,0,0,0,0,0,0,0,0};
volatile Encoder CM4Encoder = {0,0,0,0,0,0,0,0,0};
volatile Encoder GMYawEncoder = {0,0,0,0,0,0,0,0,0};
volatile Encoder GMPitchEncoder = {0,0,0,0,0,0,0,0,0};
float ZGyroModuleAngle = 0.0f;
/*
***********************************************************************************************
*Name          :GetEncoderBias
*Input         :can message
*Return        :void
*Description   :to get the initiatial encoder of the chassis motor 201 202 203 204
*
*
***********************************************************************************************
*/

void GetEncoderBias(volatile Encoder *v, CanRxMsg * msg)
{

            v->ecd_bias = (msg->Data[0]<<8)|msg->Data[1];  //�����ʼ������ֵ��Ϊƫ��  
            v->ecd_value = v->ecd_bias;
            v->last_raw_value = v->ecd_bias;
            v->temp_count++;
}

/*
***********************************************************************************************
*Name          :EncoderProcess
*Input         :can message
*Return        :void
*Description   :to get the initiatial encoder of the chassis motor 201 202 203 204
*
*
***********************************************************************************************
*/
void EncoderProcess(volatile Encoder *v, CanRxMsg * msg)
{
	int i=0;
	int32_t temp_sum = 0;    
	v->last_raw_value = v->raw_value;
	v->raw_value = (msg->Data[0]<<8)|msg->Data[1];
	v->diff = v->raw_value - v->last_raw_value;
	if(v->diff < -7500)    //���α������ķ���ֵ���̫�󣬱�ʾȦ�������˸ı�
	{
		v->round_cnt++;
		v->ecd_raw_rate = v->diff + 8192;
	}
	else if(v->diff>7500)
	{
		v->round_cnt--;
		v->ecd_raw_rate = v->diff- 8192;
	}		
	else
	{
		v->ecd_raw_rate = v->diff;
	}
	//����õ������ı��������ֵ
	v->ecd_value = v->raw_value + v->round_cnt * 8192;
	//����õ��Ƕ�ֵ����Χ���������
	v->ecd_angle = (float)(v->raw_value - v->ecd_bias)*360/8192 + v->round_cnt * 360;
	v->rate_buf[v->buf_count++] = v->ecd_raw_rate;
	if(v->buf_count == RATE_BUF_SIZE)
	{
		v->buf_count = 0;
	}
	//�����ٶ�ƽ��ֵ
	for(i = 0;i < RATE_BUF_SIZE; i++)
	{
		temp_sum += v->rate_buf[i];
	}
	v->filter_rate = (int32_t)(temp_sum/RATE_BUF_SIZE);					
}

/*
************************************************************************************************************************
*Name        : CanReceiveMsgProcess
* Description: This function process the can message representing the encoder data received from the CAN2 bus.
* Arguments  : msg     is a pointer to the can message.
* Returns    : void
* Note(s)    : none
************************************************************************************************************************
*/
void CanReceiveMsgProcess(CanRxMsg * msg)
{      
        //GMYawEncoder.ecd_bias = yaw_ecd_bias;
        can_count++;
		switch(msg->StdId)
		{
				case CAN_BUS2_MOTOR1_FEEDBACK_MSG_ID:
				{
					//LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_MOTOR1));
					(can_count<=50) ? GetEncoderBias(&CM1Encoder ,msg):EncoderProcess(&CM1Encoder ,msg);       //��ȡ���������ĳ�ʼƫ��ֵ            
                    
				}break;
				case CAN_BUS2_MOTOR2_FEEDBACK_MSG_ID:
				{
					//LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_MOTOR2));
					(can_count<=50) ? GetEncoderBias(&CM2Encoder ,msg):EncoderProcess(&CM2Encoder ,msg);
				}break;
				case CAN_BUS2_MOTOR3_FEEDBACK_MSG_ID:
				{
					//LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_MOTOR3));
					(can_count<=50) ? GetEncoderBias(&CM3Encoder ,msg):EncoderProcess(&CM3Encoder ,msg);   
				}break;
				case CAN_BUS2_MOTOR4_FEEDBACK_MSG_ID:
				{
					//LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_MOTOR4));
				 	(can_count<=50) ? GetEncoderBias(&CM4Encoder ,msg):EncoderProcess(&CM4Encoder ,msg);
				}break;
				case CAN_BUS1_ZGYRO_FEEDBACK_MSG_ID:
				{
					//LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_ZGYRO));
					ZGyroModuleAngle = -0.01f*((int32_t)(msg->Data[0]<<24)|(int32_t)(msg->Data[1]<<16) | (int32_t)(msg->Data[2]<<8) | (int32_t)(msg->Data[3])); 
				}break;
				default:
				{
				}
		}
		// check if deadlock, meeans the yaw angle is overflow //time should keep for a long time to avoid bug		
			if(!LostCounterOverflowCheck(fabs(GMYawEncoder.ecd_angle), 70.0f) || GetWorkState() == STOP_STATE)  //�����ֹͣģʽ��һֱι����ֹ��������ʧ��
			{
				//LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_DEADLOCK));
			}		 
}

/********************************************************************************
   �����̵���巢��ָ�ID��Ϊ0x200�������̷���IDΪ0x201-0x204
*********************************************************************************/
void Set_CM_Speed(CAN_TypeDef *CANx, int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq)
{
//    CanTxMsg tx_message;
//    tx_message.StdId = ;
//    tx_message.IDE = CAN_Id_Standard;
//    tx_message.RTR = CAN_RTR_Data;
//    tx_message.DLC = 0x08;
//    
//    tx_message.Data[0] = ;
//    tx_message.Data[1] = ��
//    tx_message.Data[2] = ��
//    tx_message.Data[3] = ��
//    tx_message.Data[4] = ��
//    tx_message.Data[5] = ��
//    tx_message.Data[6] = ��
//    tx_message.Data[7] = ��
//    CAN_Transmit(CANx,&tx_message);
}

/********************************************************************************
   ������巢��ָ�ID��Ϊ0x1FF��ֻ����������壬���ݻش�IDΪ0x205��0x206
	 cyq:����Ϊ�������������ָ�
*********************************************************************************/
void Set_Gimbal_Current(CAN_TypeDef *CANx, int16_t gimbal_yaw_iq, int16_t gimbal_pitch_iq)
{
    CanTxMsg tx_message;    
    tx_message.StdId = 0x1FF;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    
    tx_message.Data[0] = (unsigned char)(gimbal_yaw_iq >> 8);
    tx_message.Data[1] = (unsigned char)gimbal_yaw_iq;
    tx_message.Data[2] = (unsigned char)(gimbal_pitch_iq >> 8);
    tx_message.Data[3] = (unsigned char)gimbal_pitch_iq;
    tx_message.Data[4] = 0x00;
    tx_message.Data[5] = 0x00;
    tx_message.Data[6] = 0x00;
    tx_message.Data[7] = 0x00;
    CAN_Transmit(CANx,&tx_message);
}