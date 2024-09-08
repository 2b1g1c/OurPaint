# OurPaint
Это **система автоматизированного проектирования (САП)**, предназначенная для работы с примитивами, такими как:
- отрезок
- точка
- окружность

> [!NOTE]
> **САП "OurPaint"** - консольное приложение, которе позволяет создавать, редактировать и анализировать модели, основанные на этих примитивах. автоматизировать рутинные процессы, выводить информацию в виде монохромных изображений в формате .bmp ([BMPFile](https://github.com/sashaErkhov/bmpfile)). 
### Особенности проекта
- Гибкое редактирование требований к примитивам
- Возможность простого добавления новых требований
- Возможность написания скриптов для еще большой автоматизации процесса проектирования
### Планы по развитию
- [ ] Доработать алгоритм добавления требований( учесть что некоторые требования не могут выполняться в комбинации)
- [ ] Миграция платформы с консольного на графический интерфейс пользователя
- [ ] Добавить возможность совместных сессий
### На данной стадии
> [!IMPORTANT]
> Сейчас ведутся работы над доработкой алгоритма добавления requirements
#### Ветки:
 - main - ветка релиза. Необходим pull request (3 approve)
 - experemental-working-paint - основная ветка разработки. pull request не требуется
 - именные ветки использовать для выполнения подзадач
#### Проект:
- paint.h/.cpp основной файл проекта, в котором описан class Paint, позволяющий реализовать работу с примитивами.
- BMPpainter.h/.cpp  класс позволяющий удобно работать с классом bmp файл
- enum.h - файл с <u>перечислениями</u> всех возможных примитовов и требований
- objects.h/.cpp хранит классы примитивов
- requirement.h/.cpp классы, необходимые для реализации требований к объектам
