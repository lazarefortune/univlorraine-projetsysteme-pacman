FROM gcc:latest

WORKDIR /app

COPY main.c .

RUN gcc -o main main.c

CMD ["./main"]
