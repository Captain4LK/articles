rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

CC = gcc
LD = gcc

CFLAGS = -MD -MP -g -Wall -Wextra -std=c99 -pedantic
LIBS = -lSDL2

SRC_ALL = $(call rwildcard,.,*.c)
SRC_STDDOC = external/stddoc.c
SRC_ARTICLES = $(call rwildcard,./articles,*.c)

articles: stddoc $(SRC_ARTICLES:%.c=%.md)

stddoc: $(SRC_STDDOC:%.c=%.o)
	$(LD) -o $@ $^ $(LIBS)

%.md: %.c
	./stddoc < $^ > $@
	cp $@ $(patsubst articles/%,../website/content/%,$@)

-include $(SRC_ALL:%.c=%.d)
