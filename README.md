# Protei

Необходимо разработать два приложения под Linux: клиент и сервер.

Клиент посылает введенное пользователем на консоль сообщение, сервер при получении данного сообщения обрабатывает его: находит в данном сообщении все числа, сортирует их, вычисляет сумму и отправляет в ответ результат. Если в сообщении от клиента нет ни одного числа, то сервер должен просто отправить в ответ клиенту полученное сообщение. Можно считать, что для хранения каждого из чисел в сообщении хватает размерности встроенных типов.


Допустим, клиент отправил сообщение

20 apples, 30 bananas, 15 peaches and 1 watermelon

в ответ он должен получить сообщение

1 15 20 30

66


Обязательные требования:

При реализации используйте С++/C (использование STL приветствуется, сторонних библиотек - нет);
Работа с сетью должна выполняться средствами Berkeley sockets API (использование сторонних фреймворков для работы с сетью не допускается);
Клиент и сервер должны поддерживать протоколы TCP и UDP;
Сервер должен корректно обслуживать несколько одновременно подключенных клиентов по любому из перечисленных выше протоколов;
Клиент может выбирать (при старте приложения), какой протокол будет использован для взаимодействия с сервером, при этом должна быть возможность отправки нескольких сообщений без перезапуска или переустановки соединения.

На что стоит обратить внимание:

Приложения должны легко собираться, приложениями должно быть легко и удобно пользоваться, они должны быть рассчитаны на длительную и стабильную работу, а также иметь возможность корректно завершаться;
Специфика работы с сетью в том, что многое может пойти не так (ошибки, некорректные данные, некорректное поведение удаленной стороны), приложения должны быть готовы к этому, учитывайте разницу между протоколами TCP и UDP;
По возможности старайтесь сделать реализацию эффективной и надежной (возможно удастся обойтись без использования потоков (threads) или сократить накладные расходы при их использовании).
