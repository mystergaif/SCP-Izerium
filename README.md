# SCP-Izerium

Простая 3D-игра, использующая Raylib для графики и Bullet Physics для физического движка. В игре реализована возможность перемещения камеры, захвата и броска кубов.

## Структура проекта

```
scp-game/
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
├── build_mingw/          # Директория для сборки Bullet Physics (можно удалить после сборки)
│   └── ...
├── bullet3/              # Исходники Bullet Physics (можно удалить после сборки)
│   └── ...
├── grass.png             # Ресурсы игры (текстуры и т.д.)
└── README.md             # Этот файл
```

## Зависимости

Для сборки и запуска игры вам потребуются:

*   **Raylib:** Легковесная библиотека для разработки игр.
*   **Bullet Physics:** Мощный физический движок.
*   **MinGW-w64:** Набор инструментов для кросс-компиляции под Windows на Unix-подобных системах.

## Получение зависимостей

*   **MinGW-w64:** Установите через менеджер пакетов вашего дистрибутива Linux (например, `sudo apt install mingw-w64 g++-mingw-w64`).
*   **Raylib для MinGW-w64:** Скачайте архив с официального сайта Raylib (например, `raylib-5.5_win64_mingw-w64.zip`) и распакуйте его содержимое в директорию `lib/raylib_win64_mingw/`.
*   **Bullet Physics:** Скачайте исходники с GitHub (`git clone https://github.com/bulletphysics/bullet3.git`), перейдите в директорию `bullet3`. Для сборки под MinGW-w64, создайте директорию `build_mingw` внутри `bullet3`, перейдите в нее и выполните следующие команды CMake и make:

    ```bash
    cmake .. -G "Unix Makefiles" -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ -DCMAKE_SYSTEM_NAME=Windows -DUSE_GRAPHICAL_BENCHMARK=OFF -DBUILD_BULLET2_DEMOS=OFF -DBUILD_CPU_DEMOS=OFF -DBUILD_EXTRAS=OFF
    make
    ```

    После успешной сборки, скопируйте скомпилированные файлы библиотек (`.a`) из соответствующих поддиректорий в `bullet3/build_mingw/src/` в директорию `lib/bullet_win64_mingw/lib/`. Например:

    ```bash
    cp bullet3/build_mingw/src/LinearMath/libLinearMath.a lib/bullet_win64_mingw/lib/
    cp bullet3/build_mingw/src/BulletCollision/libBulletCollision.a lib/bullet_win64_mingw/lib/
    cp bullet3/build_mingw/src/BulletDynamics/libBulletDynamics.a lib/bullet_win64_mingw/lib/
    cp bullet3/build_mingw/src/BulletSoftBody/libBulletSoftBody.a lib/bullet_win64_mingw/lib/
    cp bullet3/build_mingw/src/BulletInverseDynamics/libBulletInverseDynamics.a lib/bullet_win64_mingw/lib/
    cp bullet3/build_mingw/src/Bullet3Common/libBullet3Common.a lib/bullet_win64_mingw/lib/
    cp bullet3/build_mingw/src/Bullet3Geometry/libBullet3Geometry.a lib/bullet_win64_mingw/lib/
    ```

## Сборка под Windows (с использованием MinGW-w64 на Linux)

Для сборки исполняемого файла под Windows (`.exe`) на машине Linux с использованием MinGW-w64, выполните следующую команду в корневой директории проекта. Убедитесь, что все зависимости получены и библиотеки Bullet Physics скопированы в `lib/bullet_win64_mingw/lib/`.

```bash
x86_64-w64-mingw32-g++ src/main.cpp -o bin/game.exe -I./lib/raylib_win64_mingw/include -L./lib/raylib_win64_mingw/lib -lraylib -I./bullet3/src -L./lib/bullet_win64_mingw/lib -lBulletDynamics -lBulletCollision -lBulletSoftBody -lBulletInverseDynamics -lBullet3Common -lLinearMath -lBullet3Geometry -static -lwinmm -lgdi32 -lopengl32 -lcomdlg32
```

## Сборка под Linux

Для сборки под Linux вам потребуется установить Raylib и Bullet Physics, скомпилированные для вашей системы Linux, с помощью вашего менеджера пакетов или скомпилировать их из исходников. Команда компиляции будет отличаться и использовать стандартный компилятор GCC/G++.

## Запуск игры (Windows)

После успешной сборки, вы найдете исполняемый файл `game.exe` в директории `bin/`. Вы можете запустить его в среде Windows.

## Начало разработки

Для начала работы над собственным проектом на основе SCP-Izerium:

1.  Клонируйте репозиторий.
2.  Убедитесь, что у вас установлены все зависимости (MinGW-w64, Raylib, Bullet Physics), как описано выше.
3.  Изучите исходный код в `src/main.cpp`. Здесь находится основная логика игры, инициализация Raylib и Bullet Physics, обработка ввода и обновление физического мира.
4.  Модифицируйте код в `src/main.cpp` для реализации своих идей. Вы можете добавлять новые объекты, изменять физические свойства, экспериментировать с графикой и вводом.
5.  После внесения изменений, пересоберите проект, используя команду сборки для Windows, указанную выше.
6.  Запустите скомпилированный `game.exe` в среде Windows для тестирования ваших изменений.

Удачи в разработке!
