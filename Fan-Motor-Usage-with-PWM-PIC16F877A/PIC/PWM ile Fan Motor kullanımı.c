/*                                                                            /
/  Bu projemizde mikrodenetleyicimiz olan PIC16F877A ile LM35 Sýcaklýk        /
/ sensöründen okuduðumuz sýcaklýk deðeri ile birlikde DC-FAN Motorumuzu       /
/ PWM kullanarak süreceðiz.                                                   /
/                                                                             /
/  A0 Pinine Baðlý LM35 sýcaklýk sensöründen sýcaklýk deðerini okuyacaðýz     /
/ sýcaklýk deðerimiz eðer 20 Derece altýnda ise fanýmýz çalýþmayacak          /
/ 20 ile 30 Derece arasýnda C2 Pinine baðlý Transistör ile Fan hýzýmýz        /                                                                       
/ Sýcaklýk Sensöründen gelen deðere göre linner bir þekilde artacak.          /                                                                     
/ sýcaklýk deðerimiz 30 Derece olduðunda ise fan hýzýmýz tam kapasite         /                                                                     
/ olarak çalýþacak                                                            /
*/  
// Linkedin:mrtbeytullah 

#include <PWM ile Fan Motor kullanýmý.h>

#define LCD_ENABLE_PIN PIN_E2
#define LCD_RS_PIN PIN_E0
#define LCD_RW_PIN PIN_E1
#define LCD_DATA4 PIN_D4
#define LCD_DATA5 PIN_D5
#define LCD_DATA6 PIN_D6
#define LCD_DATA7 PIN_D7
#include <lcd.c>  

int16 Duty,ADC_Deger;  
float Vadc,Duty_Yuzde,Sicaklik;

void main()
{                                                                                                                                            
   setup_adc_ports(AN0); //ADC ayarlamasý yapýyoruz
   setup_adc(ADC_CLOCK_DIV_16);
   set_adc_channel(0);      
   delay_us(20);  
     
   setup_timer_2(T2_DIV_BY_16,255,1);      //Timer ayarlamalarýmýzý yapýyoruz
   setup_ccp2(CCP_PWM);                   //CCP2'yi PWM olarak kullanacaðýz
              
   lcd_init();         

   while(TRUE)  
   { 
      ADC_Deger=read_adc();               
      Vadc=ADC_Deger*0.0048875855;       
      Sicaklik=(Vadc*100); /*LM35 Sýcaklýk Sensöründe her 10mV 'luk her artýþta/ 
                          /1 Derece artmaktadýr dolayýsýyla gerilim deðerini/   
                         /100 ile çarparak sýcaklýðý ölçebiliyoruz */
      
     if(sicaklik<20)
      { 
       Duty=0;  
       Duty_Yuzde=Duty/10.24; 
       set_pwm2_duty(Duty);                                                                  
       printf(lcd_putc,"\fDuty_Yuzde=%f\nSicaklik=%2.1f",Duty_Yuzde,Sicaklik);
       Delay_ms(100);  
      }  
      
     else if(Sicaklik>=20 && Sicaklik<=30)
      { 
        Duty=(Sicaklik*(512/5))-((512/5)*20);/*Duty hesabý yapar iken 20-30 Derece arasý fan hýzýmýz 
                linner olarak artacaðýndan hesap yapar iken eðim denklemi kullanarak hesap yapýyoruz*/  
        Duty_Yuzde=Duty/10.24;  
        set_pwm2_duty(Duty);                                                                      
        printf(lcd_putc,"\fDuty_Yuzde=%f\nSicaklik=%2.1f",Duty_Yuzde,Sicaklik);
        Delay_ms(100);  
      }  
      
     else if(Sicaklik>30)
      {   
         Duty=1023;  
         Duty_Yuzde=Duty/10.24; 
         set_pwm2_duty(Duty);   
         printf(lcd_putc,"\fDuty_Yuzde=%f\nSicaklik=%2.1f",Duty_Yuzde,Sicaklik);
         Delay_ms(100);  
      }
     
   }             
                  
}
