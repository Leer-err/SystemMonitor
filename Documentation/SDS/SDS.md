# 1. Use case

![alt text](/Documentation/SDS/Use%20case%20diagram.png "Use case")

## Актёры
| Актёр | Описание |
| ----------- | ----------- |
| База данных | Хранит информацию о процессах и потоках |
| Пользователь | Человек, работающий с приложением |

## 1.1 Просмотреть процессы

Позволяет пользователю просмотреть все процессы в системе и основную информацию о них.

 1. Пользователь нажимает на кнопку активации режима, либо запускает приложение; 
 2. Программа получает данные о процессах от базы данных;
 3. Программа входит в цикл вывода новой информации;
 4. Конец.

## 1.2 Просмотреть потоки

Позволяет пользователю просмотреть все потоки в системе и основную информацию о них.

 1. Пользователь нажимает на кнопку активации режима; 
 2. Программа получает данные о потоках от базы данных;
 3. Программа входит в цикл вывода новой информации;
 4. Конец.

## 1.3 Завершить процесс

Позволяет пользователю завершить конкретный процесс.

 1. Пользователь нажимает на кнопку активации; 
 2. Пользователь вводит идентификатор процесса, который необходимо завершить;
 3. Программа проводит проверку на существование процесса с данным идентификатором и, в случае отсутствия, переходит к пункту 4, иначе к пункту 6;
 4. Данные передаются системе;
 5. Программа проверяет результат на ошибку и, в случае отсутствия переходит к пункту 7;
 6. Вывод сообщения об ошибке;
 7. Конец.

## 1.4 Отсортировать по потреблению памяти

Позволяет пользователю изменить режим сортировки для процессов.

 1. Пользователь нажимает на кнопку активации режима;
 2. Программа изменяет режим отображения;
 3. Программа входит в цикл вывода информации;
 4. Конец.

## 1.4 Отсортировать по использованию ЦП

Позволяет пользователю изменить режим сортировки для процессов и потоков.

 1. Пользователь нажимает на кнопку активации режима;
 2. Программа изменяет режим отображения;
 3. Программа входит в цикл вывода информации;
 4. Конец.

# 2 Диаграммы активностей

## 2.1 Отправка сигнала

![alt text](/Documentation/SDS/Activity%20diagram%201.png "Activity")

## 2.2 Обновление информации

![alt text](/Documentation/SDS/Activity%20diagram%202.png "Activity")

# 3 Диаграммы последовательностей

## 3.1 Отправка сигнала

![alt text](/Documentation/SDS/Sequence%20diagram%201.png "Sequence")

## 3.2 Обновление информации

![alt text](/Documentation/SDS/Sequence%20diagram%202.png "Sequence")

# 4 Диаграмма состояний

![alt text](/Documentation/SDS/State%20diagram.png "State")

# 5 Диаграмма классов

![alt text](/Documentation/SDS/Class%20Diagram.png "Class diagram")

# 6 Диаграмма компонентов и развёртывания

![alt text](/Documentation/SDS/Deployment%20diagram.png"Deployment diagram")

# 7 Пример интерфейса

![alt text](/Documentation/SRS/Main%20window.png "Main window")

# 8 Дополнительные требования

1. Язык C++;
2. Использование интерфейса procfs;
3. Оконная библиотека slint;