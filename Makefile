OUTPUT := duca

.PHONY: build

build: c.cpp
	g++ $< -o ${OUTPUT}

check_version: ${OUTPUT}
	./${OUTPUT} -v