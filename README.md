# Rogalique

Двумерный рогалик на C++20 и SFML 3. Игровые сцены лежат в `Game`, компоненты и цикл в `Engine`. Кратко по механикам: [Game/README.md](Game/README.md) и [Engine/README.md](Engine/README.md).

## Сборка

Нужны Visual Studio 2026 и набор инструментов v145. Скрипт сам находит MSBuild.

Из корня репозитория:

```powershell
.\build.ps1
```

По умолчанию собирается Debug x64. Конфигурация и платформа задаются явно:

```powershell
.\build.ps1 -Configuration Release -Platform x64
```

Игра собирается в `x64\Debug\Rogalique.exe` или в такую же папку для выбранной конфигурации. Запускать её нужно из корня репозитория: пути к `Resources` относительные. Вместе с игрой собирается проект `Engine`.

## Тесты

Тесты движка на Google Test лежат в `EngineTest`. Сейчас проверяется `Vector2D`. Пакет gtest уже лежит в `packages`.

```powershell
.\build.ps1 -Tests
```

Скрипт собирает игру, затем `EngineTest`, и запускает `x64\Debug\EngineTest.exe` (папка зависит от конфигурации). Ненулевой код тестов становится кодом выхода скрипта.
