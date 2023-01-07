# Server [OPTIONS]  
## NAME:  
  TCP/UDP server   
## DESCRIPTION:  
  The program deploys a TCP and UDP server, waits for a connection from a TCP client or a message from a UDP client. After catching the message, the server finds all the numbers, sorts them and sends them to the client with their sum. If there is no number the server sends the captured message back to the client.
## OPTIONS:  
* __-ip/-i__  
	<ip> specify the server IPv4 address. Localhost is used by default.    
* __-port/-p__  
	<port> specify the server port. Port 8080 is used by default.  
* __-udp/-u__  
	ignores for the server 
* __-help/-h__    

# Client [OPTIONS]  
## NAME:  
  TCP/UDP client   
## DESCRIPTION:  
  The program deploys a TCP or UDP client, connects to the server and waits for a message in standard input. This message will be sent to the server. The client waits for a response from the server and then allows the user to send another message.  
## OPTIONS:  
* __-ip/-i__
	<ip> specify the remote server IPv4 address. Localhost is used by default.    
* __-port/-p__  
	<port> specify the remote server port. Port 8080 is used by default.  
* __-udp/-u__  
	used to specify the UDP protocol. TCP is used by default  
* __-help/-h__    




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
