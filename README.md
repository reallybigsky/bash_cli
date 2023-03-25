# Проект bash_cli 

Простой кроссплатформенный REPL CLI интерпретатор, написанный на C++

# Как собрать?

В проекте используются Boost и Google Tests. Если на машине не установлен Boost, то cmake скачает его в папку в `build/_deps`. 
Так как Boost весит около 2 ГБ, то скачивание может занять некоторое время.

## Windows MSVC 2022

Компилятор, cmake и кодогенератор уже встроены в Microsoft Visual Studio, поэтому достаточно запустить следующие команды:

```pws
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build/
cmake --build build --config Release
```

## Windows MinGw

Для сборки необходимо поставить cmake, mingw и ninja через пакетный менеджер [choco](https://chocolatey.org/install):

```pws
choco install cmake
choco install ninja
choco install mingw
```

Далее в корневой директории проекта:

```pws
cmake -DCMAKE_BUILD_TYPE=Release -G Ninja -S . -B build/
cmake --build build --config Release
```

## Unix 

Для сборки необходимо поставить cmake, любой компилятор С++20 и любой кодогенератор.

```sh
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build/
cmake --build build --config Release
```

# Как запустить?

```
bash_cli
```

# Как протестировать?

Запустить тесты в директории `tests/test_*`

# Credits

Над проектом работали:
* Асадуллин Илья
* Арефьев Владислав
* Зинченко Даниил
* Ивашкявичус Александр
