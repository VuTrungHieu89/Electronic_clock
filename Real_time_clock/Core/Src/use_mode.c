/*
 * use_mode.c
 *
 *  Created on: Apr 26, 2024
 *      Author: Admin
 */

#include "use_mode.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
uint8_t flag = 0;
uint8_t flag1 = 0;
uint8_t flag2 = 0;
uint8_t flag3 = 0;
uint8_t flag4 = 0;
uint8_t blink = 0;
uint32_t tick = 0;
uint32_t cnt = 0;
static uint8_t cursor_row = 1;
static uint8_t cursor_col = 7;
static uint8_t date_set = 0;
static uint8_t month_set = 0;
static uint32_t year_set = 24;
static uint8_t dow_set = 1;
static uint8_t hour_set = 0;
static uint8_t minute_set = 0;
static uint8_t second_set = 0;
extern uint8_t mode;
uint8_t light;
uint8_t Alarm_flag = 0;
uint8_t hour_3 = 0;
uint8_t minute_3 = 0;
uint8_t second_3 = 0;



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if (GPIO_Pin == GPIO_PIN_1){
		mode++;
		if(mode > 3)mode = 0;
	}
	if (GPIO_Pin == GPIO_PIN_4){
		flag = 1;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM1){
		second_3++;
		if(second_3 > 59)second_3 = 0;
		if(second_3 > 58){
			minute_3++;
		}
		if(minute_3 > 58){
			minute_3 = 0;
			hour_3++;
		}
	}
}

//mode: 0
void Display(){
	uint8_t date = DS1307_GetDate();
	uint8_t month = DS1307_GetMonth();
	uint16_t year = DS1307_GetYear();
	uint8_t dow = DS1307_GetDayOfWeek();
	uint8_t hour = DS1307_GetHour();
	uint8_t minute = DS1307_GetMinute();
	uint8_t second = DS1307_GetSecond();
	//uint8_t zone_hr = DS1307_GetTimeZoneHour();
	//uint8_t zone_min = DS1307_GetTimeZoneMin();
	char *DAYS_OF_WEEK[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
	char str[10];
	HD44780_Clear();
	HD44780_Cursor();
	HD44780_SetCursor(0, 0);
	HD44780_PrintStr(DAYS_OF_WEEK[dow-1]);
	HD44780_PrintSpecialChar(47);
	sprintf(str, "%d", date);
	HD44780_PrintStr(str);
	HD44780_PrintSpecialChar(47);
	sprintf(str, "%d", month);
	HD44780_PrintStr(str);
	HD44780_PrintSpecialChar(47);
	sprintf(str, "%d", year);
	HD44780_PrintStr(str);

	HD44780_SetCursor(0, 1);
	sprintf(str, "%d", hour);
	HD44780_PrintStr(str);
	HD44780_PrintSpecialChar(0x3a);
	sprintf(str, "%d", minute);
	HD44780_PrintStr(str);
	HD44780_PrintSpecialChar(0x3a);
	sprintf(str, "%d", second);
	HD44780_PrintStr(str);
	HD44780_NoCursor();

	HAL_Delay(1000);
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 1){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 1){
			while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 1);
			light = ~light;
			if(light == 0)HD44780_NoBacklight();
			if(light != 0)HD44780_Backlight();
			blink = 0;
		}
	}
	if (Alarm_flag == 1){
		if (hour == hour_set){
			if(minute == minute_set){
				if(second == second_set){
					Alarm_flag = 0;
					blink = 1;
				}
			}
		}
	}
	if (blink == 1){
		cnt++;
		if(cnt > 100)cnt = 0;
		if(cnt < 50)HD44780_NoBacklight();
		if(cnt > 50)HD44780_Backlight();
	}

}

