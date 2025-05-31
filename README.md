# SCP-Izerium

Простая 3D-игра, использующая Raylib для графики и Bullet Physics для физического движка. В игре реализована возможность перемещения камеры, захвата и броска кубов.

## Структура проекта

```
SCP-Izerium/
├── src/
│   └── main.cpp          # Исходный код игры
├── lib/
│   ├── raylib_win64_mingw/ # Файлы Raylib для Windows (MinGW-w64)
│   │   ├── include/      # Заголовочные файлы Raylib
│   │   └── lib/          # Библиотеки Raylib
│   └── bullet_win64_mingw/ # Скомпилированные библиотеки Bullet Physics для Windows (MinGW-w64)
│       └── lib/          # Библиотеки Bullet Physics
├── bin/
│   └── game.exe          # Скомпилированный исполняемый файл для Windows
├── grass.png             # Ресурсы игры (текстуры и т.д.)
└── README.md             # Этот файл
```

## Зависимости

Для сборки и запуска игры вам потребуются:

*   **Raylib:** Легковесная библиотека для разработки игр.
*   **Bullet Physics:** Мощный физический движок.
*   **MinGW-w64:** Набор инструментов для кросс-компиляции под Windows на Unix-подобных системах.

## Сборка под Windows (с использованием MinGW-w64 на Linux)

Для сборки исполняемого файла под Windows (`.exe`) на машине Linux с использованием MinGW-w64, выполните следующие шаги:

1.  Установите MinGW-w64:
    ```bash
    sudo apt update
    sudo apt install mingw-w64 g++-mingw-w64
    ```
2.  Скачайте Raylib для MinGW-w64 (например, `raylib-5.5_win64_mingw-w64.zip`) с официального сайта Raylib и распакуйте его содержимое в директорию `lib/raylib_win64_mingw/`.
3.  Убедитесь, что у вас есть скомпилированные библиотеки Bullet Physics для MinGW-w64 в директории `lib/bullet_win64_mingw/lib/`. Если нет, вам потребуется скомпилировать их самостоятельно или найти готовые сборки.
4.  Выполните следующую команду в корневой директории проекта для компиляции игры:
    ```bash
    x86_64-w64-mingw32-g++ src/main.cpp -o bin/game.exe -I./lib/raylib_win64_mingw/include -L./lib/raylib_win64_mingw/lib -lraylib -L./lib/bullet_win64_mingw/lib -lLinearMath -lBulletCollision -lBulletDynamics -lBulletSoftBody -lBulletInverseDynamics -lBullet3Common -static -lwinmm -lgdi32 -lopengl32 -lcomdlg32
    ```

## Сборка под Linux

Для сборки под Linux вам потребуется установить Raylib и Bullet Physics, скомпилированные для вашей системы Linux, с помощью вашего менеджера пакетов или скомпилировать их из исходников. Команда компиляции будет отличаться и использовать стандартный компилятор GCC/G++.

## Запуск игры (Windows)

После успешной сборки, вы найдете исполняемый файл `game.exe` в директории `bin/`. Вы можете запустить его в среде Windows.
