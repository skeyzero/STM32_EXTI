# STM32_EXTI
STM32 EXTI

![image](https://github.com/skeyzero/STM32_EXTI/raw/master/img/1.png)
关于STM32 外部中断处理


`
	RCC->CFGR&=~(1<<17);			//
	RCC->CFGR|=1<<16;				//    ??HSE??PLL???
	FLASH->ACR|=0x32;				//¸ù¾ÝFLASH±à³ÌÊÖ²á£¬ÕâÀïÓ¦¸ÃÊÇÑÓÊ±Á½¸öÊ±ÖÓ£¬Èç¹ûÅäÖÃ´íÎó³ÌÐò¶ÁÐ´¾Í²»»áÕý³££¬³ÌÐò¾Í²»ÄÜÕý³£ÔËÐÐ
	
	RCC->CFGR|=7<<18;				//9±¶Æµ
	
	RCC->CFGR&=0xffffff0c;			//Çå³ýAHBÎ»£¬Ê¹AHB²»·ÖÆµ
	RCC->CFGR|=0x00000002;//PLL×÷ÎªÏµÍ³Ê±ÖÓ

	RCC->CR|=1<<24;//enable pll
	while((RCC->CR&1<<25)==0);//pll ready
`
