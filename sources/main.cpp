#include <iostream>

int main()
{
    // Получение текущего времени
    time_t now = time(nullptr);
    struct tm* timeinfo;
    timeinfo = localtime(&now);
    char buffer[80];
    strftime(buffer, 80, "Сейчас: %d.%m.%Y %H:%M:%S", timeinfo);

    std::cout << buffer << std::endl;
}