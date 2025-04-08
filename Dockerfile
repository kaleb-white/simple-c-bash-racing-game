FROM gcc:latest

COPY . .

RUN apt-get update
RUN apt-get install libsdl2-dev -y
RUN apt-get install python3-opencv -y
