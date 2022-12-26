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
    libgsl-dev 
ADD . /WiggleTools
WORKDIR /WiggleTools
RUN   make LIBS='-lwiggletools -lBigWig -lcurl -lz -lhts -lm -lgsl -lpthread' 
RUN   make test

FROM ubuntu:20.04
RUN apt update && apt install -y --no-install-recommends \
    libbigwig0 \
    libhts3 \ 
    libgsl23
COPY   --from=builder /WiggleTools/bin/wiggletools /usr/local/bin/
