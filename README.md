# GoodEngine

Кроссплатформенный игровой движок на **C++20** с рендером на **OpenGL 3.3** и собственным редактором.
Весь низкий уровень написан вручную: математика, окно, рендер, UI, логирование — без внешних зависимостей.

> by **DNXzMDS** · C++20 · OpenGL 3.3 · CMake 3.20+

---

## Структура

```
GoodEngine/
├── Engine/     # Ядро движка (статическая библиотека GoodEngine)
│   ├── Core/           # Engine, App, Loop, Log, Time, Assert
│   ├── Math/           # Vec2/3/4, Mat4, Quat, Transform, AABB
│   ├── Platform/       # Окно, загрузка GL-функций (Win32 / X11 / Cocoa)
│   ├── Graphics/       # Шейдеры, текстуры, буферы, Framebuffer (OpenGL 3.3)
│   ├── Rendering/      # Renderer, Mesh, Material, SpriteRenderer, RenderPass
│   ├── Scene/          # Scene, SceneGraph, SceneNode, SceneManager
│   ├── Components/     # Компоненты узлов (RigidBody, коллайдеры, скрипты)
│   ├── Physics/        # PhysicsWorld, RigidBody, коллайдеры, Raycast
│   ├── UI/             # Своя UI-система: виджеты, FontAtlas, IconAtlas
│   ├── Input/          # Клавиатура, мышь, геймпад
│   ├── Audio/          # AudioSystem, mixer, source (WIP)
│   ├── Networking/     # Сокеты, клиент/сервер, пакеты (WIP)
│   ├── Scripting/      # ScriptRuntime, ScriptBind (WIP — заглушка)
│   ├── Serialization/  # JSON, SceneSerializer
│   ├── FileSystem/     # Path, FileSystem, VirtualFileSystem, FileWatcher
│   └── ...
├── Editor/     # Редактор (GoodEditor)
│   ├── Panels/   # Hierarchy, Inspector, Viewport, Console, ContentBrowser, Profiler
│   ├── Windows/  # MainWindow
│   ├── Gizmos/   # Трансформ-гизмо
│   ├── Themes/   # DarkTheme
│   └── Assets/   # Font.ttf, icons.png
└── Project/    # Пример игрового проекта (GoodProject)
```

## Сборка

Требуется **CMake 3.20+** и компилятор с поддержкой C++20
(MSVC 2019/2022, GCC 10+, Clang 12+).

```bash
git clone https://github.com/MeaskedDeadline/GoodEngine.git
cd GoodEngine

cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Бинарники появятся в `build/bin/`:

- **`GoodEditor`** — редактор (шрифт и иконки ищет в `Editor/Assets/`
  относительно рабочей директории, умеет fallback на системные шрифты)
- **`GoodProject`** — минимальный пример игрового приложения

### Linux

Понадобятся заголовки X11 и OpenGL:

```bash
sudo apt install libx11-dev libgl1-mesa-dev   # Debian/Ubuntu
```

## Редактор

- Панели: Hierarchy, Inspector, Viewport, Console, Content Browser, Profiler, Scene
- Отладочная консоль с командами: `help`, `clear`, `scene.info`, `time.info`, `font.load`
- Сохранение/загрузка сцен в `Scenes/scene.json` (File → Save/Load)
- Undo/Redo (`Core/UndoSystem`), темная тема, программная генерация иконок,
  если файлы атласа не найдены

## Roadmap

- [ ] Встроить настоящий язык скриптов (сейчас `Scripting/` — заглушка)
- [ ] Довести до ума аудио (плеер, микшер)
- [ ] Сетевая игра поверх `Networking/`
- [ ] Тесты для Math/Physics

## Лицензия

Проект учебный — делай что хочешь, но укажи автора.
