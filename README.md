# RPM_LED
Контроллер: STM32F042K6T6  
Тактирование от внешнего кварца 8Mhz, работает на 48Mhz.  
  
Получает по CAN-шине данные об оборотах (https://en.wikipedia.org/wiki/OBD-II_PIDs), выводит через сдвиговые регистры на RGB светодиоды.  
CAN передатчик: SN65HVD230  
Сдвиговый регистр (2 штуки последовательно): STP16CP05MTR 
  
Проект сделан в TrueStudio 9.1    
  
 Аппаратная часть: https://github.com/rallyfanru/RPM_LED_HW

