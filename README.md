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

3.  **Выполните сборку:** В корневой директории проекта находится скрипт `build_windows.sh`, который автоматизирует процесс сборки исполняемого файла для Windows с использованием MinGW-w64. Убедитесь, что скрипт имеет права на выполнение (`chmod +x build_windows.sh`), а затем запустите его в терминале:

    ```bash
    ./build_windows.sh
    ```

4.  **Проверьте результат:** После успешного выполнения скрипта, исполняемый файл `game.exe` будет создан в директории `bin/`. Скрипт также выведет сообщение об успешной или неудачной сборке.

## Сборка под Linux

Для сборки исполняемого файла под Linux вам потребуется установить Raylib и Bullet Physics, скомпилированные для вашей системы Linux. Вы можете установить их через менеджер пакетов вашего дистрибутива или скомпилировать из исходников.

После установки зависимостей, перейдите в корневую директорию проекта в терминале и выполните следующую команду для сборки. Эта команда использует компилятор `clang++`, но вы также можете использовать `g++` с аналогичными флагами.

```bash
clang++ src/main.cpp -o game \
    -I. \
    -I./bullet3/src \
    -L./bullet3/build/src/BulletDynamics \
    -L./bullet3/build/src/BulletCollision \
    -L./bullet3/build/src/LinearMath \
    -L./bullet3/build/src/BulletSoftBody \
    -lraylib \
    -lBulletDynamics \
    -lBulletCollision \
    -lLinearMath \
    -lBulletSoftBody \
    -lGL -lm -lpthread -ldl -lrt -lX11
```

*   `-o game`: Указывает выходной файл исполняемого файла.
*   `-I.`: Добавляет текущую директорию в пути поиска заголовочных файлов.
*   `-I./bullet3/src`: Добавляет директорию с заголовочными файлами Bullet Physics в пути поиска.
*   `-L./bullet3/build/src/...`: Указывает директории с библиотеками Bullet Physics. **Примечание:** Эти пути предполагают, что вы собрали Bullet Physics в директории `bullet3/build/`. Если вы установили Bullet Physics через менеджер пакетов, вам может потребоваться изменить эти пути или удалить их, если библиотеки находятся в стандартных системных директориях.
*   `-lraylib`: Компонует с библиотекой Raylib.
*   `-lBulletDynamics -lBulletCollision -lLinearMath -lBulletSoftBody`: Указывает библиотеки Bullet Physics для компоновки. **Примечание:** Список библиотек Bullet Physics может варьироваться в зависимости от того, как вы их установили или скомпилировали.
*   `-lGL -lm -lpthread -ldl -lrt -lX11`: Дополнительные системные библиотеки, часто необходимые для графических приложений на Linux.

После успешного выполнения команды, исполняемый файл `game` будет создан в корневой директории проекта.

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
