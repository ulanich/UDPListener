# Парсер телеметрического потока UDPListener

### Shortcuts:
1. Подключение к порту: Ctrl + C
2. Отключение: Ctrl + D
3. Отображение последних телеметрических пакетов: Ctrl + X

### Примечания:
1. Возможно, в структуре "Текущее системное время" поменяны местами часы и секунды. Чтобы использовать исправленную структуру, нужно убрать #define UNCORRECTED_VER [InfoStructs.h](https://github.com/ulanich/UDPListener/blob/main/InfoStructs.h).
2. Проверено на VS 19 с Qt VS Tools, Qt - qt_5_15_2_msvc2019_64_msvc2019_64.