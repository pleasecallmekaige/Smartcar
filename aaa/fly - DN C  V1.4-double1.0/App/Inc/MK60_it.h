/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_it.h
 * @brief      ɽ��K60 ƽ̨�жϷ����ض���ͷ�ļ�
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-06-26
 */


#ifndef __MK60_IT_H__
#define __MK60_IT_H__

/*                          ���¶����ж�������
 *  ��ȡ��Ĭ�ϵ��ж�����Ԫ�غ궨��        #undef  VECTOR_xxx
 *  �����¶��嵽�Լ���д���жϺ���      #define VECTOR_xxx    xxx_IRQHandler
 *  ���磺
 *       #undef  VECTOR_003                         ��ȡ��ӳ�䵽�ж�����������жϺ�����ַ�궨��
 *       #define VECTOR_003    HardFault_Handler    ���¶���Ӳ���Ϸ��жϷ�����
 */


//#undef  VECTOR_16   
//#define VECTOR_16       DMA0_IRQHandler        
//   
//   
//#undef  VECTOR_84       
//#define VECTOR_84       PIT0_IRQHandler
//
//
//#undef  VECTOR_87       
//#define VECTOR_87       PIT3_IRQHandler
//
//#undef  VECTOR_103
//#define VECTOR_103      PORTA_IRQHandler

//#undef  VECTOR_105
//#define VECTOR_105      PORTC_IRQHandler
   
   
   
extern void DMA8_IRQHandler(void);  
extern void PIT0_IRQHandler(void);  
extern void PIT2_IRQHandler(void);
extern void PIT3_IRQHandler(void);
extern void PORTA_IRQHandler(void);          
extern void PORTB_IRQHandler(); 
extern void PORTC_IRQHandler(); 
extern void PORTD_IRQHandler(); 
extern void PORTE_IRQHandler();
extern void uart1_handler(void);

#endif  //__MK60_IT_H__
