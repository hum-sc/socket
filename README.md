# Socket

1. socket(pf, type, protocol): Constructor del socket
2. socketpair(pf, type, protocol,sarray): Crea un par de sockets
3. close(socket): Cierra el socket (decrementa las referencias al socket, esto debido a que varias aplicaciones pueden)
utilizar el mismo socket).
4. bind(socket, address, addrlen): Asocia una dirección a un socket
5. connect(socket, destaddr, addrlen): Establece una conexión con un socket remoto
6. Send data
   1. send
   2. sendto(socket, message, length, flags, destaddr, addrlen) Enviar datos a un socket servicio no orientado a la conexión
   3. sendmsg(socket, messagestruct, flags) Enviar datos a un socket (permite enviar datos a varios sockets
   4. write(socket, buffer, length) Enviar datos a un socket
   5. writev(socket, iovector, vectorlen) Enviar datos a un socket sin tener que copiar el mensaje a bytes contiguos 
   de memoria
   6. send(socket, message, length, flags) Enviar datos a un socket