//mode:1
void Configuration(){
	char str[10];
	const char *DAYS_OF_WEEK[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
	HD44780_Clear();
	HD44780_Cursor();
	HD44780_SetCursor(0, 0);
	HD44780_PrintStr(DAYS_OF_WEEK[dow_set-1]);
	HD44780_PrintSpecialChar(47);
	sprintf(str, "%d", date_set);
	HD44780_PrintStr(str);
	HD44780_PrintSpecialChar(47);
	sprintf(str, "%d", month_set);
	HD44780_PrintStr(str);
	HD44780_PrintSpecialChar(47);
	sprintf(str, "%ld", year_set);
	HD44780_PrintStr(str);

	HD44780_SetCursor(0, 1);
	sprintf(str, "%d", hour_set);
	HD44780_PrintStr(str);
	HD44780_PrintSpecialChar(0x3a);
	sprintf(str, "%d", minute_set);
	HD44780_PrintStr(str);
	HD44780_PrintSpecialChar(0x3a);
	sprintf(str, "%d", second_set);
	HD44780_PrintStr(str);
	HD44780_SetCursor(cursor_col, cursor_row);
	HD44780_Blink();

	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 1){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 1){
			while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 1);
			flag1++;
			if(flag1 > 7)flag1 = 1;
			if(flag1 == 1){
				cursor_row = 1;
				cursor_col = 5;
			}
			if(flag1 == 2){
				cursor_row = 1;
				cursor_col = 3;
			}
			if(flag1 == 3){
				cursor_row = 1;
				cursor_col = 0;
			}
			if(flag1 == 4){
				cursor_row = 0;
				cursor_col = 10;
			}
			if(flag1 == 5){
				cursor_row = 0;
				cursor_col = 6;
			}
			if(flag1 == 6){
				cursor_row = 0;
				cursor_col = 4;
			}
			if(flag1 == 7){
				cursor_row = 0;
				cursor_col = 2;
			}
		}
	}

	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 1){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 1){
			while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 1);
			if(flag1 == 1){
				second_set++;
				if(second_set > 59)second_set = 0;
			}
			if(flag1 == 2){
				minute_set++;
				if(minute_set > 59)minute_set = 0;
			}
			if(flag1 == 3){
				hour_set++;
				if(hour_set > 23)hour_set = 0;
			}
			if(flag1 == 4){
				year_set++;
				if(year_set > 26)year_set = 22;
			}
			if(flag1 == 5){
				month_set++;
				if(month_set > 12)month_set = 1;
			}
			if(flag1 == 6){
				date_set++;
				if(date_set > 31)date_set = 1;
			}
			if(flag1 == 7){
				dow_set++;
				if(dow_set > 7)dow_set = 1;
			}
		}
	}
	// Nhấn phím PA4 để lưu lại thông tin (ghi giá trị vào thanh ghi của DS1307).
	if(flag == 1){
		   DS1307_SetDate(date_set);
		   DS1307_SetMonth(month_set);
		   DS1307_SetYear(year_set);
		   DS1307_SetDayOfWeek(dow_set);
		   DS1307_SetHour(hour_set);
		   DS1307_SetMinute(minute_set);
		   DS1307_SetSecond(second_set);
		   flag = 0;
		   mode = 0;
	}

}

//mode:2
void Set_Alarm(){
	char str[10];
	HD44780_Clear();
	HD44780_Cursor();
	HD44780_SetCursor(0, 0);
	HD44780_PrintStr("SET ALARM");

	HD44780_SetCursor(0, 1);
	sprintf(str, "%d", hour_set);
	HD44780_PrintStr(str);
	HD44780_PrintSpecialChar(0x3a);
	sprintf(str, "%d", minute_set);
	HD44780_PrintStr(str);
	HD44780_PrintSpecialChar(0x3a);
	sprintf(str, "%d", second_set);
	HD44780_PrintStr(str);
	HD44780_SetCursor(cursor_col, cursor_row);
	HD44780_Blink();

	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 1){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 1){
			while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 1);
			flag2++;
			if(flag2 > 3)flag2 = 1;
			if(flag2 == 1){
				cursor_row = 1;
				cursor_col = 5;
			}
			if(flag2 == 2){
				cursor_row = 1;
				cursor_col = 3;
			}
			if(flag2 == 3){
				cursor_row = 1;
				cursor_col = 1;
			}

		}
	}

	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 1){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 1){
			while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 1);
			if(flag2 == 1){
				second_set++;
				if(second_set > 59)second_set = 0;
			}
			if(flag2 == 2){
				minute_set++;
				if(minute_set > 59)minute_set = 0;
			}
			if(flag2 == 3){
				hour_set++;
				if(hour_set > 23)hour_set = 0;
			}
		}
	}

	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 1){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 1){
			while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 1);
			Alarm_flag = 1;
			mode = 0;
		}
	}

	if(flag == 1){
		flag = 0;
		Alarm_flag = 1;
		mode = 0;
	}


}

//mode:3
void Stop_Watch(){
	char str[10];
	HD44780_Clear();
	HD44780_SetCursor(0, 0);
	HD44780_PrintStr("STOP WATCH");

	HD44780_SetCursor(0, 1);
	sprintf(str, "%d", hour_3);
	HD44780_PrintStr(str);
	HD44780_PrintSpecialChar(0x3a);
	sprintf(str, "%d", minute_3);
	HD44780_PrintStr(str);
	HD44780_PrintSpecialChar(0x3a);
	sprintf(str, "%d", second_3);
	HD44780_PrintStr(str);
	HD44780_PrintSpecialChar(46);
	sprintf(str, "%ld", tick);
	HD44780_PrintStr(str);
	HD44780_NoCursor();

	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 1){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 1){
			while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 1);
			flag3 = ~flag3;
			if(flag3 != 0){
				HAL_TIM_Base_Start_IT(&htim1);
				tick = __HAL_TIM_GET_COUNTER(&htim1);
			}
			else if(flag3 == 0){
				HAL_TIM_Base_Stop_IT(&htim1);
			}
		}
	}
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 1){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 1){
			while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 1);
			mode = 0;
		}
	}

	if(flag == 1){
		hour_3 = 0;
		minute_3 = 0;
		second_3 = 0;
		tick = 0;
		__HAL_TIM_SET_COUNTER(&htim1, 0);
		flag = 0;
	}

}
