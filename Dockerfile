FROM ghcr.io/motis-project/docker-cpp-build AS builder

WORKDIR /motis
COPY . .
RUN cmake -G Ninja -S . -B build --preset=linux-amd64-release \
&& cmake --build build --target motis motis-web-ui
RUN ./build/motis import

FROM alpine:3.20
COPY --from=builder /motis/build/motis .
COPY --from=builder /motis/ui/build ui
COPY --from=builder /motis/deps/tiles/profile tiles-profile
COPY --from=builder /motis/data data
#RUN adduser --system --group motis && \
#    mkdir data && \
#    chown motis:motis data
#USER motis
EXPOSE 8080
CMD ["/motis", "server", "/data"]
