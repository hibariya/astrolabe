FROM ubuntu:focal

RUN apt update \
      && apt -y --no-install-recommends install build-essential autoconf automake libtool wordnet wordnet-dev

RUN mkdir /work
WORKDIR /work
