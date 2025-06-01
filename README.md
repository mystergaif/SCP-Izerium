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

Для успешной сборки проекта вам необходимо убедиться, что у вас установлены следующие зависимости:

1.  **MinGW-w64:** Этот набор инструментов необходим для кросс-компиляции проекта под Windows на вашей системе Linux. Установите его с помощью менеджера пакетов вашего дистрибутива. Например, для Debian/Ubuntu:
    ```bash
    sudo apt update
    sudo apt install mingw-w64 g++-mingw-w64
    ```

2.  **Raylib и Bullet Physics:** Необходимые скомпилированные библиотеки Raylib и Bullet Physics для Windows (MinGW-w64) уже включены в репозиторий в директории `lib/`. Вам не нужно скачивать или компилировать их отдельно, если вы собираете под Windows с использованием MinGW-w64 на Linux.

    *   Если вам необходимо собрать Bullet Physics из исходников (например, для другой платформы или конфигурации), вы можете скачать их с GitHub (`git clone https://github.com/bulletphysics/bullet3.git`) и следовать инструкциям по сборке Bullet Physics для вашей целевой платформы. Инструкции по сборке Bullet Physics под MinGW-w64 на Linux приведены ниже для справки, но обычно это не требуется, так как библиотеки уже предоставлены.

    **Сборка Bullet Physics под MinGW-w64 (для справки):**
    Перейдите в директорию `bullet3`, создайте директорию `build_mingw` внутри нее, перейдите в `build_mingw` и выполните следующие команды CMake и make:

    ```bash
    cd bullet3
    mkdir build_mingw
    cd build_mingw
    cmake .. -G "Unix Makefiles" -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ -DCMAKE_SYSTEM_NAME=Windows -DUSE_GRAPHICAL_BENCHMARK=OFF -DBUILD_BULLET2_DEMOS=OFF -DBUILD_CPU_DEMOS=OFF -DBUILD_EXTRAS=OFF
    make
    ```
    После успешной сборки, скопируйте скомпилированные файлы библиотек (`.a`) из соответствующих поддиректорий в `bullet3/build_mingw/src/` в директорию `lib/bullet_win64_mingw/lib/` в корне вашего проекта. Например:
    ```bash
    cp bullet3/build_mingw/src/LinearMath/libLinearMath.a ../../lib/bullet_win64_mingw/lib/
    cp bullet3/build_mingw/src/BulletCollision/libBulletCollision.a ../../lib/bullet_win64_mingw/lib/
    cp bullet3/build_mingw/src/BulletDynamics/libBulletDynamics.a ../../lib/bullet_win64_mingw/lib/
    cp bullet3/build_mingw/src/BulletSoftBody/libBulletSoftBody.a ../../lib/bullet_win64_mingw/lib/
    cp bullet3/build_mingw/src/BulletInverseDynamics/libBulletInverseDynamics.a ../../lib/bullet_win64_mingw/lib/
    cp bullet3/build_mingw/src/Bullet3Common/libBullet3Common.a ../../lib/bullet_win64_mingw/lib/
    cp bullet3/build_mingw/src/Bullet3Geometry/libBullet3Geometry.a ../../lib/bullet_win64_mingw/lib/
    ```
    (Примечание: пути для `cp` могут отличаться в зависимости от вашего текущего положения в терминале. Убедитесь, что вы копируете файлы в правильную директорию `lib/bullet_win64_mingw/lib/` относительно корня проекта.)

## Сборка под Windows (с использованием MinGW-w64 на Linux)

Следуйте этим шагам для сборки исполняемого файла под Windows (`.exe`) на вашей машине Linux с использованием MinGW-w64:

1.  **Клонируйте репозиторий:** Если вы еще этого не сделали, клонируйте проект с GitHub:
    ```bash
    git clone https://github.com/mystergaif/SCP-Izerium.git
    cd SCP-Izerium
    ```

2.  **Установите MinGW-w64:** Убедитесь, что MinGW-w64 установлен в вашей системе (см. раздел "Получение зависимостей").

3.  **Выполните сборку:** В корневой директории проекта выполните следующую команду в терминале. Эта команда использует кросс-компилятор `x86_64-w64-mingw32-g++` для компиляции исходного файла `src/main.cpp` и компоновки его с необходимыми библиотеками Raylib и Bullet Physics, расположенными в директории `lib/`.

    ```bash
    x86_64-w64-mingw32-g++ src/main.cpp -o bin/game.exe -I./lib/raylib_win64_mingw/include -L./lib/raylib_win64_mingw/lib -lraylib -I./bullet3/src -L./lib/bullet_win64_mingw/lib -lBulletDynamics -lBulletCollision -lBulletSoftBody -lBulletInverseDynamics -lBullet3Common -lLinearMath -lBullet3Geometry -static -lwinmm -lgdi32 -lopengl32 -lcomdlg32
    ```
    *   `-o bin/game.exe`: Указывает выходной файл исполняемого файла в директории `bin/`.
    *   `-I./lib/raylib_win64_mingw/include`: Добавляет директорию с заголовочными файлами Raylib в пути поиска.
    *   `-L./lib/raylib_win64_mingw/lib`: Добавляет директорию с библиотеками Raylib в пути поиска.
    *   `-lraylib`: Компонует с библиотекой Raylib.
    *   `-I./bullet3/src`: Добавляет директорию с заголовочными файлами Bullet Physics в пути поиска.
    *   `-L./lib/bullet_win64_mingw/lib`: Добавляет директорию с библиотеками Bullet Physics в пути поиска.
    *   `-lBulletDynamics -lBulletCollision -lBulletSoftBody -lBulletInverseDynamics -lBullet3Common -lLinearMath -lBullet3Geometry`: Указывает библиотеки Bullet Physics для компоновки.
    *   `-static`: Статическая компоновка стандартных библиотек MinGW-w64.
    *   `-lwinmm -lgdi32 -lopengl32 -lcomdlg32`: Дополнительные системные библиотеки Windows, необходимые для Raylib.

4.  **Проверьте результат:** После успешного выполнения команды, исполняемый файл `game.exe` будет создан в директории `bin/`.

## Сборка под Linux

Для сборки под Linux вам потребуется установить Raylib и Bullet Physics, скомпилированные для вашей системы Linux, с помощью вашего менеджера пакетов или скомпилировать их из исходников. Команда компиляции будет отличаться и использовать стандартный компилятор GCC/G++. Вам нужно будет адаптировать пути к заголовочным файлам и библиотекам в соответствии с вашей установкой.

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
