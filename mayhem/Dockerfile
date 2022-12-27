FROM  ubuntu:20.04 AS builder
RUN apt update && apt install -y --no-install-recommends\
    ca-certificates \ 
    git \ 
    gcc \ 
    make \ 
    python \ 
    libstdc++-10-dev \ 
    libcurl4-openssl-dev \ 
    zlib1g-dev \ 
    libbigwig-dev \ 
    libhts-dev \ 
    libgsl-dev \
    clang
COPY . /WiggleTools
WORKDIR /WiggleTools
RUN   make LIBS='-lwiggletools -lBigWig -lcurl -lz -lhts -lm -lgsl -lpthread' 
# RUN   make test
RUN rm /WiggleTools/src/wiggletools.o
RUN rm /WiggleTools/src/wigReader.o
RUN clang++ src/*.o /WiggleTools/mayhem/fuzz_hashfib_insert.cpp -lpthread -lm -lBigWig -lgsl -lhts -fsanitize=fuzzer,address -I /WiggleTools/src/ -o /fuzz

FROM ubuntu:20.04
RUN apt update && apt install -y --no-install-recommends \
    libbigwig0 \
    libhts3 \ 
    libgsl23
COPY   --from=builder /WiggleTools/bin/wiggletools /usr/local/bin/
COPY   --from=builder /fuzz /fuzz_hashfib_insert
