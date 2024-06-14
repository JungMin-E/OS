FROM ubuntu:latest

RUN apt-get update && \
    apt-get install -y gcc make

COPY . /chat_server
WORKDIR /chat_server

RUN gcc -o chat_server chat_server.c -pthread

CMD ["./chat_server"]
