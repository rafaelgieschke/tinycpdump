FROM alpine
RUN apk add gcc make musl-dev
WORKDIR /src
COPY . .
RUN make -B CC=gcc
